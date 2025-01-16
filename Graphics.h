#pragma once
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Filesystem.h"
#include "Storage.h"
#include "Logic.h"
#include "UI.h"

#define sof(x) sizeof(x)

using namespace glm;

typedef unsigned int uint;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_cords;
};


enum BufferDataType
{
	//Loaded oncem, used merely few times.
	STREAM = GL_STREAM_DRAW,
	//Loaded once, often used.
	STATIC = GL_STATIC_DRAW,
	//Often updates, often used.
	DYNAMIC = GL_DYNAMIC_DRAW
};

enum ShaderType
{
	VERTEX = GL_VERTEX_SHADER, TESSELATION = GL_TESS_CONTROL_SHADER, EVALUATION = GL_TESS_EVALUATION_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, PIXEL = GL_FRAGMENT_SHADER, COMPUTE = GL_COMPUTE_SHADER
};

enum ShaderStatus
{
	//Return true, if shader has been flaged for deletion.
	IS_DELETED = GL_DELETE_STATUS,

	//Returns value of ShaderType.
	TYPE = GL_SHADER_TYPE,

	//Returns true if compiled.
	IS_COMPILED = GL_COMPILE_STATUS,

	//Returns lenght of log. Null terminator included. If log is empty, returns zero.
	LOG_LENGHT = GL_INFO_LOG_LENGTH,

	//Returns lenght of source code. Null terminator included. If there was no source code, returns zero.
	SOURCE_LENGHT = GL_SHADER_SOURCE_LENGTH
};

enum BufferType
{
	VERTEX_BUFFER = GL_ARRAY_BUFFER, firstIteratorTOMIC_COUNTER_STORAGE = GL_ATOMIC_COUNTER_BUFFER, COPY_READ = GL_COPY_READ_BUFFER, COPY_WRITE = GL_COPY_WRITE_BUFFER,
	VERTEX_INDICES = GL_ELEMENT_ARRAY_BUFFER, TEXTURE = GL_TEXTURE_BUFFER,
};

enum ClearType
{
	COLOR = GL_COLOR_BUFFER_BIT, DEPTH = GL_DEPTH_BUFFER_BIT, STENCIL = GL_STENCIL_BUFFER_BIT
};

enum TextureType
{
	TEXTURE1D = GL_TEXTURE_1D, TEXTURE1D_ARRAY = GL_TEXTURE_1D_ARRAY, TEXTURE2D = GL_TEXTURE_2D, TEXTURE2D_ARRAY = GL_TEXTURE_2D_ARRAY, TEXTURE2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE, TEXTURE2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

enum TextureParameterType
{
	DEPTHSTENCIL_MODE = GL_DEPTH_STENCIL_TEXTURE_MODE, BASE_LEVEL = GL_TEXTURE_BASE_LEVEL, COMPARE_FUNC = GL_TEXTURE_COMPARE_FUNC, COMPARE_MODE = GL_TEXTURE_COMPARE_MODE, LOD_BIAS = GL_TEXTURE_LOD_BIAS, 
	MIN_FILTER = GL_TEXTURE_MIN_FILTER, MAG_FILTER = GL_TEXTURE_MAG_FILTER, MIN_LOD = GL_TEXTURE_MIN_LOD, MAX_LOD = GL_TEXTURE_MAX_LOD, MAX_LEVEL = GL_TEXTURE_MAX_LEVEL, 
	SWIZZLE_R = GL_TEXTURE_SWIZZLE_R, SWIZZLE_G = GL_TEXTURE_SWIZZLE_G, SWIZZLE_B = GL_TEXTURE_SWIZZLE_B, SWIZZLE_A = GL_TEXTURE_SWIZZLE_A, WRAP_S = GL_TEXTURE_WRAP_S, WRAP_T = GL_TEXTURE_WRAP_T, WRAP_R = GL_TEXTURE_WRAP_R
};

//Set parameter for texture type
void TexParameter(TextureType type, TextureParameterType param_type, int param)
{
	glTexParameteri(type, param_type, param);
}

uint inline CreateShader(ShaderType type)
{
	return glCreateShader(type);
}

int inline GetShaderInfo(uint shader_id, ShaderStatus type_of_info)
{
	int info{};
	glGetShaderiv(shader_id, type_of_info, &info);
	return info;
}

void inline BindBuffer(BufferType type, uint id)
{
	glBindBuffer(type, id);
}

void ClearBuffer(int type)
{
	glClear(type);
}

class VertexArrayObject
{
	uint id{};
	uint id_vertex_buffer_object{};
	uint vertex_count{};

public:
	void Create(String data, BufferDataType type)
	{
		vertex_count = (data.lenght / 4) / 5;

		glGenVertexArrays(1, &id);
		glBindVertexArray(id);
		glGenBuffers(1, &id_vertex_buffer_object);
		BindBuffer(VERTEX_BUFFER, id_vertex_buffer_object);
		glBufferData(VERTEX_BUFFER, data.lenght, data.Link(), type);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 20, 0);			//Stride should represent whole size of vertex unit0.
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 20, (void*)12);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	void Use()
	{
		glBindVertexArray(id);
	}

	uint Get()
	{
		return id;
	}

	uint VertexCount()
	{
		return vertex_count;
	}
};

class Texture
{
	uint id{};
	int2 size{};
	int colorChannelCount{};

public:

	void Load(const char* path)
	{
		unsigned char* data = stbi_load(path, &size.x, &size.y, &colorChannelCount, 0);
		glGenTextures(1, &id);
		glBindTexture(TEXTURE2D, id);
		glTexImage2D(TEXTURE2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(TEXTURE2D);

		TexParameter(TEXTURE2D, WRAP_S, GL_MIRRORED_REPEAT);
		TexParameter(TEXTURE2D, WRAP_T, GL_MIRRORED_REPEAT);
		TexParameter(TEXTURE2D, MIN_FILTER, GL_NEAREST);
		TexParameter(TEXTURE2D, MAG_FILTER, GL_LINEAR);

		glBindTexture(TEXTURE2D, 0);
		stbi_image_free(data);
	}

	void Bind()
	{
		glBindTexture(TEXTURE2D, id);
	}

};

class Shader
{
	uint id{};
	ShaderType type{};
	String source;
	String sourcePath;
	String log{};
public:

	char Create(char* path)
	{
		if (!path) return -2;
		
		sourcePath.Set(path);
		File file;
		file.Read(path);
		source.Set(file.Get(), file.Lenght());
		char* d = source;

		switch (source[2])
		{
		case 'v':
			type = VERTEX;
			break;
		case 'p':
			type = PIXEL;
			break;
		}


		id = CreateShader(type);
		glShaderSource(id, 1, &d, &source.lenght);
		glCompileShader(id);


		if (!(GetShaderInfo(id, IS_COMPILED)))
		{
			Report("Shader was not compiled. Path of source: ");
			Report(file.Path());
			Report('\n');

			log.Set(2048);
			glGetShaderInfoLog(id, 2048, &log.lenght, log.Link());
			log.Shrink();
			Report(log.Link());
			glDeleteShader(id);
			return -1;
		}
		return 1;
	}

	uint Get()
	{
		return id;
	}

	~Shader()
	{
		log.Delete();
		glDeleteShader(id);
	}
};

class ShaderProgram
{
	uint id{};
	Storage<Shader>shader{};
public:

	void Init(String shader_paths)
	{
		if (shader_paths.lenght < 1)
		{
			Report("Shader path is empty.\n");
		}

		uint shaderAmount = CountValue(shader_paths, ';', 0);
		shader.Resize(shaderAmount);

		uint* lenghtOfPath = new uint[shaderAmount]{};
		lenghtOfPath[0] = FindValue(shader_paths, ';', 0);
		shader.Set();
		shader[0]->Create(shader_paths);
		for (uint iterator{ 1 }, position = lenghtOfPath[0]; iterator < shaderAmount; iterator++)
		{
			lenghtOfPath[iterator] = FindValue(shader_paths + position + 1, ';', 0);
			shader.Set();
			if (shader[iterator]->Create(shader_paths + position + 1) == -1) return;
			position += lenghtOfPath[iterator];
		}

		id = glCreateProgram();

		for (uint iterator{}; iterator < shaderAmount; iterator++)
		{
			glAttachShader(id, shader[iterator]->Get());
		}

		glLinkProgram(id);
	}

	void Use()
	{
		glUseProgram(id);
	}

	int GetUniformLocation(char* name)
	{
		return glGetUniformLocation(id, name);
	}

	void Set(uint variable_id, float value)
	{
		glUniform1f(variable_id, value);
	}

	void Set(uint variable_id,  glm::vec3 value)
	{
		glUniform3f(variable_id, value.x, value.y, value.z);
	}

	void Set(uint variable_id, glm::mat4 matrix)
	{
		glUniformMatrix4fv(variable_id, 1, false, glm::value_ptr(matrix));
	}

};

class Graphics
{
	VertexArrayObject* current{};
	Storage<VertexArrayObject> models;
	Storage<Texture> textures;
public:
	ShaderProgram shaderProgram;
	Storage<uint> uniformParams;

	void Init(String shader_paths)
	{
		uniformParams.Resize(10);
		models.Resize(100);
		textures.Resize(100);
		shaderProgram.Init(shader_paths);
		shaderProgram.Use();

		uniformParams.Set(shaderProgram.GetUniformLocation(String("world")));
		uniformParams.Set(shaderProgram.GetUniformLocation(String("view")));
		uniformParams.Set(shaderProgram.GetUniformLocation(String("projection")));
	}

	uint CreateModel(String data)
	{
		VertexArrayObject* buffer = new VertexArrayObject;
		buffer->Create(data, STATIC);
		return models.Set(buffer);
	}

	uint CreateTexture(const char* path)
	{
		Texture* texture = new Texture;
		texture->Load(path);

		return textures.Set(texture);
	}

	uint SetModel(VertexArrayObject* vao)
	{
		return models.Set(vao);
	}

	void Draw(uint model_id, mat4 world, mat4 view, mat4 projection)
	{
		current = models.Get(model_id);
		shaderProgram.Set(*uniformParams[0], world);
		shaderProgram.Set(*uniformParams[1], view);
		shaderProgram.Set(*uniformParams[2], projection);

		glDrawArrays(GL_TRIANGLES, 0, current->VertexCount());
	}

	void DrawQueue(Storage<Logic::Object>* queue, mat4 view, mat4 projection)
	{
		Logic::Object* current{};
		uint size = queue->GetSize();
		for (uint i{}; i < size; i++)
		{
			current = queue->Get(i);
			if (current->toRender == false) continue;
			textures.Get(current->texture)->Bind();
			Draw(current->model, current->World(), view, projection);
		}
	}

	void DrawQueue(Storage<UI::Object>* queue, uint default_model, mat4 view, mat4 projection)
	{
		UI::Object* current{};
		uint size = queue->GetSize();
		for (uint i{}; i < size; i++)
		{
			current = queue->Get(i);
			if (current->toRender == false)
				continue;
			textures.Get(current->texture)->Bind();
			Draw(default_model, current->GetWorld(), view, projection);
		}
	}
};