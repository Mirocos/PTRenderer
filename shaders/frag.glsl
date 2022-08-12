#version 460 core



in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;





void main(){
    vec3 normal = normalize(Normal);
    normal = normal * 0.5f + 0.5f;
    FragColor = vec4(normal, 1.0f);
}
