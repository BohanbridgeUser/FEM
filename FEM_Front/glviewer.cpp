#include <QtGui>
#include <CGAL/Qt/CreateOpenGLContext.h>

#include "glviewer.h"


GlViewer::GlViewer(QWidget *pParent)
	: QGLViewer(pParent)
{
	m_scene = NULL;
}

void GlViewer::initializeGL() 
{
	QGLViewer::initializeGL();
  	setBackgroundColor(::Qt::white);
	setSceneRadius(2.);
}

void GlViewer::draw()
{
	QGLViewer::draw();
  	if(m_scene != NULL)
	{
		m_scene->render(this);
	}
}

void GlViewer::adjustCamera(double x, double y, double z, double r) {
    setSceneCenter(CGAL::qglviewer::Vec(x, y, z));
    setSceneRadius((float)(1.2 * r));
    showEntireScene();
}