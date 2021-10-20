#version 330 core

// The Jacobi Iteration Fragment Program Used to Solve Poisson Equations
// Good convergence typically requires 40 - 80 iterations per the gpugems article

// For diffusion equation,
    // x = u; alpha = ((dx)^2)/(visc*dt); rBeta = 1 / ( 4 + ((dx)^2)/(visc*dt) ) ; b = u
// For solving pressure,
    // x = p; alpha = -(dx)^2; rBeta = 1/4 ; b = divergence(w)

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0, for 0 to 255
out vec4 xNew; // new x // line was out vec4 newFragColor;

uniform float alpha;
uniform float rBeta; // reciprocal beta    

// Gets the texture units from the main function
// Assume GL_TEXTURE_WRAP_* are GL_CLAMP_TO_EDGE
uniform sampler2D x;    // x vector (Ax = b) 
uniform sampler2D b;    // b vector (Ax = b) 

void main()
{
    // left, right, bottom, and top x samples
    vec4 xL = texelFetch(x, ivec2(gl_FragCoord.xy) - ivec2(1, 0), 0);
    vec4 xR = texelFetch(x, ivec2(gl_FragCoord.xy) + ivec2(1, 0), 0);
    vec4 xB = texelFetch(x, ivec2(gl_FragCoord.xy) - ivec2(0, 1), 0);
    vec4 xT = texelFetch(x, ivec2(gl_FragCoord.xy) + ivec2(0, 1), 0);

    // b sample, from center
    vec4 bC = texelFetch(b, ivec2(gl_FragCoord.xy), 0);

    // evaluate Jacobi iteration
    xNew = (xL + xR + xB + xT + alpha * bC) * rBeta;
}