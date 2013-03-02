#version 150


in vec3 normal;
in vec3 light_dir;                                                     
in vec3 eye_dir;  

in vec3 Color;
out vec4 outputF;

const vec4 diffuse_color = vec4( 0.4, 0.4, 0.4, 1.0);
const vec4 ambient_color = vec4( 0.0, 0.0, 0.0, 1.0 );
const vec4 specular_color = vec4( 1.0, 1.0, 1.0, 1.0);

void main(void)                                                             
{                                                                           
    vec4 final_color = vec4( Color, 1);

    
    vec3 N = normalize(normal);                                             
    vec3 L = normalize(light_dir);                                           
    
    float lambertTerm = dot(N,L);                                           
    
    final_color += ambient_color;
    
    if(lambertTerm > 0.0)                                                 
    {           
        float shine = 5.0;
        
        final_color += diffuse_color * lambertTerm;                                                       
        
        vec3 E = normalize(eye_dir);                                         
        vec3 R = reflect(-L, N);                       
        
        float specular = pow( max(dot(R, E), 0.0),  shine );                       
                 
        final_color += specular *  specular_color ;                                                                                 
    }                                                                      
    
    outputF = vec4( final_color.rgb, 1.0 );                                             
}
