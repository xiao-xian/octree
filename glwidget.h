#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

#include "octree.h"
#include "helpers.h"
#include <tr1/memory>

class glwidget : public QGLWidget
{
    Q_OBJECT
public:
    glwidget(QGLFormat &format, QWidget *parent = 0);

public slots:

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent ( QKeyEvent * event );

private:

    std::tr1::shared_ptr< octree_render<float>  >  _render;

    // matrix
    Matrix4X4 g_model, g_view, g_project;

    QPoint lastPos;
    float g_camera_rotX, g_camera_rotY;
    float g_camera_dist;

    int g_display_level;
    WorldSpace g_grid;
};

#endif // GLWIDGET_H
