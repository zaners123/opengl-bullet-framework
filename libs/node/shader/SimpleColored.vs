#version 430 core
/**

Used to print something with a vertex location and vertex color.

*/
uniform mat4 wvp;
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 incolor;
out vec4 color;
out vec4 pos;
void main(){
	pos = wvp * vPosition;
	gl_Position = wvp * vPosition;
	color = incolor;
}
