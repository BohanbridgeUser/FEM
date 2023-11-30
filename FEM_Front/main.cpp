#include <QtGui>
#include <QSurfaceFormat>
#include "window.h"

#include <CGAL/Qt/resources.h>

int main(int argv, char **args)
{	
	QApplication app(argv, args);
	app.setOrganizationDomain("bohan.com");
  	app.setOrganizationName("Lotus");
	app.setApplicationName("FEM_FRONT");

	CGAL_QT_INIT_RESOURCES;

	QSurfaceFormat glFormat;
	glFormat.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(glFormat);

	MainWindow window;
	window.show();

	return app.exec();
}
