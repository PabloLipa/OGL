#version 330 core

// entrada
layout(location = 0) in vec3 VPms;
layout(location = 1) in vec2 vertexUV;

// saida
out vec2 UV;


uniform mat4 MVP;

void main(){

	// MVP * position
	gl_Position =  MVP * vec4(VPms,1);
	
	UV = vertexUV;
}

