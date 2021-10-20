#version 330 core

// Outputs pixel data with 4 components (RGBA)
// This is normalized. ie. range from 0.0 to 1.0
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 nTexCoord;

// vec4 uniform just to test it out
uniform vec4 myvec;
uniform vec4 myvec2;

// Gets the texture unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	
	// testing with 5x5
	vec4 tempColor = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0);
	// Bottom Left Corner
	if (gl_FragCoord.xy == vec2(0.5, 0.5))	
	{
		//tempColor.rgb = vec3(0.5,0.5,0.5);
		vec4 c = texelFetch(tex0, ivec2(2, 3), 0);
		if (c.b > 254)	// demonstrates that raw data value is fetched from texel 
		{
			FragColor = c;
		}
		else
		{
			FragColor = tempColor;
		}
	}
	// Mid
	else if (gl_FragCoord.xy == vec2(2.5, 2.5))
	{
		//tempColor.rgb = vec3(0.5,0.5,0.5);
		vec4 c = texelFetch(tex1, ivec2(4, 0), 0);

		if (c.x == -1)	// demonstrates that raw data value is fetched from texel 
		{
			FragColor = vec4(0,1,0,1);
		}
		else
		{
			FragColor = vec4(1,0,0,1);
		}
	}
	else if (gl_FragCoord.xy == vec2(0.5, 4.5))
	{
		FragColor = tempColor + vec4(0.005f, 0.005f, 0.005f, 0.0f);
		if (FragColor.x > 3.0f)
		{
			FragColor = vec4(0.0f, 0.0f, 0.0f, tempColor.w);
		}
	}
	else if (gl_FragCoord.xy == vec2(1.5, 3.5))
	{
		
		if (( myvec * myvec2 ).x == 0.5)
		{
			FragColor = tempColor + vec4(0.003f, 0.003f, 0.003f, 0.0f);
			if (FragColor.x > 1.0f)
			{
				FragColor = vec4(0.0f, 0.0f, 0.0f, tempColor.w);
			}
		}
	}
	else
	{
		FragColor = tempColor;
	}


	

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


