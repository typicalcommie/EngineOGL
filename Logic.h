#pragma once
#include "Init.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
typedef unsigned int uint;

namespace Logic
{

	glm::mat4 Rotate(float x, float y, float z)
	{
		glm::mat4 rotateX{ 1 }, rotateY{ 1 }, rotateZ{ 1 };

		rotateX = glm::rotate(rotateX, x, glm::vec3(1, 0, 0));
		rotateY = glm::rotate(rotateY, y, glm::vec3(0, 1, 0));
		rotateZ = glm::rotate(rotateZ, z, glm::vec3(0, 0, 1));

		return rotateX * rotateY * rotateZ;
	}

	glm::mat4 Rotate(vec3 degrees)
	{
		glm::mat4 rotateX{ 1 }, rotateY{ 1 }, rotateZ{ 1 };

		rotateX = glm::rotate(rotateX, degrees.x, glm::vec3(1, 0, 0));
		rotateY = glm::rotate(rotateY, degrees.y, glm::vec3(0, 1, 0));
		rotateZ = glm::rotate(rotateZ, degrees.z, glm::vec3(0, 0, 1));

		return rotateX * rotateY * rotateZ;
	}

	vec3 MoveWASD(vec3 view_radian)
	{
		vec3 target = { cosf(view_radian.x) * cosf(view_radian.y), cosf(view_radian.y), sinf(view_radian.x) * cosf(view_radian.y) };

		vec3 result{};

		if (Base::GetKey('W'))
		{
			 result += vec3(cos(view_radian.x), 0.0f, sin(view_radian.x)) * 0.01f;
		}

		else if (Base::GetKey('S'))
		{
			result -= vec3(cos(view_radian.x), 0.0f, sin(view_radian.x)) * 0.01f;
		}

		if (Base::GetKey('A'))
		{
			result -= normalize(cross(target, vec3(0, 1, 0))) * 0.01f;
		}

		else if (Base::GetKey('D'))
		{
			result += normalize(cross(target, vec3(0, 1, 0))) * 0.01f;
		}
		return result;
	}

	class Object
	{
	public:
		bool toRender = false;
		vec3 position{};
		vec3 rotation{};
		vec3 scale{};
		uint model{};
		uint texture{};


		mat4 World()
		{
			mat4 j(1);

			j = translate(j, position);
			j = glm::scale(j, scale);
			j *= Rotate(rotation);


			return j;
		}
	};

	class Scene
	{

	public:
		Storage<Object> data;
		bool toRender = false;
		vec3 position{};
		vec3 size{1, 1, 1};

		Scene()
		{
			data.Resize(100);
		}

		uint CreateObject(bool to_render, vec3 position, vec3 rotation, vec3 scale, uint model, uint texture)
		{
			Object* obj = new Object;
			obj->toRender = to_render;
			obj->position = position;
			obj->rotation = rotation;
			obj->scale = scale;
			obj->texture = texture;
			
			return data.Set(obj);
		}

		bool CollisionTest(uint first_object_id, uint second_object_id)
		{
			Object* first = data.Get(first_object_id);
			Object* second = data.Get(second_object_id);

			vec3 lowestPoint1, highestPoint1, lowestPoint2, highestPoint2;

			lowestPoint1 = first->position - first->scale;
			highestPoint1 = first->position + first->scale;
			lowestPoint2 = second->position - second->scale;
			highestPoint2 = second->position + second->scale;

			bool X = (lowestPoint1.x <= highestPoint2.x && lowestPoint1.x >= lowestPoint2.x) || (lowestPoint2.x <= highestPoint1.x && lowestPoint2.x >= lowestPoint1.x);
			bool Y = (lowestPoint1.y <= highestPoint2.y && lowestPoint1.y >= lowestPoint2.y) || (lowestPoint2.y <= highestPoint1.y && lowestPoint2.y >= lowestPoint1.y);


			return X && Y;
		}


	};

	enum CameraType
	{
		FIXATED_2D, FOLLOWING_2D, FOLLOWING_3D
	};

	class Camera
	{

	public:
		Object body;

		vec3 cameraRadian{};
		vec3 cameraOffset{};
		float cameraSensitivity{};
		float fieldOfView{};
		void UpdateView(double2 mouse_delta)
		{
			cameraRadian += vec3(mouse_delta.x, mouse_delta.y, 0) * cameraSensitivity; 
		}

		mat4 View(CameraType type)
		{
			vec3 finalPosition = cameraOffset + body.position;
			switch (type)
			{
				case FOLLOWING_3D:
				{
					if (cameraRadian.y > 1.56f) cameraRadian.y = 1.56f;			//Clamping Y axis.
					else if (cameraRadian.y < -1.56f) cameraRadian.y = -1.56f;

					float cosX = cosf(cameraRadian.x), sinX = sinf(cameraRadian.x);	//Get cos and sin of X axis, then restrict it magnitude with cos of Y axis. If angle by Y axis increaces, share of other two axis will be decreased accordingly.
					cosX *= cosf(cameraRadian.y);	sinX *= cosf(cameraRadian.y);

					return lookAtRH(finalPosition, finalPosition + normalize(vec3(cosX, sinf(cameraRadian.y), sinX)), vec3(0, 1, 0));
					break;
				}
				case FOLLOWING_2D:
					return lookAtRH(vec3(finalPosition.x, finalPosition.y, 0), vec3(finalPosition.x, finalPosition.y, 1), vec3(0, 1, 0));
					break;

				case FIXATED_2D:
					return lookAtRH(vec3(cameraOffset.x, cameraOffset.y, 0), vec3(cameraOffset.x, cameraOffset.y, 1), vec3(0, 1, 0));
					break;
			}


		}

		mat4 Projection(CameraType type)
		{
			if (Base::windowSizeX > 0 && Base::windowSizeY > 0)
			{
				if (type == FOLLOWING_3D)
					return perspectiveFovRH(fieldOfView, (float)Base::windowSizeX, (float)Base::windowSizeY, 0.01f, 1000.f);	//It's mandatory to have zNear greater than zero, otherwise depth testing not gonna work.
				else return orthoRH(0.f, (float)Base::windowSizeX, 0.f, (float)Base::windowSizeY, 0.01f, 1000.f);
			}
			else return mat4(1);
		}
	};

}