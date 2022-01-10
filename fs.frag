#version 400

out vec3 n;
out vec3 p;

void main() {
	vec3 L = normalize(gl_LightSource[0].position.xyz - p);  
	vec3 V = normalize(-p); 
	vec3 R = normalize(-reflect(L,N));
}
