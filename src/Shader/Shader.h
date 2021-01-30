#pragma once
#include <glm/glm.hpp>

class string;
#define string std::string

struct ShaderProgramSource
{
	string VertexSource;
	string FragmentSource;
};


class Shader
{

public:
	Shader(const string& filepath);
	unsigned int ID;
	unsigned int VS;
	unsigned int FS;

	void Use();
	void SetMat4(const string& uniformName, bool transpose, glm::mat4 value);

private:	
	ShaderProgramSource source;
	ShaderProgramSource ParseShader(const string& filepath);
	//creates the individual vertex and fragment shaders
	unsigned int CreateShader(unsigned int type, const string& source);
	//creates the program which contains the shaders
	void CompileShader();




};