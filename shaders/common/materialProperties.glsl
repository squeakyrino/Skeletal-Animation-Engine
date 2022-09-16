#ifndef MATERIAL_PROP_H
#define MATERIAL_PROP_H
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 
#endif