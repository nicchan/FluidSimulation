#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0, for 0 to 255
out vec4 qNew;	// advected qty

// TODO texture storage scale? (currently only has 255 as max per component)

uniform float timestep;
uniform float rdx;	// (1 / dx)
uniform float texWidth;	// texture width
uniform float texHeight;	// texture height
uniform sampler2D u;	// input velocity
uniform sampler2D q;	// qty to be advected

// min max texture coordinates at boundaries
float fragCoordMinX = 1.0f;
float fragCoordMinY = 1.0f;
float fragCoordMaxX = texWidth - 1.0f;
float fragCoordMaxY = texHeight - 1.0f;

// helper functions
float lerp(float start, float end, float t){return start+(end-start)*t;}
float blerp(float c00, float c10, float c01, float c11, float tx, float ty)
{
    return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}


void main()
{
	// follow the velocity field "back in time"
	// TODO: explore using rungeKutta3
	vec2 pos = gl_FragCoord.xy - timestep * rdx * texelFetch(u, ivec2(gl_FragCoord.xy), 0).xy; 
	
	if (pos == gl_FragCoord.xy)
	{
		qNew = texelFetch(q, ivec2(gl_FragCoord.xy), 0);
	}
	else
	{
		// clamp the position to be within the boundaries
		pos = clamp(pos, vec2(fragCoordMinX, fragCoordMinY), vec2(fragCoordMaxX, fragCoordMaxY));

		// interpolate and write to the output fragment
		// gp : grid point of bilerp grid ; q : quantity

		// grid point 00
		float gp00x = (pos.x - 0.5) - fract(pos.x - 0.5) + 0.5;
		float gp00y = (pos.y - 0.5) - fract(pos.y - 0.5) + 0.5;
		vec2 gp00 = vec2(gp00x, gp00y);
		vec4 q00 = texelFetch(q, ivec2(gp00), 0);

		// grid point 11
		float gp11x = gp00x + 1;
		float gp11y = gp00y + 1;
		vec2 gp11 = vec2(gp11x, gp11y);
		vec4 q11 = texelFetch(q, ivec2(gp11), 0);

		// grid point 01
		float gp01x = gp00x;
		float gp01y = gp11y;
		vec2 gp01 = vec2(gp01x, gp01y);
		vec4 q01 = texelFetch(q, ivec2(gp01), 0);

		// grid point 10
		float gp10x = gp11x;
		float gp10y = gp00y;
		vec2 gp10 = vec2(gp10x, gp10y);
		vec4 q10 = texelFetch(q, ivec2(gp10), 0);

		// interpolating factors
		float tx = pos.x - gp00x;
		float ty = pos.y - gp00y;

		// write results to output
		qNew.x = blerp(q00.x, q10.x, q01.x, q11.x, tx, ty);
		qNew.y = blerp(q00.y, q10.y, q01.y, q11.y, tx, ty);
		qNew.z = blerp(q00.z, q10.z, q01.z, q11.z, tx, ty);
		qNew.w = blerp(q00.w, q10.w, q01.w, q11.w, tx, ty);
	}
}





