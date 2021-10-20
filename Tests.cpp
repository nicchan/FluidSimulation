#include<iostream>
#include<Windows.h>
// TODO: #include<assert.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Fluid.h"


// Specify the pixel sizing of the window to be created by glfwCreateWindow
// TODO: currently only doing square; should make it rectangle with 2 components values being specified 
unsigned int windowSize = 1000;

// Set texture to have same resolution as the window
//unsigned int textureSize = windowSize;
unsigned int textureSize = 5;	// test 5x5


// Vertices coordinates
GLfloat vertices[] =
{	//		COORDINATES				|		COLORS			|	normTexCoord	    //
		-1.0f, -1.0f, 0.0f,				1.0f, 0.0f, 0.0f,		0.0f, 0.0f,			// Lower left corner 0
		 1.0f, -1.0f, 0.0f,				1.0f, 1.0f, 1.0f,		1.0f, 0.0f,			// Lower right corner 1
		-1.0f,  1.0f, 0.0f,				0.0f, 1.0f, 0.0f,		0.0f, 1.0f,			// Upper left corner 2
		 1.0f,	1.0f, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f			// Upper right corner 3 
};


// Canvas placement coordinates at final render
GLfloat placementVertices[] =
{	//		COORDINATES				|		COLORS			|	normTexCoord	    //
		-0.5f, -0.5f, 0.0f,				1.0f, 0.0f, 0.0f,		0.0f, 0.0f,			// Lower left corner 0
		 0.5f, -0.5f, 0.0f,				1.0f, 1.0f, 1.0f,		1.0f, 0.0f,			// Lower right corner 1
		-0.5f,  0.5f, 0.0f,				0.0f, 1.0f, 0.0f,		0.0f, 1.0f,			// Upper left corner 2
		 0.5f,	0.5f, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f			// Upper right corner 3 
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


// Not sure if this is needed...
void bindFramebufferAndSetViewport(GLuint fb, unsigned int width, unsigned int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, width, height);
}

int testRun()
{

	// check assumption for data sizing
	if (sizeof(int) != sizeof(float))
	{
		std::cout << "sizeof(int) " << sizeof(int) << " does not equal " << "sizeof(float) " << sizeof(float) << std::endl;
		return -1;
	}
	if (sizeof(unsigned int) != sizeof(float))
	{
		std::cout << "sizeof(unsigned int) " << sizeof(unsigned int) << " does not equal " << "sizeof(float) " << sizeof(float) << std::endl;
		return -1;
	}

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core contains only modern fcn packages

	// Create a GLFWwindow object of x by y pixels, naming it "FluidSimulation"
	GLFWwindow* window = glfwCreateWindow(windowSize, windowSize, "FluidSimulation", NULL, NULL);
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
	// Tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	// In this case the viewpoint goes from x = 0, y = 0, to x = 1000, y = 1000
	// The first two parameters of glViewport set the location of the lower left corner of the window. 
	// The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
	glViewport(0, 0, windowSize, windowSize);

	// Generates Shader object using shaders default.vert and default.frag
	// TODO: more shadersPrograms...
	// Testing RTT...
	Shader shaderProgram("default.vert", "TestRTT.frag");
	Shader shaderProgramB("default.vert", "default.frag");

	/* Set shader program scale */
	// Assigns scale value for shader program
	shaderProgram.setUniform1f(0.0f, "vertScale");
	// Assigns scale value for shader program B
	shaderProgramB.setUniform1f(0.5f, "vertScale");

	// setting vec4 uniform
	shaderProgram.setUniform4f(1.0f, 1.0f, 1.0f, 1.0f, "myvec");
	shaderProgram.setUniform4f(0.5f, 0.5f, 0.5f, 0.5f, "myvec2");

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

	//Texture dataTexture(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture dataTexture(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	dataTexture.texUnit(shaderProgram, "tex0", 0);

	// Testing Texture in another tex unit 
	Texture dataTexture1(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	dataTexture1.texUnit(shaderProgram, "tex1", 1);

	// Testing additional Texture for RTT
	Texture dataTextureB(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	//Texture dataTextureB(textureSize, textureSize, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	dataTextureB.texUnit(shaderProgramB, "tex0", 0);


	//Trying to understand texture unit slots...
	std::cout << "\n" << "GL_TEXTURE0: " << GL_TEXTURE0 << "\n" << std::endl;
	std::cout << "\n" << "GL_TEXTURE1: " << GL_TEXTURE1 << "\n" << std::endl;
	std::cout << "\n" << "GL_TEXTURE2: " << GL_TEXTURE2 << "\n" << std::endl;
	std::cout << "\n" << "GL_TEXTURE3: " << GL_TEXTURE3 << "\n" << std::endl;


	// Testing update with texture byte data with glTexSubImage2D
	/*
	unsigned char* newByteArray = initByteArray(dataTexture.width * dataTexture.height * 4, 255);
	dataTexture.BulkImageReplace(newByteArray);
	delete[] newByteArray;
	*/
	// testing with 5x5
	//unsigned char temp[] =
	//{
	//	// row index 0
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 1
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 2
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 3
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 4
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255
	//};
	float temp[] =
	{
		// row index 0
		255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
		// row index 1
		255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
		// row index 2
		255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
		// row index 3
		255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
		// row index 4
		255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255
	};
	dataTexture.BulkImageReplace(temp);


	//float temp1[] =
	//{
	//	// row index 0
	//	-1, 0, 0, -1.0,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 1
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 2
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 3
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255,
	//	// row index 4
	//	255, 0, 0, 255,		0, 255, 0, 255,	   0, 0, 255, 255,	   0, 0, 0, 255,	   255, 255, 255, 255
	//};
	float* temp1 = createBoundaryOffsetData(textureSize, textureSize, 4);
	dataTexture1.BulkImageReplace(temp1);
	delete[] temp1;



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
		bindFramebufferAndSetViewport(fb, textureSize, textureSize);
		//glBindFramebuffer(GL_FRAMEBUFFER, fb);
		//glViewport(1, 1, textureSize, textureSize);
		// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.12f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign new color (the glClearColor) to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Binds texture so that it appears in render
		dataTexture.Bind();
		dataTexture1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/* Second Draw*/
		// Bind the VAO so OpenGL knows to use it
		canvasVAO.Bind();
		// Bind the VBO so OpenGL knows to use it
		canvasVBO.Bind();
		canvasEBO.Bind();
		// Binds default FrameBuffer
		bindFramebufferAndSetViewport(0, windowSize, windowSize);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		//std::cout << "dataTexture.ID: " << dataTexture.ID << std::endl;
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




