#version 430 core
/**

Used to print something with a vertex location and no texture

Also, it's instanced

*/
uniform mat4 wvp;
layout (location = 0) in vec4 vPosition;
layout (location = 3) in vec4 instanceLoc;

out vec4 color;
out vec4 pos;
void main(){
	gl_Position = pos = wvp * (vPosition+instanceLoc);
	color = vPosition;
}
