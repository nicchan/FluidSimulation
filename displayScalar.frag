#version 330 core

// Outputs pixel color data with 4 components (RGBA)
// For display purposes, this must be normalized. ie. range from 0.0 to 1.0
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// Bias
uniform float bias;

// Scale
uniform float scale;

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
	float scalarValue = texture(texSample, nTexCoord).x;
	if (scalarValue < 0)
	{
		FragColor = (-scalarValue) * scale * vec4(1,0.25,0.25,1);
	}
	else
	{
		FragColor = (scalarValue) * scale * texture(texSample, nTexCoord).xxxx;
		//FragColor = bias + scale * texture(texSample, nTexCoord).xxxx;
	}
}


