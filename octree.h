//
//  octree.h
//  octree
//
//  Created by Xian Xiao on 31/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef octree_octree_h
#define octree_octree_h

#include "shader.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

#include <tr1/memory>

#include <math.h>

#include "data_structure.h"
#include "mesh.h"

typedef struct{ int i, j, k;} Int3;

template< typename T > class Node
{
public:
    Node():x(0.),y(0.),z(0.),level(0),value(0){}
    Node( T a, T b, T c ) : x(a), y(b), z(c),level(0), value(0){}
    
    virtual ~Node(){}
    
    int                 child[2][2][2];     // children index 
    T                   x, y, z;            // center position
    int                 level;              // tree level
    std::vector< int >  vertIds;            // vertex index into a mesh
    T                   value;
};

template< typename T > class Octree
{
public:
    
    Octree( int max_layers, Mesh< T > &i_mesh, T i_step = 0.5 ) 
    : leaf_layer( max_layers ), mesh( i_mesh ),step( i_step )
    {
    }
    
    ~Octree(){}
    
    void top_down_build( int level, int parent_id = 0 );
    
    void bottom_up_build();
    
    uint64_t morton_encode(uint64_t x, uint64_t y, uint64_t z)
    {
        uint64_t answer = 0;
        for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT); ++i) 
        {
            answer |= ((x & ((uint64_t)1 << i)) << 2*i) | ((y & ((uint64_t)1 << i)) << (2*i + 1)) | ((z & ((uint64_t)1 << i)) << (2*i + 2));
        }
        return answer;
    }
 
    void morton_decode(uint64_t morton, uint64_t &x, uint64_t &y, uint64_t &z )
    {
        x = 0; y = 0; z = 0;
        for (uint64_t i = 0; i < (sizeof(uint64_t) * CHAR_BIT); ++i) 
        {
            x |= ((morton & ((uint64_t) 1 << 3*i)) >> (3*i)-i);
            y |= ((morton & ((uint64_t) 1 << (3*i+1))) >> (3*i+1)-i);
            z |= ((morton & ((uint64_t) 1 << (3*i+2))) >> (3*i+2)-i);
        }
    }
    
public:
    
    std::vector< Node< T > > nodes;
    int leaf_layer;
    T step;
    Mesh< T >  mesh;
    
    std::multimap< uint64_t, Int3 > motorn_code;
};


template< typename T > struct octree_level 
{
    octree_level()
    {
        phong_shader = std::tr1::shared_ptr<glsl_shader>( new glsl_shader() );
    }
    virtual  ~octree_level(){}
    
    void addCube( T x, T y, T z, T i_scale  );
    
    void initBuffer();
    void setTansform( Matrix4X4 &i_model, Matrix4X4 &i_view, Matrix4X4 &i_project );
    void draw();
    
    std::vector< T >        vertices;
    std::vector< T >        normals;
    
    std::vector< GLuint  >  face_index;

private:
    std::tr1::shared_ptr< glsl_shader >  phong_shader;
    
    GLuint                  vertex_buffer;
    GLuint                  normal_buffer;
    GLuint                  index_buffer;
    GLuint                  vao_id;
    
    Matrix4X4               model, view, project;
};

template< typename T > struct octree_render
{
    typedef std::map< int, octree_level< T > > LevelMap;
    
    explicit octree_render( Octree< T >& octree );
    ~octree_render(){}
    
    LevelMap levels;
        
};

#endif
