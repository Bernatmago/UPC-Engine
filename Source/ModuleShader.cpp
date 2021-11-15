#include "ModuleShader.h"

#include "glew.h"

#include "MathGeoLib.h"

ModuleShader::ModuleShader()
{

}

bool ModuleShader::Init()
{
	static const bool transpose = GL_TRUE;
	vertex_shader_id = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("vertex_mvp.glsl"));
	//vertex_shader_id = CompileShader(GL_VERTEX_SHADER, LoadShaderSource("vertex_simple.glsl"));
	fragment_shader_id = CompileShader(GL_FRAGMENT_SHADER, LoadShaderSource("fragment.glsl"));
	shader_id = CreateProgram(vertex_shader_id, fragment_shader_id);
	
	glUseProgram(shader_id);
	//glUniformMatrix4fv(glGetUniformLocation(shader_id, "model"), 1, transpose, &model[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shader_id, "view"), 1, transpose, &view[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shader_id, "proj"), 1, transpose, &proj[0][0]);


	return true;
}

char* ModuleShader::LoadShaderSource(const char* shader_file_name)
{
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, shader_file_name, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		data = (char*)malloc(size + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);
	}
	return data;
}

unsigned int ModuleShader::CompileShader(unsigned type, const char* source)
{
	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;
}

unsigned ModuleShader::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program_id, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;
}