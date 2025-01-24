#include "Graphics.h"
#include "Filesystem.h"
#include "Logic.h"
#include "Physics.h"

#define rad(x) radians((float)x)

int main()
{

	//INITIALIZATION

	Base::InitGLFW();
	stbi_set_flip_vertically_on_load(true); //Flip textures on the load for correct render

	Graphics graphics;
	ShaderProgram program;

	graphics.Init(String("Vertex.glsl;Pixel.glsl;"));

	float vertices[] = {
		-1.f, -1.f, 0.0f, 0.f, 0.f,
		-1.f, 1.f, 0.0f, 0.f, 1.f,
		1.f, 1.f, 0.0f, 1.f, 1.f,
		-1.f, -1.f, 0.0f, 0.f, 0.f,
		1.f, 1.f, 0.0f, 1.f, 1.f,
		1.f, -1.f, 0.0f, 1.f, 0.f
	};

	String v((char*)vertices, 30 * 4);

	uint model = graphics.CreateModel(v);
	uint texture = graphics.CreateTexture("image.png");

	Logic::Scene scene;
	uint first = scene.CreateObject(true, vec3(2, 0, 2), vec3{}, vec3{ 100, 100, 0 }, model, texture);
	uint second = scene.CreateObject(true, vec3(-5, 0, 2), vec3(0), vec3(100, 100, 0), model, texture);

	Logic::Camera plr;
	plr.fieldOfView = 1.4f; //80degr
	plr.cameraRadian = vec3(1.56f, 0, 0); //89degr
	plr.cameraSensitivity = 0.0005f;

	Physics::AABB aabb, aabb1;
	aabb.rotation = vec3(6.28, 0, 0);
	aabb.ParseModel(vertices, 6, 5);
	aabb1.ParseModel(vertices, 6, 5);

	aabb.Collision(aabb1);

	//Report("Minimum pos: ");
	//Report(aabb.pointMin.x);
	//Report(' ');
	//Report(aabb.pointMin.y);
	//Report(' ');
	//Report(aabb.pointMin.z);

	//Report("\nMaximum pos: ");
	//Report(aabb.pointMax.x);
	//Report(' ');
	//Report(aabb.pointMax.y);
	//Report(' ');
	//Report(aabb.pointMax.z);
	//Report('\n');

	vec3 rotation{ 3.14, 3.14, 0 };
	vec3 rot;
	mat4 quater;



	while (!glfwWindowShouldClose(Base::window))
	{
		Base::Routine();



		//if (Base::isWindowInFocus) plr.UpdateView(Base::mouseDelta);

		//plr.body.position += Logic::MoveWASD(plr.cameraRadian) * 3.f;

		if (Base::GetKey(GLFW_KEY_RIGHT))
			scene.data.Get(first)->position.x -= 10;
		else if (Base::GetKey(GLFW_KEY_LEFT))
			scene.data.Get(first)->position.x += 10;

		if(Base::GetKey(GLFW_KEY_UP)) scene.data.Get(first)->position.y += 10;
		else if (Base::GetKey(GLFW_KEY_DOWN)) scene.data.Get(first)->position.y -= 10;


		if(scene.CollisionTest(first, second)) scene.data.Get(first)->position.x += 1.f;

		ClearBuffer(COLOR | DEPTH);

		graphics.DrawQueue(&scene.data, glm::lookAtRH(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 0)), orthoRH(-400.f, 400.f, -300.f, 300.f, 0.1f, 100.f));

		glfwSwapBuffers(Base::window);
	}

	glfwTerminate();

	return 1;
}