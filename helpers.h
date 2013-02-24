//
//  helpers.h
//  octree
//
//  Created by Xian Xiao on 31/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef octree_helpers_h
#define octree_helpers_h
#include "data_structure.h"
#include "mesh.h"
#include "shader.h"
#include <tr1/memory>

bool load_obj( const char *i_file, Mesh< float > &i_mesh );

struct WorldSpace
{
    WorldSpace();
    ~WorldSpace();
    
    void init();
    void draw();
    void set_transform( Matrix4X4 &m, Matrix4X4 &v, Matrix4X4 &p )
    {
        model = m;
        view  = v;
        project = p;
    }
    
    static const int size = 10;
    
    static float axis[6];
    
    std::vector< float > plane;
    
     GLuint  axis_id;
     GLuint  plane_id;
     GLuint  vao_id;
    
private:
    bool initialized; 
    
    std::tr1::shared_ptr< glsl_shader >  phong_shader;
    
    Matrix4X4 model, view, project;
    
};

#endif
