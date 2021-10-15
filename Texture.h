#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H


#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
	public:
		GLuint ID;
		GLenum target;
		GLenum slot;
		GLenum pixelDataType;
		unsigned int width;
		unsigned int height;
		Texture(int widthImg, int heightImg, GLenum texTarget, GLenum slot, GLenum format, GLenum sizedInternalFormat, GLenum pixelType);
		
		// TODO: figure out C++ instantiation overloading stuff and clean this up
		/*
		Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType); 
		// Example: Texture dataTexture("pop_cat.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
		*/

		// Assigns a texture unit to a texture
		void texUnit(Shader &shader, const char* uniform, GLuint unit);
		// Binds a texture
		void Bind();
		// Unbinds a texture
		void Unbind();
		// Deletes a texture
		void Delete();
		// Refresh (replace) entire image with newByteArray
		void BulkImageReplace(void* newByteArray);
};

#endif