// Qt
#include <QtGui>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include <QClipboard>

#include "window.h"

#include <fstream>
#include <string>

MainWindow::MainWindow() : 
QMainWindow(), Ui_MainWindow(), 
maxNumRecentFiles(15), recentFileActs(15)
{
	setupUi(this); 
    
	// init scene
	m_scene = new Scene;
	viewer->set_scene(m_scene);
	textBrowser->setPlainText(QString::fromStdString(m_info));
    
	// Handling actions
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    
	addRecentFiles(menuData, actionQuit); 
	connect(this, SIGNAL(openRecentFile(QString)), this, SLOT(open(QString)));
	connectSlots();

	// dock windows
	menuWindow->addAction(dockWidget_infos->toggleViewAction());
	menuWindow->addAction(dockWidget_options->toggleViewAction());
}

MainWindow::~MainWindow()
{
	delete m_scene;
}

void MainWindow::update()
{
	viewer->repaint();
	textBrowser->setPlainText(QString::fromStdString(m_info));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}

/******************* I/O **********************/

void MainWindow::addToRecentFiles(QString fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileListLotus").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > (int)maxNumRecentFiles)
        files.removeLast();
    settings.setValue("recentFileListLotus", files);
    updateRecentFileActions();
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileListLotus").toStringList();
    
    int numRecentFiles = qMin(files.size(), (int)maxNumberOfRecentFiles());
    for (int i = 0; i < numRecentFiles; ++i) {
        QString strippedName = QFileInfo(files[i]).fileName();
        QString text = tr("&%1 %2").arg(i).arg(strippedName);
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (unsigned int j = numRecentFiles; j < maxNumberOfRecentFiles(); ++j)
        recentFileActs[j]->setVisible(false);
    
    recentFilesSeparator->setVisible(numRecentFiles > 0);
}

void MainWindow::addRecentFiles(QMenu* menu, QAction* insertBeforeAction)
{
    if (insertBeforeAction)
        recentFilesSeparator = menu->insertSeparator(insertBeforeAction);
    else 
        recentFilesSeparator = menu->addSeparator();
    recentFilesSeparator->setVisible(false);
    
    for (unsigned int i = 0; i < maxNumberOfRecentFiles(); ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile_aux()));
        if (insertBeforeAction)
            menu->insertAction(insertBeforeAction, recentFileActs[i]);
        else
            menu->addAction(recentFileActs[i]);
    }
    updateRecentFileActions();
}

void MainWindow::openRecentFile_aux()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        emit openRecentFile(action->data().toString());
}

void MainWindow::open(const QString& filename)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	m_scene->load(filename);
	double x, y, z, r;
    m_scene->get_bounding_shape(x, y, z, r);
	viewer->adjustCamera(x, y, z, std::max(r, 2.));
	QApplication::restoreOverrideCursor();
	addToRecentFiles(filename);
	update();
}

/******************* Visualization **********************/

void MainWindow::on_actionView_mesh_facets_toggled()
{
	m_scene->toggle_view_mesh_facets();
	update();
}

void MainWindow::on_actionView_mesh_edges_toggled()
{
	m_scene->toggle_view_mesh_edges();
	update();
}

void MainWindow::on_actionView_mesh_normals_toggled()
{
	m_scene->toggle_view_mesh_normals();
	update();
}

/******************* Camera **********************/

void MainWindow::on_actionSave_camera_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "", "*.cam");
	if(!fileName.isNull())
		saveCameraInFile(fileName);
}

void MainWindow::on_actionLoad_camera_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "", "*.cam");
	if (!fileName.isNull()) {                 // got a file name
		openCameraFromFile(fileName);
	}
}

void MainWindow::saveCameraInFile(const QString &filename)
{
	std::ofstream out (filename.toUtf8());
	if (!out)
		exit (EXIT_FAILURE);
	// << operator for point3 causes linking problem on windows
	out << 	viewer->camera()->position()[0] << " \t" << viewer->camera()->position()[1] << " \t" << viewer->camera()->position()[2] << " \t " <<
		   	viewer->camera()->viewDirection()[0] << " \t" << viewer->camera()->viewDirection()[1] << " \t" << viewer->camera()->viewDirection()[2] << " \t " <<
			viewer->camera()->upVector()[0] << " \t" << viewer->camera()->upVector()[1] << " \t" << viewer->camera()->upVector()[2] << " \t " <<
			viewer->camera()->fieldOfView();
	out << std::endl;

	out.close();
}

void MainWindow::openCameraFromFile(const QString &filename){

	std::ifstream file;
	file.open(filename.toStdString().c_str());

	CGAL::qglviewer::Vec pos;
	CGAL::qglviewer::Vec view;
	CGAL::qglviewer::Vec up;
	float fov;

	file >> (pos[0]) >> (pos[1]) >> (pos[2]) >>
			(view[0]) >> (view[1]) >> (view[2]) >>
			(up[0]) >> (up[1]) >> (up[2]) >>
			fov;

	viewer->camera()->setPosition(pos);
	viewer->camera()->setViewDirection(view);
	viewer->camera()->setUpVector(up);
	viewer->camera()->setFieldOfView(fov);

	viewer->camera()->computeModelViewMatrix();
	viewer->camera()->computeProjectionMatrix();

	update();
}

void MainWindow::on_actionLoad_triggered() 
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
	if (!fileName.isEmpty())
		open(fileName);
}

void MainWindow::on_actionClear_data_triggered()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
  	m_scene->clear_data();
	QApplication::restoreOverrideCursor();
	update();
}

//********************** Option **********************//

void MainWindow::connectSlots()
{
	connect(this->spinInput_point_size, SIGNAL(valueChanged(double)), this, SLOT(setInput_point_size(double)));
	connect(this->spinColor_point_size, SIGNAL(valueChanged(double)), this, SLOT(setColor_point_size(double)));
	connect(this->spinMesh_edge_width, SIGNAL(valueChanged(double)), this, SLOT(setMesh_edge_width(double)));
	connect(this->spinVector_ratio, SIGNAL(valueChanged(double)), this, SLOT(setVector_ratio(double)));
}

void MainWindow::setInput_point_size(double d)
{
	m_scene->setInput_point_size(d);
}

void MainWindow::setColor_point_size(double d)
{
	m_scene->setColor_point_size(d);
}

void MainWindow::setMesh_edge_width(double d)
{
	m_scene->setMesh_edge_width(d);
}

void MainWindow::setVector_ratio(double d)
{
	m_scene->setVector_ratio(d);
}