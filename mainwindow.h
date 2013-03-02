#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class glwidget;

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    MainWindow();
    ~MainWindow();

     glwidget *_glWidget;
};

#endif // MAINWINDOW_H
