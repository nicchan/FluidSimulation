#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Normalized Texture Coordinates
layout (location = 2) in vec2 aNormTex;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
// normalized
out vec2 nTexCoord;

// Controls the scale of the vertices
uniform float vertScale;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x + aPos.x * vertScale, aPos.y + aPos.y * vertScale, aPos.z + aPos.z * vertScale, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns texture coordinates from the Vertex Data to "texCoord"
	nTexCoord = aNormTex;
	//TexCoord = ivec2(aTex);
}