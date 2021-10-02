#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// Gets the texture unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	
	// testing with 5x5
	vec4 tempColor = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0);
	if (gl_FragCoord.xy == vec2(0.5, 0.5))
	{
		//tempColor.rgb = vec3(0.5,0.5,0.5);
		tempColor = texelFetch(tex0, ivec2(2, 3), 0);
	}
	if (gl_FragCoord.xy == vec2(2.5, 2.5))
	{
		//tempColor.rgb = vec3(0.5,0.5,0.5);
		tempColor = texelFetch(tex1, ivec2(0, 0), 0);
	}
	FragColor = tempColor;


	

	//vec4 tempColor = texture(tex0, nTexCoord) + vec4(0.005f, 0.005f, 0.005f, 0.0f);
	//vec4 tempColor = texture(tex0, nTexCoord);
	/*
	vec4 tempColor = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0) + vec4(0.005f, 0.005f, 0.005f, 0.0f);
	if (tempColor.x <= 1.0f)
	{
		FragColor = tempColor;
	}
	else
	{
		FragColor = vec4(0.0f, 0.0f, 0.0f, tempColor.w);
	}
	*/
}	


