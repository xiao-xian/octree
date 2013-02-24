//
//  shader.h
//  octree
//
//  Created by Xian Xiao on 4/02/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef octree_shader_h
#define octree_shader_h

#define GLFW_INCLUDE_GL3 
#include <GL/glfw.h>

#include <string>


struct glsl_shader
{
    glsl_shader();
    ~glsl_shader();
    
    void init( std::string &path );
    GLuint program_id;
        
private:
    
    void add_shader( const char* pShaderText, GLenum ShaderType ); 
    
    
};

#endif
