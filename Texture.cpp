#include"Texture.h"


Texture::Texture(int widthImg, int heightImg, GLenum texTarget, GLenum texSlot, GLenum format, GLenum sizedInternalFormat, GLenum pixelType)
{


	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit (eg. GL_TEXTURE0...)
	glActiveTexture(texSlot);
	// Binds texture to a target (eg. GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_CUBE_MAP...)
	glBindTexture(texTarget, ID);

	// Configures the target of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Extra lines in case you chose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);


	// Assigns the target of the texture to the texture object
	slot = texSlot;
	target = texTarget;
	pixelDataType = pixelType;
	pixelDataFormat = format;
	width = widthImg;
	height = heightImg;
	// set number of color channel to 4
	int numColCh = 4;
	int dataLength = widthImg * heightImg * numColCh;

	/*
	// initialize texture data to zeroes
	//unsigned char* bytes = new unsigned char[dataLength]();
	*/

	/*
	// test 5x5 
	unsigned char temp[] =
	{
			255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
			255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
			255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
			255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
			255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255
	};
	unsigned char* bytes = new unsigned char[dataLength]();
	for (int ii = 0; ii < dataLength; ii++) 
	{
		bytes[ii] = temp[ii];
	}
	*/

	unsigned char* bytes = NULL;

	// Assigns the image to the OpenGL Texture objectGLenum target,
	// void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* data);
	glTexImage2D(texTarget, 0, sizedInternalFormat, widthImg, heightImg, 0, format, pixelType, bytes);

	// TODO: check if delete operation is correct
	delete[] bytes;

	// Unbinds the OpenGL Texture Object so that it can't accidentally be modified
	glBindTexture(texTarget, 0);
}

/*
Texture::Texture(const char* image, GLenum texTarget, GLenum slot, GLenum format, GLenum pixelType)
{
	// Assigns the target of the texture to the texture object
	target = texTarget;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up (stb and OpenGL do not have the same read ordering)
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes 
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texTarget, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you chose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	glTexImage2D(texTarget, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generates MipMaps
	glGenerateMipmap(texTarget);

	// Deletes the image data as it is already in the OpenGL Texture Object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture Object so that it can't accidentally be modified
	glBindTexture(texTarget, 0);
}
*/




void Texture::texUnit(Shader &shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	if (tex0Uni == -1)
	{
		std::cout << uniform << " does not correspond to an active uniform variable in Shader Program " << shader.ID << std::endl;
		return;
	}
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(tex0Uni, unit);
}

void Texture::Bind()
{
	glActiveTexture(slot);
	glBindTexture(target, ID);
}

void Texture::Unbind()
{
	glActiveTexture(slot);
	glBindTexture(target, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}

// Refresh (replace) entire image with newByteArray
void Texture::BulkImageReplace(void* newByteArray)
{	// TODO: clean up
	glActiveTexture(slot);
	glBindTexture(target, ID);
	glTexSubImage2D(target, 0, 0, 0, width, height, pixelDataFormat, pixelDataType, newByteArray);
	glBindTexture(target, 0);
}
