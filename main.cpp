//
//  main.cpp
//  octree
//
//  Created by Xian Xiao on 31/01/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <sys/time.h>

#include "helpers.h"
#include "mesh.h"
#include "octree.h"

//#define TOPDOWN_BUILD_OCTREE 1


// Mouse movement variables:
int         g_mouseX        = 0;
int         g_mouseY        = 0;
float       g_cameraDist    = 0.5f;
float       g_cameraAngleX  = 0.f;
float       g_cameraAngleY  = 0.f;
bool        g_rightDown     = false;
bool        g_leftDown      = false;
float       g_translate     = 0; 

// matrix
Matrix4X4 g_model, g_view, g_project;


const int g_max_level = 10;
float g_level = g_max_level;

WorldSpace world;

//--------------------------------------------------------------------------------------
//  Callback for changing window's size
//--------------------------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio;
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    h =  (h == 0 ? 1 : h );
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    ratio = (1.0f * w) / float( h );

    set_projection_matrix(g_project, 53.13, ratio, 0.01f, 30.0f );
    
}

//--------------------------------------------------------------------------------------
// Record the mouse screen coordinates and working out the movement
//--------------------------------------------------------------------------------------
void mouse_move( int x, int y )
{
    if(g_leftDown)
    {
        g_cameraAngleY += (x - g_mouseX);
        g_cameraAngleX += (y - g_mouseY);
        g_mouseX = x;
        g_mouseY = y;
    }
    
    if(g_rightDown)
    {
        g_cameraDist -= (y - g_mouseY) * 0.05f;
        g_mouseY = y;
    }
    
    // update transformation 
    
    Matrix4X4 identity;
    g_model = identity;
    g_view = identity;
    g_project = identity;
    
    set_translation_matrix(g_view, g_translate, 0, -6*g_cameraDist );
    set_rotation_matrix(g_view, g_cameraAngleX, 0.);
    
    Matrix4X4 rotM;
    set_rotation_matrix(rotM, g_cameraAngleY, 1.);
    mult_matrix( g_view, rotM );
}

//--------------------------------------------------------------------------------------
// Record the mouse button and press event, working out the movement
//--------------------------------------------------------------------------------------
void mouse_button( int button, int event )
{
    int x, y; 
    glfwGetMousePos(&x, &y);
    
    g_mouseX = x;
    g_mouseY = y;
    
    if( GLFW_MOUSE_BUTTON_LEFT == button )
    {
        if( event == GLFW_PRESS ) g_leftDown = true; 
        else if( event == GLFW_RELEASE ) g_leftDown = false;    
    }
    else if( GLFW_MOUSE_BUTTON_RIGHT == button )
    {
        if( event == GLFW_PRESS ) g_rightDown = true; 
        else if( event == GLFW_RELEASE ) g_rightDown = false;  
    }
    
}

//--------------------------------------------------------------------------------------
//  Callback for handle keyboard events. 
//--------------------------------------------------------------------------------------
void processKeys( int key, int action ) 
{    
    //cout<<" press key "<<key<<endl;
    if (key == 27) 
    {
        exit(0);
    }
    else if( key == 283 ) // up
    {
        if( g_level < g_max_level )
        {
            // TODO, bug in glfw, it's called twice every time a key is pressed
            g_level += 0.5;
        }
    }
    else if( key == 284 ) // down
    {
        if( g_level > 0 )
        {
            g_level -= 0.5;
        }
    }
    else if( key == 285 ) // left
    {
        g_translate -= 0.5;
    }
    else if( key == 286 ) // right
    {
        g_translate += 0.5;
    }
}
void render_scene( octree_render< float > &render )
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    int l = static_cast<int>( g_level );
    render.levels[ l ].model = g_model;
    render.levels[ l ].view = g_view;
    render.levels[ l ].project = g_project;
    
    render.levels[ l ].draw();
    
    world.set_transform(g_model, g_view, g_project ); 
    world.draw();
    
    glPopAttrib();
}

int main(int argc, const char * argv[])
{
    int running = GL_TRUE;

    if( !glfwInit() )
    {
        exit( EXIT_FAILURE );
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4 );
    
    // Open an OpenGL window
    if( !glfwOpenWindow( 1024, 768, 8,8,8,8,24,8, GLFW_WINDOW ) )
    {
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    glfwSetWindowTitle( "OpenGL Volume Render Demo" );
    
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST );

    // Load OBJ mesh
    Mesh< float > mesh;
    load_obj("../../obj/teapot_d.obj", mesh );
    
    // Create Octree
    float voxel_size = 0.03;
    Octree< float > octree( g_max_level, mesh, voxel_size);
    
    // Build 
#if TOPDOWN_BUILD_OCTREE 
    octree.top_down_build( 0, 0 );
#else
    octree.bottom_up_build();
#endif
    
    octree_render< float > render( octree );
    world.init();
    
    // Add mouse motion callbacks: 
    glfwSetMousePosCallback( mouse_move );
    glfwSetMouseButtonCallback( mouse_button );
    glfwSetKeyCallback( processKeys );
    
    // Main loop
    while( running )
    {   
        glfwSetWindowTitle( "Octree OpenGL 3.2 Demo" );
        
        // resize callback to setup viewing
        glfwSetWindowSizeCallback( changeSize );
        
        // core rendering:
        render_scene( render  );
     
        // swap buffers:
        glfwSwapBuffers();
        
        // Check if ESC key was pressed or window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) &&
        glfwGetWindowParam( GLFW_OPENED );
    }
    
    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    exit( EXIT_SUCCESS );
    
    return 0;

}

