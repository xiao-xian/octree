//
//  helpers.cpp
//  octree
//
//  Created by Xian Xiao on 31/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>

using namespace std;
#include "helpers.h"

bool load_obj( const char *i_path, Mesh< float > &i_mesh )
{    
    FILE * file = fopen(i_path, "r");
    if( file == NULL )
    {
        cout<<" Failed to load OBJ "<<i_path<<endl;
        return false;
    }
    
    while( 1 )
    {
        char lineHeader[128];
    
        int res = fscanf(file, "%s", lineHeader);
        
        if (res == EOF)
        {
            break; // EOF = End Of File. Quit the loop.
        }
        
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            float x, y, z; 
            fscanf(file, "%f %f %f\n", &x, &y, &z );
            Vertex< float > v( x, y, z );
            i_mesh.m_vertices.push_back( v );
        }
        else if( strcmp( lineHeader, "vn" ) == 0 )
        {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z );
            //Vertex<float> v( x, y, z );
            i_mesh.m_normals.push_back( Vertex<float>( x, y, z ) );
            // TODO: for the rest of entries, we skip...
        }
        else 
        {
            
        }      
    }
    
    return true;
}


float WorldSpace::axis[ 6 ] = { -size, 0, 0, size, 0, 0 };

WorldSpace::WorldSpace()
:initialized( false )
{
    plane.reserve( size * size * 3 );
    
    for( int i = -size; i <= size; i++ )
    {
        
        plane.push_back( -size );
        plane.push_back( 0 );
        plane.push_back( i );
        
        plane.push_back( size );
        plane.push_back( 0 );
        plane.push_back( i );
        
        plane.push_back( i );
        plane.push_back( 0 );
        plane.push_back( -size );
        
        plane.push_back( i );
        plane.push_back( 0 );
        plane.push_back( size );
        
    }
    
    phong_shader = std::tr1::shared_ptr<glsl_shader>( new glsl_shader() );
}

WorldSpace::~WorldSpace()
{
    // TODO: delete vbo
    if( this->plane_id ){}
}

void 
WorldSpace::init()
{
    if( !initialized )
    {
        std::string path = "../../../../octree/shader/phong";
        phong_shader->init(path);
        
        // vertex
        
        glGenVertexArrays( 1, &vao_id );
        glBindVertexArray( vao_id );
   
        glGenBuffers(1, &axis_id );
        glBindBuffer(GL_ARRAY_BUFFER, axis_id );
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &axis[0], GL_STATIC_DRAW );
        GLuint loc = glGetAttribLocation( phong_shader->program_id, "vertex_position" );
        
        glEnableVertexAttribArray(loc);
        
        glVertexAttribPointer( loc, 3, GL_FLOAT, 0, 0, 0);
                
        glGenBuffers(1, &plane_id );
        glBindBuffer(GL_ARRAY_BUFFER, plane_id );
        glBufferData(GL_ARRAY_BUFFER, plane.size() * sizeof( float ), &plane[0], GL_STATIC_DRAW );
        loc = glGetAttribLocation( phong_shader->program_id, "vertex_position" );
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer( loc, 3, GL_FLOAT, 0, 0, 0);
        
        initialized = true;
        glBindVertexArray( 0 );
    }

}

void 
WorldSpace::draw()
{
    if( !initialized ) 
        return; 
    
    if( vao_id )
    {
        glUseProgram( phong_shader->program_id );
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "modelMatrix"), 1, false, model.get() );
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "viewMatrix" ), 1, false, view.get() );
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "projMatrix"),  1, false, project.get()  );
        
        float color[3] = { 0.4,  0.4,  0.4 };
        glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), color[0], color[1], color[2] );
        glBindVertexArray( vao_id );
        glDrawArrays(GL_LINES, 0,  plane.size() / 3 );
        
#if 0 
        {
            color[0] = 1.; color[1] = 0; color[2] = 0;
            glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), color[0], color[1], color[2] );
            glBindVertexArray( vao_id );
            glDrawArrays(GL_LINES, 0,  2 );
        }
        
        {
            color[0] = 0; color[1] = 1.; color[2] = 0;
            Matrix4X4 ymat = model;
            set_rotation_matrix(ymat, 90., 2 );
            glUseProgram( phong_shader->program_id );
            glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "modelMatrix"), 1, false, ymat.get() );
            glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), color[0], color[1], color[2] );
            glDrawArrays(GL_LINES, 0, 2 );
        }
        
        {
            color[0] = 0; color[1] = 0.; color[2] = 1;
            Matrix4X4 zmat = model;
            set_rotation_matrix(zmat, 90., 1 );
            glUseProgram( phong_shader->program_id );
            glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "modelMatrix"), 1, false, zmat.get() );
            glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), color[0], color[1], color[2] );
            glDrawArrays(GL_LINES, 0, 2 );
        }
        
#endif  


    }

}




