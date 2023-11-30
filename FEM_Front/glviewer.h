#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <QtGlobal>
#include <CGAL/Qt/qglviewer.h>
#include "scene.h"

typedef typename CGAL::QGLViewer         QGLViewer;

class GlViewer : public QGLViewer 
{
    Q_OBJECT
    
private:
    Scene* m_scene;
    
public:
    GlViewer(QWidget *parent);
    
    void set_scene(Scene* pScene) { m_scene = pScene; }
    void draw();
    void adjustCamera(double x, double y, double z, double r);
    
protected:
    void initializeGL();
};

#endif
