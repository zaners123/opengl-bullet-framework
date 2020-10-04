#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 incolor;
layout (location = 2) in vec4 instance_position;

layout (location = 3) in vec3 spareAttr;

uniform mat4 wvp;

out vec4 color;

void main(){
//	vec4 pos = vPosition * sqrt(2.0/3.0/sqrt(3));//If you solve for the side length of a hexagon, knowing the area's one, you get this
	vec4 pos = vPosition * 2 / sqrt(3);//If you solve for the side length of a hexagon, knowing the area's one, you get this
	pos *=.5;
	pos.y = vPosition.y;
	if (spareAttr.x==1) pos.z+=.5f;

	vec4 ipos = instance_position;
	ipos.x *= sqrt(3)/2;

	pos += ipos;


	gl_Position = wvp * (pos);
	//testing todo get image wrapping rather than everything being the same color
	color = vec4(spareAttr.y/2,spareAttr.y/6,mod(ipos.x,1),0);
	color = vec4(gl_Position,0,0,0);
}