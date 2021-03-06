#version 430

START_VERTEX

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoord;

uniform mat4 T_MVP;
uniform float R_useMVP;


out vec3 pos;
out vec2 texCoord0;

void main(){
	texCoord0 = texCoord.xy;
	
	vec3 v = position;
	v.y -= 1;
	
	if(R_useMVP == 1.0) gl_Position = T_MVP * vec4(position, 1);
	else gl_Position = vec4(position, 1);
	
	
	pos = position;
	
}

END_VERTEX



START_FRAGMENT

in vec2 texCoord0;
in vec3 pos;

uniform sampler2D diffuse;
uniform float opacity;
uniform vec3 color;
uniform float red;

out vec4 finalColor;

void main(){
//gl_FragDepth = texture(R_gBuffer_3T, texCoord0).r;



finalColor = texture(diffuse, texCoord0);
//finalColor = vec4(texCoord0, 0, 1);

}




END_FRAGMENT






