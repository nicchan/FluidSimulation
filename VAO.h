#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
	public:
		// Reference ID of the Vertex Array Object
		GLuint ID;
		// Constructor that generates a Vertex Array Object
		VAO();

		// Links a VBO Attribute to the VAO using a certain layout
		void LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		// Binds the VAO
		void Bind();
		// Unbinds the VAO
		void Unbind();
		// Deletes the VAO
		void Delete();
};

#endif