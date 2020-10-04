#version 430 core

uniform vec4 fColorIn;
out vec4 fColor;

void main(){


//      fColor = vec4(1.0, 0.0, 0.0, 1.0);
     fColor = fColorIn;
}

