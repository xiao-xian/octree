//
//  Matrix4X4.h
//  volumeRender
//
//  Created by Xian Xiao on 27/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef volumeRender_Matrix4X4_h
#define volumeRender_Matrix4X4_h

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <iostream>

typedef float T;

struct Vector3 {
    
public:
    Vector3() { data[0] = data[1] = data[2] = 0.0; }
    Vector3( T x, T y, T z ){ data[0] = x; data[1] = y; data[2] = z; }
    T& operator []( int i ){ assert( i < 3 ); return data[ i ]; }
    T* get(){ return data; }
    
private:
    T data[3];
};

struct Matrix4X4
{
public:
    Matrix4X4()
    {
        // fill matrix with 0s
        for (int i = 0; i < 16; ++i )
            data[ i ] = 0.0f;
        
        // fill diagonal with 1s
        for (int i = 0; i < 4; ++i )
            data[i + i * 4 ] = 1.0f;
    }
    
    Matrix4X4( const Matrix4X4 &i_mat )
    {
        for( int i = 0; i < 16; i++)
            data[ i ] = i_mat[ i ];
    }
    
    Matrix4X4& operator=( Matrix4X4 &i_mat )
    {
        for( int i = 0; i < 16; i++)
            data[ i ] = i_mat[ i ];
        
        return *this;
    }
    
    T& operator [] ( int i ){ assert( i < 16 ); return data[ i ]; }
    const T operator[] (int i ) const { assert( i < 16 ); return data[ i ]; }
    
    // todo: bad idea here DO NOT DELETE
    T* get() { return data; }
    
private:
    
    T data[16];
};

//--------------------------------------------------------------------------------------
// print 
//--------------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out,Matrix4X4& i_a )
{
    out<<"[ ";
    for( int i = 0; i < 16; i++)
    {
        out<<i_a[ i ]<<" ";
    }
    out<<"] "<< std::endl;
    return out;
}

inline Vector3& operator *= ( Vector3& i_v, Matrix4X4& i_a )
{
    i_v[ 0 ] = i_a[12] + i_a[ 0 ] * i_v[0] + i_a[ 1 ]*i_v[1] + i_a[ 2 ]*i_v[2];
    i_v[ 1 ] = i_a[13] + i_a[ 4 ] * i_v[0] + i_a[ 5 ]*i_v[1] + i_a[ 6 ]*i_v[2];
    i_v[ 2 ] = i_a[14] + i_a[ 8 ] * i_v[0] + i_a[ 9 ]*i_v[1] + i_a[ 10 ]*i_v[2];
    
    return i_v;
}

//--------------------------------------------------------------------------------------
// Cross product : res = a X b
//--------------------------------------------------------------------------------------
inline void cross_product( Vector3 &i_a, Vector3 &i_b, Vector3 &i_res) 
{
    i_res[0] = i_a[1] * i_b[2]  -  i_b[1] * i_a[2];
    i_res[1] = i_a[2] * i_b[0]  -  i_b[2] * i_a[0];
    i_res[2] = i_a[0] * i_b[1]  -  i_b[0] * i_a[1];
}

//--------------------------------------------------------------------------------------
//  Normalize a vec3
//--------------------------------------------------------------------------------------
inline void normalize( Vector3 &i_a )
{    
    float mag = sqrt( i_a[0] * i_a[0]  
                    + i_a[1] * i_a[1]  
                    + i_a[2] * i_a[2] );
    
    i_a[0] /= mag;
    i_a[1] /= mag;
    i_a[2] /= mag;
}

//--------------------------------------------------------------------------------------
//  Defines a transformation matrix mat with a translation
//--------------------------------------------------------------------------------------
inline void set_translation_matrix( Matrix4X4 &i_mat, float x, float y, float z) 
{    
    i_mat[12] = x;
    i_mat[13] = y;
    i_mat[14] = z;
}

//--------------------------------------------------------------------------------------
//  Defines a transformation matrix mat with a rotation along an axis 
//--------------------------------------------------------------------------------------
inline void set_rotation_matrix( Matrix4X4 &o_mat, float angle, const short axis )
{
        
    angle *= 3.1415926 / 180. ;
    
    if( axis == 0 )
    {
        o_mat[ 1 * 4 + 1 ] = cos( angle );
        o_mat[ 1 * 4 + 2 ] = sin( angle );
        o_mat[ 2 * 4 + 1 ] = -sin( angle );
        o_mat[ 2 * 4 + 2 ] = cos( angle );
    }
    else if( axis == 1 )
    {
        o_mat[ 0 * 4 + 0 ] = cos( angle );
        o_mat[ 0 * 4 + 2 ] = -sin( angle );
        o_mat[ 2 * 4 + 0 ] = sin( angle );
        o_mat[ 2 * 4 + 2 ] = cos( angle );
    }
    else if(axis == 2 )
    {
        o_mat[ 0*4 + 0 ] = cos( angle );
        o_mat[ 0*4 + 1] = sin( angle );
        o_mat[ 1*4 + 0 ] = -sin( angle );
        o_mat[ 1*4 + 1 ] = cos( angle );
    }
}

//--------------------------------------------------------------------------------------
//  Projection Matrix
//--------------------------------------------------------------------------------------
inline void set_projection_matrix( Matrix4X4 &o_mat, float fov, float ratio, float nearP, float farP )
{
    float f = 1.0f / tan (fov * (M_PI / 360.0));
    
    Matrix4X4 identity;
    o_mat = identity;
    
    o_mat[ 0 ]       = f / ratio;
    o_mat[1 * 4 + 1] = f;
    o_mat[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    o_mat[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    o_mat[2 * 4 + 3] = -1.0f;
    o_mat[3 * 4 + 3] = 0.0f;
}


//--------------------------------------------------------------------------------------
//  Matrix multiplication a = a * b
//--------------------------------------------------------------------------------------
inline void mult_matrix( Matrix4X4 &i_a, Matrix4X4 &i_b ) 
{
    T res[16];
    
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j) 
        {
            res[ j*4 + i] = 0.0f;
            
            for (int k = 0; k < 4; ++k) 
            {
                res[ j*4 + i] += i_a[ k*4 + i] * i_b[j*4 + k];
            }
        }
    }
    
    memcpy(i_a.get(), res, 16 * sizeof(T));
}

//--------------------------------------------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//
//--------------------------------------------------------------------------------------

inline void set_view_matrix( float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ, 
                     Matrix4X4 &o_view ) 
{
    Vector3 dir, right, up; 
    
    up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
     
    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    
    normalize( dir );
    
    cross_product(dir,up,right);

    normalize(right);

    cross_product(right,dir,up);
    
    normalize(up);

    
    o_view[0]  = right[0];
    o_view[4]  = right[1];
    o_view[8]  = right[2];
    o_view[12] = 0.0f;
    
    o_view[1]  = up[0];
    o_view[5]  = up[1];
    o_view[9]  = up[2];
    o_view[13] = 0.0f;
    
    o_view[2]  = -dir[0];
    o_view[6]  = -dir[1];
    o_view[10] = -dir[2];
    o_view[14] =  0.0f;
    
    o_view[3]  = 0.0f;
    o_view[7]  = 0.0f;
    o_view[11] = 0.0f;
    o_view[15] = 1.0f;
    
    Matrix4X4 aux;
    
    set_translation_matrix(aux, -posX, -posY, -posZ);
    
    mult_matrix(o_view, aux);
}


#endif
