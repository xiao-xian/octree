//
//  shader.cpp
//  octree
//
//  Created by Xian Xiao on 4/02/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "shader.h"
//

#include <iostream>
#include <fstream>
using namespace std;

glsl_shader::glsl_shader()
{
}

glsl_shader::~glsl_shader()
{
    if( program_id )
    {
        glDeleteProgram( program_id );
    }
}

void glsl_shader::init( string &path )
{
    string vertShader = path+".vert";
    string fragShader = path+".frag";
    
    string vertCode, fragCode;
    
    std::ifstream VertexShaderStream(vertShader.c_str(), std::ios::in);
    
	if(VertexShaderStream.is_open())
    {
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			vertCode += "\n" + Line;
		VertexShaderStream.close();
	}
    
    std::ifstream FragmentShaderStream( fragShader.c_str(), std::ios::in );    
    
    if(FragmentShaderStream.is_open())
    {
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			fragCode += "\n" + Line;
		FragmentShaderStream.close();
	}
    
    program_id = glCreateProgram();
    
    if (program_id == 0)
    {
        cout<<"Error creating shader program! "<<endl;
        return;
    }
    
    add_shader( vertCode.c_str(), GL_VERTEX_SHADER);
    add_shader( fragCode.c_str(), GL_FRAGMENT_SHADER);
    
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };
    
    glLinkProgram( program_id);
    glGetProgramiv( program_id, GL_LINK_STATUS, &Success);
    
    if (Success == 0)
    {
        glGetProgramInfoLog(program_id, sizeof(ErrorLog), NULL, ErrorLog);
        cout<< "Error linking shader program : "<<ErrorLog<<endl;
        return;
    }

}

void glsl_shader::add_shader(const char* pShaderText, GLenum ShaderType )
{
    GLuint ShaderObj = glCreateShader(ShaderType);
    if (ShaderObj == 0)
    {
        cout<<"Error creating shader type : "<<ShaderType<<endl;
        return;
    }
    
    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        cout<<"Error compiling shader type : "<<ShaderType<<" : "<<InfoLog<<endl;
        return;
    }
    
    glAttachShader(program_id, ShaderObj);
}
