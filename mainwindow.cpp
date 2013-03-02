#include "mainwindow.h"

#include <QtWidgets>
#include <QGLFormat>

#include "glwidget.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow()
{
    QGLFormat glFormat;
    glFormat.setVersion(3,2 );
    glFormat.setProfile( QGLFormat::CoreProfile );
    glFormat.setSampleBuffers( true );

    _glWidget = new glwidget(glFormat);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(_glWidget);
    setLayout(mainLayout);

    setWindowTitle(tr("Octree Experiment"));

}

MainWindow::~MainWindow()
{
    
}
