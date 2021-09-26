#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// Gets the texture unit from the main function
uniform sampler2D tex0;

void main()
{
	
	vec4 tempColor = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0) + vec4(0.005f, 0.005f, 0.005f, 0.0f);
	//vec4 tempColor = texture(tex0, nTexCoord) + vec4(0.005f, 0.005f, 0.005f, 0.0f);

	if (tempColor.x <= 1.0f)
	{
		FragColor = tempColor;
	}
	else
	{
		FragColor = vec4(0.0f, 0.0f, 0.0f, tempColor.w);
	}
}


