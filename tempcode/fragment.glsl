
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 outColor;
in vec2 Tex;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D outTex;
uniform float alpha;

out vec4 fColor;

void main ()
{
  vec3 ambientLight = vec3(0.3f, 0.3f, 0.3f);
  vec3 ambient = ambientLight * lightColor;
  vec3 NV = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diffuseLight = max(dot(NV, lightDir), 0.0);
  vec3 diffuse = diffuseLight * lightColor;

  int shininess = 128;
  vec3 viewDir = normalize(cameraPos - FragPos);
  vec3 reflectDir = reflect(lightDir, NV);
  float specularLight = max (dot(viewDir, reflectDir), 0.0);
  specularLight = pow(specularLight, shininess);
  vec3 specular = specularLight * lightColor;

  vec3 result = (diffuse+specular+ambient) * outColor;

  vec4 texcolor = texture(outTex, Tex);

  if ( texcolor[0]+texcolor[1]+texcolor[2] <= 0.1 ){
     fColor = vec4((result),0.0);
  }
  else {
     fColor = vec4((result),10);
  }
  
  fColor = texture(outTex, Tex) * fColor;

}