#version 430 core
/**

Used to print something with a vertex location and no texture

Also, it's instanced

*/
uniform mat4 wvp;
layout (location = 0) in vec4 vPosition;
out vec4 color;
out vec4 pos;
void main(){
	pos = wvp * vPosition;
	gl_Position = wvp * vPosition;
	color = vPosition;
}
