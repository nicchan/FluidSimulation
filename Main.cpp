#include<iostream>
#include<Windows.h>
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
				-1.0f, -1.0f, 0.0f,				1.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// Lower left corner 0
				 1.0f, -1.0f, 0.0f,				1.0f, 1.0f, 1.0f,		1.0f, 0.0f,	// Lower right corner 1
				-1.0f,  1.0f, 0.0f,				0.0f, 1.0f, 0.0f,		0.0f, 1.0f,	// Upper left corner 2
				 1.0f,	1.0f, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f	// Upper right corner 3 
};


// Canvas placement coordinates at final render
GLfloat placementVertices[] =
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

// Canvas indices for vertices order 
GLuint canvasIndices[] =
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

// Helper function to swap IDs
void swapTextureIDs(GLuint& idA, GLuint& idB)
{
	GLuint temp = idA;
	idA = idB;
	idB = temp;
}


void bindFramebufferAndSetViewport(GLuint fb, unsigned int width, unsigned int height) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, width, height);
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

	// Wait so that I have time to start recording 
	double prevT = glfwGetTime();
	double crntT = glfwGetTime();
	while (crntT - prevT < 3)
	{
		crntT = glfwGetTime();
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures GL
	gladLoadGL();

	// Specify the viewpoint of OpenGL in the Window
	// In this case the viewpoint goes from x = 0, y = 0, to x = 1000, y = 1000
	unsigned int canvasSize = 1000;
	glViewport(0, 0, canvasSize, canvasSize);

	// Generates Shader object using shaders default.vert and default.frag
	// TODO: more shadersPrograms...
	// Testing RTT...
	Shader shaderProgram("default.vert", "TestRTT.frag");
	Shader shaderProgramB("default.vert", "default.frag");

	/* Set shader program scale */
	// Assigns scale value for shader program
	shaderProgram.setScale(0.5f);
	// Assigns scale value for shader program B
	shaderProgramB.setScale(0.5f);

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

	// Generates Vertex Array Object and binds it
	VAO canvasVAO;
	canvasVAO.Bind();
	// Generates Vertex Buffer Object for the Canvas (ie. window)
	VBO canvasVBO(placementVertices, sizeof(placementVertices));
	// Generates Element Buffer Object and links it to indices
	EBO canvasEBO(canvasIndices, sizeof(canvasIndices));

	// Links VBO attributes such as coordinates and colors to VAO
	canvasVAO.LinkAttrib(canvasVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	canvasVAO.LinkAttrib(canvasVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	canvasVAO.LinkAttrib(canvasVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	canvasVAO.Unbind();
	canvasVBO.Unbind();
	canvasEBO.Unbind();

	/* Textures */
	unsigned int textureSize = 800;

	
	Texture dataTexture(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	dataTexture.texUnit(shaderProgram, "tex0", 0);

	// Testing additional Texture for RTT
	Texture dataTextureB(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	dataTextureB.texUnit(shaderProgramB, "tex0", 0);
	
	/* FrameBufferObject (test for RTT) */

	// Create and bind the FrameBuffer
	unsigned int fb;
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);

	// Attach the dataTextureB as the first color attachment of fb
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataTextureB.ID, 0);

	// Ensure FrameBuffer is complete, otherwise clean up and exit
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is incomplete." << std::endl;
		VAO1.Delete();
		canvasVAO.Delete();
		VBO1.Delete();
		canvasVBO.Delete();
		EBO1.Delete();
		dataTexture.Delete();
		dataTextureB.Delete();
		shaderProgram.Delete();
		shaderProgramB.Delete();
		return -1;
	}
	// unbind fb
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Testing update with texture byte data with glTexSubImage2D
	unsigned char* newByteArray = initByteArray(dataTexture.width * dataTexture.height * 4, 255);
	dataTexture.BulkImageRefresh(newByteArray);
	delete[] newByteArray;



	// Set up time step and timers
	double prevTime = glfwGetTime();
	double crntTime = glfwGetTime();
	double timeStep = 0.01;
	unsigned int countOT = 0;	// counter to track OTs

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		/* Simple Timer */
		crntTime = glfwGetTime();
		if (crntTime - prevTime < timeStep)
		{
			continue;
		}
		else
		{
			prevTime = crntTime;
		}

		/* First Draw */
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Bind the VBO so OpenGL knows to use it
		VBO1.Bind();
		EBO1.Bind();
		// Binds fb for first Draw
						//bindFramebufferAndSetViewport(fb, textureSize, textureSize);
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign new color (the glClearColor) to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Binds texture so that it appears in render
		dataTexture.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Second Draw*/
		// Bind the VAO so OpenGL knows to use it
		canvasVAO.Bind();
		// Bind the VBO so OpenGL knows to use it
		canvasVBO.Bind();
		canvasEBO.Bind();
		// Binds default FrameBuffer
					//bindFramebufferAndSetViewport(0, canvasSize, canvasSize);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign new color (the glClearColor) to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgramB.Activate();
		// Binds texture so that it appears in render
		dataTextureB.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Show stuff on screen; Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Swap TexA and TexB
		swapTextureIDs(dataTexture.ID, dataTextureB.ID);
		// Re-attach textureB to fb
		// This is needed otherwise only "two states" will be shown
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataTextureB.ID, 0);
		// Take care of all GLFW events
		glfwPollEvents();
		
		// track for real-time related performance 
		crntTime = glfwGetTime();
		if (crntTime - prevTime > timeStep)
		{
			countOT++;
		}
	}

	// Delete all the objects we've created
	VAO1.Delete();
	canvasVAO.Delete();
	VBO1.Delete();
	canvasVBO.Delete();
	EBO1.Delete();
	dataTexture.Delete();
	dataTextureB.Delete();
	shaderProgram.Delete();
	shaderProgramB.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	std::cout << "OT loop counts " << countOT << std::endl;
	return 0;
}




