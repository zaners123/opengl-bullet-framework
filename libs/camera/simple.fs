#version 430 core

//main simplest fragment shader. Just gradients vertex-defined colors

out vec4 fColor;
in vec4 color;

void main(){
     fColor = color;
}
