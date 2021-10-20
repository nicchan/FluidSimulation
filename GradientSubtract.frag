#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0, for 0 to 255
out vec4 uNew;	// new velocity


uniform float halfrdx;	// (0.5 / dx)
// Assume GL_TEXTURE_WRAP_* are GL_CLAMP_TO_EDGE
uniform sampler2D p;	// pressure scaler field; only x component is being used
uniform sampler2D w;	// velocity vector field; only x and y components are being used 


void main()
{
	vec4 pL = texelFetch(p, ivec2(gl_FragCoord.xy) - ivec2(1, 0), 0);  
	vec4 pR = texelFetch(p, ivec2(gl_FragCoord.xy) + ivec2(1, 0), 0);   
	vec4 pB = texelFetch(p, ivec2(gl_FragCoord.xy) - ivec2(0, 1), 0);   
	vec4 pT = texelFetch(p, ivec2(gl_FragCoord.xy) + ivec2(0, 1), 0); 

	uNew = texelFetch(w, ivec2(gl_FragCoord.xy), 0);   
  	uNew.xy -= halfrdx * vec2(pR.x - pL.x, pT.x - pB.x); // two components of gradient of P in finite difference form
}
 