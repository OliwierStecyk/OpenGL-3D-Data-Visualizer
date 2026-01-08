#version 330

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 texcoord0;
uniform vec3 lightPos; 
vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D tex0;
uniform float alfa;

struct Material { 
	vec3 ambient;     
	vec3 diffuse;     
	vec3 specular;     
	float shininess; 
					 
}; 

uniform Material material;
void main() {       
	   
	viewPos[0] =-100;
	viewPos[1] = -100;
	viewPos[2] = 500;
	
	// ambient     
	vec3 ambient = lightColor * material.ambient;     
	// diffuse      
	vec3 norm = normalize(Normal);   
	vec3 lightDir = normalize(lightPos - FragPos);    
	float diff = max(dot(norm, lightDir), 0.0);     
	vec3 diffuse = lightColor * (diff * material.diffuse);    
	// specular    
	vec3 viewDir = normalize(viewPos - FragPos);     
	vec3 reflectDir = reflect(-lightDir, norm);      
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);     
	vec3 specular = lightColor * (spec * material.specular);       
	vec3 result = ambient + diffuse + specular;    
	FragColor = vec4(result, alfa); 
}
