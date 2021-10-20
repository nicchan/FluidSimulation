#version 330 core

// Outputs pixel data with 4 components (RGBA)
out vec4 outColor; // output value 
in vec2 nTexCoord;

uniform vec3 strength;     // "strength"
uniform vec2 position;  // normalized event position

uniform float radius;   // brush radius
uniform vec2 texDims;

uniform sampler2D base;


float gaussian(vec2 pos, float radius)
{
  return exp(-dot(pos, pos) / radius);
}

void main()
{
    vec2 pos = texDims * position - gl_FragCoord.xy;
    float rad = texDims.x * radius;

    //outColor = texture(base, nTexCoord) + color.xyzz * gaussian(pos, rad);
    outColor = texelFetch(base, ivec2(gl_FragCoord.xy), 0) + strength.xyzz * gaussian(pos, rad);

    /*
    outColor = texelFetch(base, ivec2(gl_FragCoord.xy), 0) + strength.xyzz;
    if (position.x <0)
    {
        outColor = (pos*rad).xxxx + vec4(1,0,0,1);
    }
    */
}


/*
void main(half2       coords : TEX0,
  
      out half4       outColor : COLOR,
  
  uniform half3       color,
  uniform half2       position,
  uniform half        radius,
  uniform half2       windowDims,
  uniform samplerRECT base)
{
  half2 pos = windowDims * position - coords;
  half  rad = windowDims.x * radius;
      
  outColor = h4texRECT(base, coords) + color.xyzz * gaussian(pos, rad);
} 
*/

