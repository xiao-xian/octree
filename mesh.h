//
//  mesh.h
//  octree
//
//  Created by Xian Xiao on 31/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef octree_mesh_h
#define octree_mesh_h

#include <vector>
#include <math.h>

template< typename T> class Vertex
{
public:
     
    Vertex(){}
    Vertex( const T a, const T b, const T c ) : x(a), y(b), z(c){}
    
    virtual ~Vertex(){}
    
    inline T length() const { return sqrt( x*x + y*y + z*z ); }
    
    inline T distTo( const Vertex< T > &i_v )
    { 
        return sqrt( (x-i_v.x)*(x-i_v.x )+(y-i_v.y)*(y-i_v.y)+(z-i_v.z)*(z-i_v.z) );
    }
    
public:
    T  x, y, z;
    
};


template< typename T > class Mesh
{
public: 
    Mesh(){}
    virtual ~Mesh(){}
    
    std::vector< Vertex< T > > m_vertices;
    std::vector< Vertex< T > > m_normals;
};


#endif
