#include "Storage.h"
#include "Init.h"

namespace UI
{

	class Object
	{
	public:
		vec2 position{};
		float rotation{};
		vec2 size{};
		uint texture{};
		bool toRender = false;

		mat4 GetWorld()
		{
			mat4 world(1);
			world = translate(world, vec3(position, 0.0f));
			world = scale(world, vec3(size, 1));
			world = rotate(world, rotation, vec3(0, 0, 1));
			return world;
		}
	};

	class UserInterface
	{
	public:
		Storage<Object> storage;
		uint defaultModel{};
		mat4 view{};
		mat4 projection{};

		void Initialization(uint default_model)
		{
			storage.Resize(100);
			defaultModel = default_model;
			projection = glm::orthoRH(0.0f, (float)Base::windowSizeX, 0.0f, (float)Base::windowSizeY, 0.01f, 100.0f);
			view = glm::lookAtRH(vec3(0, 0, 1), vec3(0, 0, 0), vec3(0, 1, 0));
		}

		uint CreateElement(vec2 position, vec2 size, float rotation, uint texture)
		{
			Object* uiElement = new Object;
			uiElement->position = position;
			uiElement->size = size;
			uiElement->rotation = rotation;
			uiElement->texture = texture;

			return storage.Set(uiElement);
		}

		Object* Get(uint id)
		{
			return storage.Get(id);
		}
	};
}