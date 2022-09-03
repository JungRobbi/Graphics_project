#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTex;

out vec3 outColor;
out vec3 FragPos;
out vec3 Normal;
out vec2 Tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() { 
  gl_Position = projection * view * model * vec4(vPos, 1.0);
  FragPos = vec3(model*vec4(vPos,1.0));
  Normal = vec3(model*vec4(vNormal,0.0));
  outColor=vColor;
  Tex = vTex;
}