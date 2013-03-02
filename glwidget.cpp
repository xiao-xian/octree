#include "glwidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include "shader.h"
#include "mesh.h"
#include "helpers.h"

#include <iostream>
using namespace std;

#define MAX_LEVEL 10

glwidget::glwidget(QGLFormat &format, QWidget *parent) :
    QGLWidget(format, parent)
{
    g_camera_rotX  = 0;
    g_camera_rotY  = 0;
    g_camera_dist = -1.5;
    g_display_level = MAX_LEVEL;
}

QSize glwidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize glwidget::sizeHint() const
{
    return QSize(1280, 960);
}

void glwidget::initializeGL()
{
    cout<<" GL Version: "<<glGetString( GL_VERSION )<<endl;
    cout<<" GLSL Version: "<<glGetString(  GL_SHADING_LANGUAGE_VERSION )<<endl;

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,1.0);

    Mesh< float > mesh;
    load_obj("../../../../octree/obj/bunny_l.obj", mesh );

    float voxel_size = 0.005;
    Octree< float > octree( MAX_LEVEL, mesh, voxel_size);

    octree.top_down_build( 0, 0 );
    _render = std::tr1::shared_ptr< octree_render<float> >( new octree_render<float>( octree ) );

    Matrix4X4 identity;
    g_model = identity;
    g_view = identity;
    g_project = identity;

    set_translation_matrix(g_view, 0, 0, -1.5 );
    set_rotation_matrix(g_view, 0, 0.);

    Matrix4X4 rotM;
    set_rotation_matrix(rotM, 0, 1.);
    mult_matrix( g_view, rotM );

    g_grid.init();
}

void glwidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // draw octree
    _render->levels[ g_display_level ].setTansform( g_model, g_view, g_project );
    _render->levels[ g_display_level ].draw();

    // draw plane grid
    g_grid.set_transform(g_model, g_view, g_project );
    g_grid.draw();

    glPopAttrib();
}

void glwidget::resizeGL(int w, int h)
{
    float ratio;

    h =  (h == 0 ? 1 : h );

    glViewport(0, 0, w, h);

    ratio = (1.0f * w) / h;

    set_projection_matrix(g_project, 53.13, ratio, 0.01f, 30.0f );

    updateGL();
}


void glwidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void glwidget::mouseMoveEvent(QMouseEvent *event)
{

    if( event->buttons() & Qt::RightButton )
    {
        g_camera_dist -= ( event->y() - lastPos.y() ) * 0.05f;
    }
    else if( event->buttons() & Qt::LeftButton )
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        g_camera_rotX += dy;
        g_camera_rotY += dx;
    }

    Matrix4X4 identity;
    g_view = identity;
    set_translation_matrix(g_view, 0, 0, g_camera_dist );
    set_rotation_matrix(g_view, g_camera_rotX, 0.);

    Matrix4X4 rotM;
    set_rotation_matrix(rotM, g_camera_rotY, 1.);

    mult_matrix( g_view, rotM );

    updateGL();

    lastPos = event->pos();
}

void glwidget::keyPressEvent ( QKeyEvent * event )
{
    if( event->key() == Qt::Key_Up )
    {
        g_display_level++;
        g_display_level = ( g_display_level > MAX_LEVEL ? MAX_LEVEL : g_display_level );
    }
    else if( event->key() == Qt::Key_Down )
    {
        g_display_level--;
        g_display_level = ( g_display_level < 0 ? 0 : g_display_level );
    }

    updateGL();
}
