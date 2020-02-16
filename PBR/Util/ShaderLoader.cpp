#include "ShaderLoader.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

GLuint ShaderLoader::Load(const std::string& vertexFilePath, const std::string& fragmentFilePath)
{
	// 쉐이더들 생성
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// 버텍스 쉐이더를 컴파일
	std::cout << "Compiling shader : " << vertexFilePath << std::endl;
	std::string vertexSourceCode = GetShaderCode(vertexFilePath);
	const char* vertexSourcePointer = vertexSourceCode.c_str();
	glShaderSource(VertexShaderID, 1, &vertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// 버텍스 쉐이더를 검사
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}



	// 프래그먼트 쉐이더를 컴파일
	std::cout << "Compiling shader : " << fragmentFilePath << std::endl;
	std::string fragmentSourceCode = GetShaderCode(fragmentFilePath).c_str();
	const char* fragmentSourcePointer = fragmentSourceCode.c_str();
	glShaderSource(FragmentShaderID, 1, &fragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// 프래그먼트 쉐이더를 검사
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}



	// 프로그램에 링크
	std::cout << "Linking program" << std::endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// 프로그램 검사
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

std::string ShaderLoader::GetShaderCode(const std::string& path, const std::string& includeIdentifier /*= "#include "*/)
{
	static bool isRecursiveCall = false;

	std::string fullSourceCode = "";
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
		return fullSourceCode;
	}

	std::string lineBuffer;
	while (std::getline(file, lineBuffer))
	{
		// Look for the new shader include identifier
		if (lineBuffer.find(includeIdentifier) != lineBuffer.npos)
		{
			// Remove the include identifier, this will cause the path to remain
			lineBuffer.erase(0, includeIdentifier.size());

			// The include path is relative to the current shader file path
			std::string pathOfThisFile;
			getFilePath(path, pathOfThisFile);
			lineBuffer.insert(0, pathOfThisFile);

			// By using recursion, the new include file can be extracted
			// and inserted at this location in the shader source code
			isRecursiveCall = true;
			fullSourceCode += GetShaderCode(lineBuffer, includeIdentifier);

			// Do not add this line to the shader source code, as the include
			// path would generate a compilation issue in the final source code
			continue;
		}

		fullSourceCode += lineBuffer + '\n';
	}

	// Only add the null terminator at the end of the complete file,
	// essentially skipping recursive function calls this way
	if (!isRecursiveCall)
		fullSourceCode += '\0';

	file.close();

	return fullSourceCode;
}

void ShaderLoader::getFilePath(const std::string& filePath, std::string& pathWithoutFileName)
{
	// Remove the file name and store the path to this folder
	size_t found = filePath.find_last_of("/\\");
	pathWithoutFileName = filePath.substr(0, found + 1);
}