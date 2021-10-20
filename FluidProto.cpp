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



// S(u) = P * F * D * A (u)

void advectVelocity() 
{
	// bind/activate advect fragment program 
	// bind frame buffer
	// attach output texture to frame buffer
	// bind sampler texture
	// draw 
}

void viscousDiffuse()
{
	// TODO
}


void addForces()
{
	// TODO
}

void projectVelocity()
{

	// compute divergence of velocity
		// bind/activate divergence Fragment Program
		// bind frame buffer
		// attach output texture to frame buffer
		// bind sampler texture
		// draw to output texture

	// solve poisson-pressure equation for pressure
		// bind/activate Jacobi Iteration Fragment Program (uses two texture units)
		// iterate 40-80 times
			// bind frame buffer
			// attach output texture to frame buffer
			// bind sampler texture
			// draw to output texture
			// swap texture IDs
			// re-attach output texture to frame buffer

	// subtract gradient of p from u
		// bind/activate Gradient subtract Fragment Program
		// bind frame buffer
		// attach output texture to frame buffer
		// bind sampler texture
		// draw to output texture
}

void advectDensity()
{
	// bind/activate advect fragment program 
	// bind frame buffer
	// attach output texture to frame buffer
	// bind sampler texture
	// draw to output texture
}

// Helper function to swap IDs
void swapTextureIDs(GLuint& idA, GLuint& idB)
{
	GLuint temp = idA;
	idA = idB;
	idB = temp;
}


// 2D coord -> 1D index conversion
// assumes inputs have positive values; x < width;
int IX(int x, int y, int width, int numColorChannel)
{
	return (x + (y * width)) * numColorChannel;
}


float* createBoundaryOffsetData(int fluidTexWidth, int fluidTexHeight, int numColorChannel)
{
	const int dataLength = fluidTexWidth * fluidTexHeight * numColorChannel;

	float* imageData = new float[dataLength]();

	// Set Values
	// 
	// Bottom Border (*, 0) where * is between 1 to texWidth-2 inclusive
	for (int texii = 1; texii < fluidTexWidth - 1; texii++)
	{
		int dataPos = IX(texii, 0, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = 0;
		// y or g color component
		imageData[dataPos + 1] = 1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// Top Border (*, TH-1) where * is between 1 to texWidth-2 inclusive
	for (int texii = 1; texii < fluidTexWidth - 1; texii++)
	{
		int dataPos = IX(texii, fluidTexHeight - 1, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = 0;
		// y or g color component
		imageData[dataPos + 1] = -1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// Left Border (0, *) where * is between 1 to texHeight-2 inclusive
	for (int texjj = 1; texjj < fluidTexHeight - 1; texjj++)
	{
		int dataPos = IX(0, texjj, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = 1;
		// y or g color component
		imageData[dataPos + 1] = 0;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// Right Border (TW-1, *) where * is between 1 to texHeight-2 inclusive
	for (int texjj = 1; texjj < fluidTexHeight - 1; texjj++)
	{
		int dataPos = IX(fluidTexWidth - 1, texjj, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = -1;
		// y or g color component
		imageData[dataPos + 1] = 0;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// BL corner
	{
		int dataPos = IX(0, 0, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = 1;
		// y or g color component
		imageData[dataPos + 1] = 1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// BR corner
	{
		int dataPos = IX(fluidTexWidth - 1, 0, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = -1;
		// y or g color component
		imageData[dataPos + 1] = 1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// TL corner
	{
		int dataPos = IX(0, fluidTexHeight - 1, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = 1;
		// y or g color component
		imageData[dataPos + 1] = -1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}
	// TR corner
	{
		int dataPos = IX(fluidTexWidth - 1, fluidTexHeight - 1, fluidTexWidth, numColorChannel);
		// x or r color component
		imageData[dataPos] = -1;
		// y or g color component
		imageData[dataPos + 1] = -1;
		// z or b color component
		imageData[dataPos + 2] = 0;
		// w or a color component
		imageData[dataPos + 3] = 1;
	}

	return imageData;
}



int textureWidth = 1500;
int textureHeight = 1500;
int windowWidth = 1000;
int windowHeight = 1000;
float timestep = 0.05;
bool displayBoundary = false;


int fluidProto()
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
	
	// Create a GLFWwindow object of x by y pixels, naming it "FluidSimulation"
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "FluidSimulation", NULL, NULL);

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
	// Tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	// In this case the viewpoint goes from x = 0, y = 0, to x = 1000, y = 1000
	// The first two parameters of glViewport set the location of the lower left corner of the window. 
	// The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
	glViewport(0, 0, windowWidth, windowHeight);


	// VBO/VAO/EBO/FBO stuff
	GLfloat vertices[] =
	{	//		COORDINATES				|		COLORS			|	normTexCoord	    //
			-1.0f, -1.0f, 0.0f,				1.0f, 0.0f, 0.0f,		0.0f, 0.0f,			// Lower left corner 0
			 1.0f, -1.0f, 0.0f,				1.0f, 1.0f, 1.0f,		1.0f, 0.0f,			// Lower right corner 1
			-1.0f,  1.0f, 0.0f,				0.0f, 1.0f, 0.0f,		0.0f, 1.0f,			// Upper left corner 2
			 1.0f,	1.0f, 0.0f,				0.0f, 0.0f, 1.0f,		1.0f, 1.0f			// Upper right corner 3 
	};
	// Indices for vertices order 
	GLuint indices[] =
	{
		0, 2, 3, // Upper triangle
		0, 1, 3, // Lower triangle
	};
	// Generates Vertex Array Object and binds it
	VAO fluidVAO;
	fluidVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO fluidVBO(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO fluidVEO(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	fluidVAO.LinkAttrib(fluidVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	fluidVAO.LinkAttrib(fluidVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	fluidVAO.LinkAttrib(fluidVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	fluidVAO.Unbind();
	fluidVBO.Unbind();
	fluidVEO.Unbind();
	// Generates Vertex Array Object and binds it
	VAO canvasVAO;
	canvasVAO.Bind();
	// Generates Vertex Buffer Object for the Canvas (ie. window)
	VBO canvasVBO(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO canvasEBO(indices, sizeof(indices));
	// Links VBO attributes such as coordinates and colors to VAO
	canvasVAO.LinkAttrib(canvasVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	canvasVAO.LinkAttrib(canvasVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	canvasVAO.LinkAttrib(canvasVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	canvasVAO.Unbind();
	canvasVBO.Unbind();
	canvasEBO.Unbind();

	// Create and bind the FrameBuffer
	unsigned int fb;
	glGenFramebuffers(1, &fb);

	// Create textures
	std::cout << "\n" << "Creating Textures" << "\n" << std::endl;
	Texture velocityTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	Texture pressureTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	Texture densityTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	Texture divergenceVTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE3, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	Texture boundaryInfoTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE4, GL_RGBA, GL_RGBA32F, GL_FLOAT);
	float* data = createBoundaryOffsetData(textureWidth, textureHeight, 4);
	boundaryInfoTexture.BulkImageReplace(data);
	if (data[0] != 1)
	{
		std::cout << "\n" << "ERROR in data of boundaryInfoTexture" << "\n" << std::endl;
	}
	delete[] data;
	Texture tempTexture(textureWidth, textureHeight, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA32F, GL_FLOAT);

	// Create shader programs
	std::cout << "\n" << "Creating Shader Programs" << "\n" << std::endl;
	Shader advectVelocityShader("default.vert", "advect.frag");
	Shader advectDensityShader("default.vert", "advect.frag");
	Shader displayBoundaryShader("default.vert", "displayVector.frag");
	Shader displayDensityShader("default.vert", "displayScalar.frag");
	Shader displayPressureShader("default.vert", "displayScalar.frag");
	Shader computeDivergenceShader("default.vert", "divergence.frag");
	Shader jacobiPoissonPressureShader("default.vert", "JacobiPoisson.frag");
	Shader subtractGradientShader("default.vert", "GradientSubtract.frag");
	Shader setVBoundaryShader("default.vert", "setBoundary.frag");
	Shader setPBoundaryShader("default.vert", "setBoundary.frag");
	Shader setDBoundaryShader("default.vert", "setBoundary.frag");
	Shader addDensityShader("default.vert", "addPropertyQuantity.frag");
	Shader addVelocityShader("default.vert", "addPropertyQuantity.frag");



	// Configure shader programs
	std::cout << "\n" << "Configuring Shader Programs"  << "\n" << std::endl;
	// - 
	advectVelocityShader.setUniform1f(timestep, "timestep");
	advectVelocityShader.setUniform1f(1.0f/1.0f, "rdx");	// (1 / dx)
	advectVelocityShader.setUniform1f(textureWidth, "texWidth");
	advectVelocityShader.setUniform1f(textureHeight, "texHeight");
	velocityTexture.texUnit(advectVelocityShader, "u", 0);
	velocityTexture.texUnit(advectVelocityShader, "q", 0);
	// - 
	advectDensityShader.setUniform1f(timestep, "timestep");
	advectDensityShader.setUniform1f(1.0f / 1.0f, "rdx");	// (1 / dx)
	advectDensityShader.setUniform1f(textureWidth, "texWidth");
	advectDensityShader.setUniform1f(textureHeight, "texHeight");
	velocityTexture.texUnit(advectDensityShader, "u", 0);
	densityTexture.texUnit(advectDensityShader, "q", 2);
	// - 
	computeDivergenceShader.setUniform1f(0.5f / 1.0f, "halfrdx");	// (0.5 / dx)
	velocityTexture.texUnit(computeDivergenceShader, "w", 0);
	// - 
	jacobiPoissonPressureShader.setUniform1f(-(1.0f*1.0f), "alpha");	// alpha = -(dx)^2
	jacobiPoissonPressureShader.setUniform1f(1.0f/4.0f, "rBeta");		// rBeta = 1 / 4
	pressureTexture.texUnit(jacobiPoissonPressureShader, "x", 1);
	divergenceVTexture.texUnit(jacobiPoissonPressureShader, "b", 3);
	// - 
	subtractGradientShader.setUniform1f(0.5f / 1.0f, "halfrdx");	// (0.5 / dx)
	pressureTexture.texUnit(subtractGradientShader, "p", 1);
	velocityTexture.texUnit(subtractGradientShader, "w", 0);
	// - 
	displayBoundaryShader.Activate();
	boundaryInfoTexture.texUnit(displayBoundaryShader, "texSample", 4);	// if display boundary
	displayBoundaryShader.setUniform4f(0.5f, 0.5f, 0.5f, 0.5f, "scale");
	displayBoundaryShader.setUniform4f(0.5f, 0.5f, 0.5f, 0.5f, "bias");
	// - 
	densityTexture.texUnit(displayDensityShader, "texSample", 2);	// if display density
	displayDensityShader.setUniform1f(1.0f, "scale");
	displayDensityShader.setUniform1f(0.0f, "bias");
	// - 
	pressureTexture.texUnit(displayPressureShader, "texSample", 1);	// if display pressure
	displayPressureShader.setUniform1f(1.5f, "scale");
	displayPressureShader.setUniform1f(0.0f, "bias");
	// - 
	setVBoundaryShader.setUniform1f(-1.0f, "scale");
	boundaryInfoTexture.texUnit(setVBoundaryShader, "b", 4);
	velocityTexture.texUnit(setVBoundaryShader, "x", 0);
	// - 
	setPBoundaryShader.setUniform1f(1.0f, "scale");
	boundaryInfoTexture.texUnit(setPBoundaryShader, "b", 4);
	pressureTexture.texUnit(setPBoundaryShader, "x", 1);
	// - 
	setDBoundaryShader.setUniform1f(0.0f, "scale");
	boundaryInfoTexture.texUnit(setDBoundaryShader, "b", 4);
	densityTexture.texUnit(setDBoundaryShader, "x", 2);
	// - 
	addDensityShader.setUniform1f(0.1f, "radius");	// brush radius
	addDensityShader.setUniform2f(textureWidth, textureHeight, "texDims");
	densityTexture.texUnit(addDensityShader, "base", 2);
	addDensityShader.setUniform3f(0.01f, 0.0f, 0.0f, "strength");	// default strength
	addDensityShader.setUniform2f(0.1f, 0.1f, "position");	// default normalized event position
	// - 
	addVelocityShader.setUniform1f(0.1f, "radius");	// brush radius
	addVelocityShader.setUniform2f(textureWidth, textureHeight, "texDims");
	velocityTexture.texUnit(addVelocityShader, "base", 0);
	addVelocityShader.setUniform3f(0.02f, 0.02f, 0.0f, "strength");	// default strength
	addVelocityShader.setUniform2f(0.1f, 0.1f, "position");	// default normalized event position


	// Main while loop
	std::cout << "\n" << "Starting main while loop..." << "\n" << std::endl;
	while (!glfwWindowShouldClose(window))
	{

		// Bind VBO/VAO/EBO stuff
		fluidVAO.Bind();
		fluidVBO.Bind();
		fluidVEO.Bind();
		// Set viewport
		glViewport(0, 0, textureWidth, textureHeight);

		//--------------------------------------------------------------------------
		// Advect Velocity
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		advectVelocityShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(velocityTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Add Velocity
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		addVelocityShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(velocityTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Set Boundary Velocity
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		setVBoundaryShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		boundaryInfoTexture.Bind();
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(velocityTexture.ID, tempTexture.ID);

		
		//--------------------------------------------------------------------------
		// Project Velocity
		// a. compute divergence of velocity
		//--------------------------------------------------------------------------
		// bind/activate divergence Fragment Program
		computeDivergenceShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(divergenceVTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Project Velocity
		// b. solve poisson-pressure equation for pressure
		//--------------------------------------------------------------------------

		// iterate 40-80 times
		for (int ii = 0; ii < 80; ii++)
		{
			// bind/activate Jacobi Iteration Fragment Program (uses two texture units)
			jacobiPoissonPressureShader.Activate();
			// bind frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, fb);
			// attach output texture to frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
			// bind sampler texture
			pressureTexture.Bind();
			divergenceVTexture.Bind();
			// draw to output texture
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			// swap texture IDs
			swapTextureIDs(pressureTexture.ID, tempTexture.ID);

			// Set Boundary Pressure
			// bind/activate advect fragment program 
			setPBoundaryShader.Activate();
			// bind frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, fb);
			// attach output texture to frame buffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
			// bind sampler texture
			boundaryInfoTexture.Bind();
			pressureTexture.Bind();
			// draw to output texture
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			// swap texture IDs
			swapTextureIDs(pressureTexture.ID, tempTexture.ID);
		}
		//--------------------------------------------------------------------------
		// Project Velocity
		// c. subtract gradient of p from u
		//--------------------------------------------------------------------------
		// bind/activate Gradient subtract Fragment Program
		subtractGradientShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		pressureTexture.Bind();
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(velocityTexture.ID, tempTexture.ID);
		

		//--------------------------------------------------------------------------
		// Set Boundary Velocity
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		setVBoundaryShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		boundaryInfoTexture.Bind();
		velocityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(velocityTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Advect Density
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		advectDensityShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		velocityTexture.Bind();
		densityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(densityTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Add Density
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		addDensityShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		densityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(densityTexture.ID, tempTexture.ID);
		//--------------------------------------------------------------------------
		// Set Boundary Density
		// 
		//--------------------------------------------------------------------------
		// bind/activate advect fragment program 
		setDBoundaryShader.Activate();
		// bind frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		// attach output texture to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempTexture.ID, 0);
		// bind sampler texture
		boundaryInfoTexture.Bind();
		densityTexture.Bind();
		// draw to output texture
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// swap texture IDs
		swapTextureIDs(densityTexture.ID, tempTexture.ID);

		//--------------------------------------------------------------------------
		// Display
		// 
		//--------------------------------------------------------------------------
		// Bind VBO/VAO/EBO stuff
		canvasVAO.Bind();
		canvasVBO.Bind();
		canvasEBO.Bind();
		// Set view port
		glViewport(0, 0, windowWidth, windowHeight);
		//// bind/activate display fragment program 
		//displayDensityShader.Activate();
		//// Binds sampler texture so that it appears in render
		//densityTexture.Bind();
		if (displayBoundary)
		{
			displayBoundaryShader.Activate();
			boundaryInfoTexture.Bind();
		}
		else
		{
			// bind/activate display fragment program 
			displayDensityShader.Activate();
			// Binds sampler texture so that it appears in render
			densityTexture.Bind();

			//displayPressureShader.Activate();
			//pressureTexture.Bind();

		}
		// Bind default FrameBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign new color (the glClearColor) to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Show stuff on screen; Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		//--------------------------------------------------------------------------
		// Other Stuff
		// 
		//--------------------------------------------------------------------------
		// Take care of all GLFW events
		glfwPollEvents();
	}
	
	
	// Delete all the objects we've created
	// TODO: cleanup and delete objects
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}