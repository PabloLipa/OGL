#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;


uniform sampler2D myTex;

void main(){

	// Output color = color of the texture at the specified UV
	color = texture( myTex, UV ).rgb;
}
