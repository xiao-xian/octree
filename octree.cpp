//
//  octree.cpp
//  octree
//
//  Created by Xian Xiao on 3/02/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "octree.h"

using namespace std;

#include <stack>
#include <boost/tuple/tuple.hpp>

static GLfloat cube_vert[ 108 ] = 
{
    // Front1
    -0.5,-0.5,-0.5,
    -0.5, 0.5,-0.5,
    0.5, 0.5,-0.5,
    
    // Front2
    0.5, 0.5,-0.5,
    0.5,-0.5,-0.5,
    -0.5,-0.5,-0.5,
    
    // Top1
    -0.5, 0.5,-0.5,
    -0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    
    // Top2
    0.5, 0.5, 0.5,
    0.5, 0.5,-0.5,
    -0.5, 0.5,-0.5,
    
    // Back1
    -0.5,-0.5, 0.5,
    -0.5, 0.5, 0.5,
    0.5, 0.5, 0.5,
    
    // Back2
    0.5, 0.5, 0.5,
    0.5,-0.5, 0.5,
    -0.5,-0.5, 0.5,
    
    // Bottom1
    -0.5,-0.5,-0.5,
    -0.5,-0.5, 0.5,
    0.5,-0.5, 0.5,
    
    // Bottom2
    0.5,-0.5, 0.5,
    0.5,-0.5,-0.5,
    -0.5,-0.5,-0.5,
    
    // Left Side1
    -0.5,-0.5,-0.5,
    -0.5, 0.5,-0.5,
    -0.5, 0.5, 0.5,
    
    // Left Side2
    -0.5, 0.5, 0.5,
    -0.5,-0.5, 0.5,
    -0.5,-0.5,-0.5,
    
    // Right Side1
    0.5,-0.5,-0.5,
    0.5, 0.5,-0.5,
    0.5, 0.5, 0.5,
    
    // Right Side2
    0.5, 0.5, 0.5,
    0.5,-0.5, 0.5,
    0.5,-0.5,-0.5
};

GLfloat cube_normal[ 108 ] =
{
    0, 0,-1,
    0, 0,-1,
    0, 0,-1,
    
    0, 0,-1,
    0, 0,-1,
    0, 0,-1,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    
    0,-1, 0,
    0,-1, 0,
    0,-1, 0,
    
    0,-1, 0,
    0,-1, 0,
    0,-1, 0,
    
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};

template< typename T > 
void Octree<T>::top_down_build( int level, int parent_id)
{
    if( !level && !parent_id )
    {
        // root
        nodes.push_back( Node<T>() );
        
        nodes[ parent_id ].vertIds.resize( mesh.m_vertices.size() );
        
        for( int i = 0, num = mesh.m_vertices.size(); i < num; i++ )
        {
            nodes[ parent_id ].vertIds[ i ] = i;
        }
    }
    
    if( !nodes[ parent_id ].vertIds.size() )
    {
        // parent node doesn't have any points
        return;
    }
    
    if(  nodes[ parent_id ].level >= ( leaf_layer  ))
    {
        return;
    }
    
    T quad_size = pow( T(2.), T( leaf_layer - 2 - level ) ) * step;
    
    // Add 8 children and update their center coordinates
    for( int i = 0; i < 2; i++ ){
        for( int j = 0; j < 2; j++){
            for( int k = 0; k < 2; k++){
                
                nodes[ parent_id ].child[i][j][k] = static_cast<int>(nodes.size() );
                nodes.push_back( Node<T>());
                nodes[ nodes[ parent_id ].child[i][j][k] ].x = nodes[parent_id].x 
                + pow( T(-1), T(i) ) * quad_size;
                nodes[ nodes[ parent_id ].child[i][j][k] ].y = nodes[parent_id].y
                + pow( T(-1), T(j) ) * quad_size;
                nodes[ nodes[ parent_id ].child[i][j][k] ].z = nodes[parent_id].z 
                + pow( T(-1), T(k) ) * quad_size;
                
                nodes[ nodes[ parent_id ].child[i][j][k] ].level = level + 1 ;
            }
        }
    }
    
    //assign vertex ids to childern...
    for( int i=0, num = nodes[ parent_id ].vertIds.size(); 
        i < num; i++ )
    {
        int vertId = nodes[ parent_id ].vertIds[ i ];
        
        int chx = ( mesh.m_vertices[ vertId ].x > nodes[ parent_id ].x ? 0 : 1 );
        int chy = ( mesh.m_vertices[ vertId ].y > nodes[ parent_id ].y ? 0 : 1 );
        int chz = ( mesh.m_vertices[ vertId ].z > nodes[ parent_id ].z ? 0 : 1 );
        
        int child_id = nodes[ parent_id ].child[chx][chy][chz];
        
        nodes[ child_id ].vertIds.push_back( vertId );
    }
    
    // recursively call subdive for all children
    for( int i = 0; i < 2; i++ ){
        for( int j = 0; j < 2; j++){
            for( int k = 0; k < 2; k++){
                int child_id = nodes[ parent_id ].child[i][j][k];
                top_down_build(  level + 1, child_id );
            }
        }
    }
    
    return;
} // end of subdived

template <typename T >
void Octree<T>::bottom_up_build()
{
    const int num = mesh.m_vertices.size();
    std::map< uint64_t, Int3 > mortcode;
    
    // leaf node for each mesh vertex
    for( int i = 0; i < num; i++)
    {
        int ci =  static_cast< int >( mesh.m_vertices[ i ].x / step );
        int cj =  static_cast< int >( mesh.m_vertices[ i ].y / step );
        int ck =  static_cast< int >( mesh.m_vertices[ i ].z / step );
                           
        uint64_t mc = morton_encode( ci, cj, ck );
        
        Int3 coords;
        coords.i = ci; coords.j = cj; coords.k = ck;
        mortcode.insert( std::make_pair( mc, coords ) );
    }
    
    typedef std::map<uint64_t, boost::tuple<T, T, T, int > > NonLeafMap; 
    NonLeafMap non_leaf_map;
    
    // build non-leaf node 
    for( std::map< uint64_t, Int3 >::iterator it= mortcode.begin();
        it != mortcode.end(); it++ )
    {
        T x = step * T( it->second.i ) + 0.5 * step ;
        T y = step * T( it->second.j ) + 0.5 * step ;
        T z = step * T( it->second.k ) + 0.5 * step ;
        
        nodes.push_back( Node<T>( x, y, z) );
        nodes[ nodes.size() - 1 ].level = leaf_layer;
        
        //dummy one
        nodes[ nodes.size() - 1  ].vertIds.push_back( 0 );
        
        // 
        uint64_t mc = it->first;
        
        for( int l = leaf_layer - 1; l > 0; l-- )
        {
            uint64_t local = mc & 0x7;
            mc = ( mc >> 3 );
           
            T quad_size = pow( T(2.), T( leaf_layer - l -2 ) ) * step;
            
            x += pow( -1, ( (          local & 0x1 ) % 2 )) * quad_size;
            y += pow( -1, ( ( ( local >> 1 ) & 0x1 ) % 2 )) * quad_size;
            z += pow( -1, ( ( ( local >> 2 ) & 0x1 ) % 2 )) * quad_size;
            
            non_leaf_map.insert( std::make_pair( mc, boost::tuple< T,T,T, int >(x,y,z, l) ) );
        }
                             
    }
    
    for( typename std::map< uint64_t, boost::tuple<T, T, T, int> >::iterator iter = non_leaf_map.begin(); 
        iter != non_leaf_map.end(); ++iter )
    {
        T x = boost::get<0>( iter->second );
        T y = boost::get<1>( iter->second );
        T z = boost::get<2>( iter->second );
        
        nodes.push_back( Node<T>( x, y, z) );
        nodes[ nodes.size() - 1 ].level = boost::get<3>( iter->second ) ;
        nodes[ nodes.size() - 1 ].vertIds.push_back( 0 );
    }
}

template< typename T > void octree_level<T>::addCube(T x, T y, T z, T i_scale)
{
    for( int v = 0; v < 36 ; v++)
    {
        vertices.push_back( cube_vert[ v * 3 ] * i_scale + x );
        vertices.push_back( cube_vert[ v * 3 + 1 ] * i_scale + y );
        vertices.push_back( cube_vert[ v * 3 + 2 ] * i_scale + z );
        
        normals.push_back( cube_normal[ v * 3 ] );
        normals.push_back( cube_normal[ v * 3 + 1 ] );
        normals.push_back( cube_normal[ v * 3 + 2 ] );
        
        face_index.push_back( face_index.size() );
    }
}

template< typename T > void octree_level<T>::initBuffer()
{       
    std::string path = "../../../../octree/shader/phong";
    phong_shader->init(path);
    
    if( vertices.size() && normals.size() && face_index.size() )
    {
        glGenVertexArrays( 1, &vao_id );
        glBindVertexArray( vao_id );
        
        // vertex
        glGenBuffers(1, &vertex_buffer );
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer );
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), &vertices[0], GL_STATIC_DRAW );
        GLuint loc = glGetAttribLocation( phong_shader->program_id, "vertex_position" );
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer( loc, 3, GL_FLOAT, 0, 0, 0);
        
        // normal
        glGenBuffers(1, &normal_buffer );
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer );
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof( T ), &normals[0], GL_STATIC_DRAW );
        loc = glGetAttribLocation( phong_shader->program_id, "vertex_normal" );
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer( loc, 3, GL_FLOAT, 0, 0, 0);
        
        // index
        glGenBuffers(1, &index_buffer );
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer );
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_index.size() * sizeof( GLuint ), &face_index[0], GL_STATIC_DRAW );
        
        glBindVertexArray( 0 );
    }
}

template< typename T > void octree_level<T>::setTansform( Matrix4X4 &i_model, Matrix4X4 &i_view, Matrix4X4 &i_project )
{
    model = i_model;
    view = i_view;
    project = i_project;
}

template< typename T > void octree_level<T>::draw()
{
    if( vao_id )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        
        glUseProgram( phong_shader->program_id );
        
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "modelMatrix"), 1, false, model.get() );
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "viewMatrix" ), 1, false, view.get() );
        glUniformMatrix4fv( glGetUniformLocation( phong_shader->program_id, "projMatrix"),  1, false, project.get()  );
        
        float color[3] = { 0.0, 0.0, 0.0 };
        glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), color[0], color[1], color[2] );
        
        // draw!
        glBindVertexArray( vao_id );
        glDrawArrays(GL_LINES, 0, vertices.size() / 3 );
        
        glUseProgram( phong_shader->program_id );
        glUniform3f(glGetUniformLocation(phong_shader->program_id, "vertex_color"), 0.1, 0.2, 0.3 );
        
        glDrawElements(GL_TRIANGLES, face_index.size(), GL_UNSIGNED_INT, NULL);
    
        glPopAttrib();
        
    }
}


template< typename T > octree_render<T>::octree_render( Octree< T >& octree )
{
    for( int i = 0; i <= octree.leaf_layer; i++)
    {
        levels.insert( std::make_pair(i, octree_level<T>() ) );
    }
    
    // build level data
   
    for( int n = 0, num = octree.nodes.size(); n < num; n++)
    {
        Node<T> &node = octree.nodes[ n ];
        
        if( node.vertIds.size())
        {
            T scale = pow( 2., T( ( octree.leaf_layer - node.level ))  ) * octree.step;
            
            if( levels.find( node.level ) == levels.end() )
            {
                cout<<" exception: level error!!!"<<endl;
                continue;
            }
            
            levels[ node.level ].addCube( node.x, node.y, node.z, scale );
        }
    }
    
    for( int i = 0; i <= octree.leaf_layer; i++)
    {
        levels[ i ].initBuffer();
    }
    
}

// declare these to make linking work..
template class Octree< float >;
template struct octree_level< float >;
template struct octree_render< float >;

