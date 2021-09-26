#version 330 core

// Outputs colors 
out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// Gets the texture Unit from the main function
uniform sampler2D tex0;

void main()
{
	
	FragColor = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0);   
	//FragColor = texture(tex0, nTexCoord);
	

	/*
	vec4 tempColor = texture(tex0, nTexCoord) + vec4(0.1f, 0.1f, 0.1f, 0.1f);
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
