#pragma once

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

class Util
{
public:
	static string readFile(string file) {
		string fileString;
		ifstream fStream(file.c_str());
		fileString.assign((std::istreambuf_iterator<char>(fStream)), std::istreambuf_iterator<char>());
		return fileString;
	}

	static void printShaderLog(GLuint shader)
	{
		int infoLogLength = 0;
		int maxLength = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

		if (infoLogLength > 0)
		{
			printf("%s\n", infoLog);
		}
		delete[] infoLog;
	}

};