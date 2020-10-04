#version 430 core

layout (location = 0) in vec4 vPosition;

uniform mat4 scale;

void main(){
     gl_Position = vPosition * scale;
     gl_PointSize = 50.0;
}