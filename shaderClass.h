#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename);

class Shader
{
	public:
		// Reference ID of the Shader Program
		GLuint ID;
		// Constructor that builds the Shader Program from 2 different shaders
		Shader(const char* vertexFile, const char* fragmentFile);

		// Activates the Shader Program
		void Activate();
		// Deletes the Shader Program
		void Delete();
	private:
		// checks if the different shaders have compiled properly
		void compileErrors(unsigned int shader, const char* type);

};

#endif