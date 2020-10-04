#version 430 core

//main used to print something with a location and vertex color. Simple times call for simple measures

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 incolor;

uniform mat4 wvp;

out vec4 color;
out vec4 pos;

void main(){
	pos = wvp * vPosition;
	gl_Position = wvp * vPosition;
	color = incolor;
}
