#include "Graphics.h"
#include "Filesystem.h"
#include "OGLOverlay.h"
#include "Logic.h"

#define rad(x) radians((float)x)

int main()
{

	//INITIALIZATION

	Base::InitGLFW();
	stbi_set_flip_vertically_on_load(true); //Flip textures on the load for correct render

	//SOME MAGIC

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
	uint first = scene.CreateObject(true, vec3(2, 0, 2), vec3{}, vec3{ 1, 1, 0 }, model, texture);
	uint second = scene.CreateObject(true, vec3(-5, 0, 2), vec3(0), vec3(1, 1, 0), model, texture);

	Logic::Camera plr;
	plr.fieldOfView = 1.4f; //80degr
	plr.cameraRadian = vec3( 1.56f, 0, 0 ); //89degr
	plr.cameraSensitivity = 0.0005f;

	while (!glfwWindowShouldClose(Base::window))
	{
		Base::Routine();

		if (Base::isWindowInFocus) plr.UpdateView(Base::mouseDelta);

		plr.body.position += Logic::MoveWASD(plr.cameraRadian) * 3.f;

		if (Base::GetKey(GLFW_KEY_RIGHT))
			scene.data.Get(first)->position.x -= 0.1f;
		else if (Base::GetKey(GLFW_KEY_LEFT))
			scene.data.Get(first)->position.x += 0.1f;

		if(Base::GetKey(GLFW_KEY_UP)) scene.data.Get(first)->position.y += 0.1f;
		else if (Base::GetKey(GLFW_KEY_DOWN)) scene.data.Get(first)->position.y -= 0.1f;


		if(scene.CollisionTest(first, second)) scene.data.Get(first)->position.x += 0.1f;

		ClearBuffer(COLOR | DEPTH);


		graphics.DrawQueue(&scene.data, glm::lookAtRH(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 0)), orthoRH(0, 800, 0, 600, 0, 100));

		glfwSwapBuffers(Base::window);
	}

	glfwTerminate();

	return 1;
}