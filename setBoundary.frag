#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0, for 0 to 255
out vec4 value;	// output value 

uniform float scale;	// scale parameter
uniform sampler2D x;	// state field

// boundary texture (lookup)
// four components per texel
// x: offset in x
// y: offset in y
// z: offset in z (not used in 2D)
// w: isBoundaryCell (1 or 0)
uniform sampler2D b;


void main()
{
	// look up info in boundary texture
	vec4 temp = texelFetch(b, ivec2(gl_FragCoord.xy), 0).xyzw;

	// check if boundary cell
	if (temp.w == 1)
	{
		// set boundary cell value
		vec2 offset = temp.xy;
		value = scale * texelFetch(x, ivec2(gl_FragCoord.xy + offset), 0); 
	}
	else
	{
		// direct value pass through for non-boundary cell
		value = texelFetch(x, ivec2(gl_FragCoord.xy), 0);
	}
}
