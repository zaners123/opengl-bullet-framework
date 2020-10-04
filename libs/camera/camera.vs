#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vColor;

uniform mat4 matrix;

out vec4 color;

void main(){
//	color = vec4(vColor.x,vColor.y,vColor.z,1);
	color = vec4(1,1,1,1);
	gl_Position = matrix * vPosition;
}
