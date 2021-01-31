#version 430 core

out vec4 fColor;

in vec4 color;
in vec2 texCoorded;
in vec2 id;

uniform sampler2D Tex1;

void main(){
	 if(id.x==0)fColor = vec4(float(0x1f)/256,float(0x6d)/256,float(0x04)/256,1);
     if(id.x==1)fColor = vec4(float(0x6b)/256,float(0x9b)/256,float(0x1e)/256,1);
     if(id.x==2)fColor = vec4(float(0x8d)/256,float(0xbf)/256,float(0x39)/256,1);
     if(id.x==3)fColor = vec4(float(0xb9)/256,float(0xd9)/256,float(0x80)/256,1);
// 	 fColor = vec4(vec3(texture(Tex1, texCoorded)),1.0);
// 	 fColor = texture(Tex1, texCoorded) + color;
}
