#version 430 core
//a standard instance shader, with color. Use a custom shader for anything else

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 incolor;
layout (location = 2) in vec4 instance_position;
//layout (location = 3) in vec4 colorCube;

uniform mat4 wvp;

out vec4 color;
out vec4 pos;

void main(){
	pos = wvp * (vPosition + instance_position);

	gl_Position = wvp * (vPosition + instance_position);
	color = incolor;
//	color.x = mod(color.x + float(gl_InstanceID)/2,1);
//	color.y = mod(color.y + float(gl_InstanceID)/4,1);
//	color.z = mod(color.z + float(gl_InstanceID)/8,1);

	//fog
//	float dist =(gl_Position.x*gl_Position.x + gl_Position.y*gl_Position.y + gl_Position.z*gl_Position.z);
//	if (gl_Position.x < 0) dist*=2;
//	dist = 3000/(dist+2000);
//	color = vec4(color.x * dist,color.y * dist,color.z * dist,0);
}