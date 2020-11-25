#version 430 core
/**

Used to print something with a vertex location, texture wrapping, and texture.

*/
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 incolor;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec4 instanceLoc;

uniform mat4 wvp;
out vec4 color;
out vec4 pos;
out vec2 texCoorded;
void main(){
	texCoorded = texCoord;
	gl_Position = pos = wvp * (vPosition+instanceLoc);
	color = incolor;
}
