#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"


// Vertices coordinates
GLfloat vertices[] =
{	//				COORDINATES				|		COLORS			|	TexCoord	//
				-0.5f, -0.5f, 0.0f,				1.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// Lower left corner 0
				 0.5f, -0.5f, 0.0f,				1.0f, 1.0f, 1.0f,		1.0f, 0.0f,	// Lower right corner 1
				-0.5f,  0.5f, 0.0f,				0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// Upper left corner 2
				 0.5f,	0.5f, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f	// Upper right corner 3 
};

// Indices for vertices order 
GLuint indices[] =
{
	0, 2, 3, // Upper triangle
	0, 1, 3, // Lower triangle
};


// Helper function to initialize a new byte array
unsigned char* initByteArray(int byteLength, int initValue)
{
	if (initValue < 0)
	{
		return NULL;
	}
	if (initValue > 255)
	{
		return NULL;
	}
	unsigned char* ptr = new unsigned char[byteLength]();
	if (initValue > 0)
	{
		for (int index = 0; index < byteLength; index++)
		{
			ptr[index] = (char)initValue;
		}
	}
	return ptr;
}


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core contains only modern fcn packages

	// Create a GLFWwindow object of 1000 by 1000 pixels, naming it "FluidSimulation"
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "FluidSimulation", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures GL
	gladLoadGL();

	// Specify the viewpoint of OpenGL in the Window
	// In this case the viewpoint goes from x = 0, y = 0, to x = 1000, y = 1000
	glViewport(0, 0, 1000, 1000);

	// Generates Shader object using shaders default.vert and default.frag
	// TODO: more shadersPrograms...
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Textures
	
	// Texture dataTexture("pop_cat.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	Texture dataTexture(800, 800, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	dataTexture.texUnit(shaderProgram, "tex0", 0);


	// Testing update with texture byte data with glTexSubImage2D
	
	unsigned char* newByteArray = initByteArray(dataTexture.width * dataTexture.height * 4, 255);
	dataTexture.BulkImageRefresh(newByteArray);
	delete[] newByteArray;


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign new color (the glClearColor) to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		// Binds texture so that it appears in render
		dataTexture.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	dataTexture.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}




