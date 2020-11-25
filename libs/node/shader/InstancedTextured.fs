#version 430 core

out vec4 fColor;

in vec4 color;
in vec2 texCoorded;

uniform sampler2D Tex1;

void main(){
//      fColor = color;
	 fColor = vec4(vec3(texture(Tex1, texCoorded)),1.0);
// 	 fColor = texture(Tex1, texCoorded) + color;
}
