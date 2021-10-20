#version 330 core

// Outputs pixel color data with 4 components (RGBA)
// For display purposes, this must be normalized. ie. range from 0.0 to 1.0
out vec4 FragColor;
// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// Scale
uniform vec4 scale;
// Bias
uniform vec4 bias;
// Gets the texture Unit from the main function
uniform sampler2D texSample;



/*
 * The scale and bias parameters
 * allow the manipulation of the values in the texture 
 * before display.  This is useful, for example, if the values in the texture 
 * are signed.  A scale and bias of 0.5 can bring the range [-1, 1] into the 
 * range [0, 1] for visualization or other purposes.
*/

void main()
{
	FragColor = bias + scale * texture(texSample, nTexCoord);
	//FragColor = bias + scale * texture(texSample, vec2(0,0));

	/*
	vec4 value = texelFetch(texSample, ivec2(0, 0), 0);
	FragColor = bias + scale * texelFetch(texSample, ivec2(0, 0), 0);
	if (value.x == 1)
	{
		FragColor = bias + scale * vec4(0,1,0,1);
	}
	else
	{
		FragColor = vec4(1,0,0,1);
	}
	*/
}


