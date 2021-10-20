#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0, for 0 to 255
out vec4 div; // divergence // line was out vec4 newFragColor;

uniform float halfrdx;   // (0.5 / dx)
// Assume GL_TEXTURE_WRAP_* are GL_CLAMP_TO_EDGE
uniform sampler2D w;	// vector field

void main()
{
    vec4 wL = texelFetch(w, ivec2(gl_FragCoord.xy) - ivec2(1, 0), 0);
    vec4 wR = texelFetch(w, ivec2(gl_FragCoord.xy) + ivec2(1, 0), 0);
    vec4 wB = texelFetch(w, ivec2(gl_FragCoord.xy) - ivec2(0, 1), 0);
    vec4 wT = texelFetch(w, ivec2(gl_FragCoord.xy) + ivec2(0, 1), 0);

	div = vec4( halfrdx * ((wR.x - wL.x) + (wT.y - wB.y)) , 0, 0, 0);
}