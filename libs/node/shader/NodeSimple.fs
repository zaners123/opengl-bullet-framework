#version 430 core

//main simplest fragment shader. Just gradients vertex-defined colors

out vec4 fColor;
in vec4 color;

in vec2 texCoord;
uniform sampler2D Tex1;

void main(){
     fColor = color;
// 	 fColor = texture(Tex1, texCoord);
}
