#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H


#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
	public:
		GLuint ID;
		GLenum type;
		unsigned int width;
		unsigned int height;
		Texture(int widthImg, int heightImg, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
		
		// TODO: figure out C++ instantiation overloading stuff and clean this up
		/*
		Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType); 
		*/

		// Assigns a texture unit to a texture
		void texUnit(Shader shader, const char* uniform, GLuint unit);
		// Binds a texture
		void Bind();
		// Unbinds a texture
		void Unbind();
		// Deletes a texture
		void Delete();
		// Refresh (replace) entire image with newByteArray
		void BulkImageRefresh(unsigned char* newByteArray);
};

#endif