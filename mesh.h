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
#include <limits>
#include <iostream>

template< typename T> class Vertex
{
public:
     
    Vertex():x(0),y(0),z(0){}
    Vertex( const T a ):x(a),y(a),z(a){}
    Vertex( const T a, const T b, const T c ) : x(a), y(b), z(c){}
    Vertex( const Vertex<T> &i_v ):x( i_v.x ), y( i_v.y ), z( i_v.z ){}
    
    virtual ~Vertex(){}
    
    inline T length() const { return sqrt( x*x + y*y + z*z ); }
    
    inline T distTo( const Vertex< T > &i_v )
    { 
        return sqrt( (x-i_v.x)*(x-i_v.x )+(y-i_v.y)*(y-i_v.y)+(z-i_v.z)*(z-i_v.z) );
    }

    inline void translateBy( const T tx, const T ty, const T tz )
    {
        x += tx; y += ty; z += tz;
    }

    inline void scaleTo( const T scale )
    {
        x *= scale; y *= scale; z *= scale;
    }
    
public:
    T  x, y, z;
    
};

inline std::ostream& operator<<(std::ostream& out,Vertex<float>& i_v )
{
    out<<"[ "<<i_v.x<<" "<<i_v.y<<" "<<i_v.z<<" ] "<<std::endl;
    return out;
}

template< typename T > class Mesh
{
public: 
    Mesh()
    {
        bbox_max = Vertex<T>( -std::numeric_limits<T>::max() );
        bbox_min = Vertex<T>( std::numeric_limits<T>::max() );
    }

    virtual ~Mesh(){}
    
    std::vector< Vertex< T > > m_vertices;
    std::vector< Vertex< T > > m_normals;

    std::vector< unsigned int > m_triangleIds;

    Vertex< T > bbox_min, bbox_max;
};


#endif
