#version 330 core

// Outputs colors 
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the texture Unit from the main function
uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, texCoord);

	
	/*
	vec4 tempColor = texture(tex0, texCoord) + vec4(0.1f, 0.1f, 0.1f, 0.1f);
	if (tempColor.r <= 1.0f)
	{
		FragColor = tempColor;
	}
	else
	{
		FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	*/
}
