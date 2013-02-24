#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix; 
uniform mat4 projMatrix;

uniform vec3 vertex_color;

//hard coded for the moment
vec4 light_pos = vec4(0.0, 0.8, 0.5, 1.0);

//vec3 light_pos = vec3(0.0, 0.0, 0.0);

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 Color;

out vec3 normal;
out vec3 light_dir;                                                     
out vec3 eye_dir;  


void main()
{
    Color = vertex_color; //vec3( 0.6, 0.6, 0.6 );
    
    // position in model-view space:
    vec4 vec = vec4( vertex_position, 1.0 );
    
    vec4 pos = viewMatrix * modelMatrix * vec; 
    vec4 light_cam = viewMatrix * modelMatrix * light_pos;
    
    // position in screen position:
    gl_Position = projMatrix *  pos;
    
    /// TODO: should be passed by application instead of here!!!
    mat4 normalMatrix = inverse( transpose( viewMatrix * modelMatrix ) );
    
    vec4 n = vec4( vertex_normal, 1.0 );
    n = normalize ( normalMatrix * n );
    normal = n.xyz;
    
    light_dir = light_cam.xyz - pos.xyz;
    
    eye_dir = -pos.xyz;
}