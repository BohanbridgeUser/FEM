#ifndef _WINDOW_
#define _WINDOW_

// std
#include <QWidget>

// Qt
#include <QString>
#include "scene.h"
#include "glviewer.h"
#include "ui_cad2mesh.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
	Q_OBJECT

private:
	Scene* m_scene;
	std::string m_info = "";

	unsigned int maxNumRecentFiles;
	QAction* recentFilesSeparator;
	QVector<QAction*> recentFileActs;

public:
	MainWindow();
	~MainWindow();

	void update();

protected:
	void addRecentFiles(QMenu* menu, QAction* insertBefore = 0);    
	unsigned int maxNumberOfRecentFiles() const {return maxNumRecentFiles;}
	void *updateChart(double min_ratio, double max_ratio, double min_area, double max_area);

	protected slots:

		// drag & drop
		void closeEvent(QCloseEvent *event);

		// recent files
		void openRecentFile_aux();
		void updateRecentFileActions();
		void addToRecentFiles(QString fileName);

		// io
		void on_actionLoad_triggered();
		void open(const QString& file);
		void saveCameraInFile(const QString &filename);
		void openCameraFromFile(const QString &filename);
		void on_actionSave_camera_triggered();
		void on_actionLoad_camera_triggered();

		// settings
		void setInput_point_size(double d);
		void setColor_point_size(double d);
		void setMesh_edge_width(double d);
		void setVector_ratio(double d);

		void connectSlots();

	public slots:

	    // Data
		void on_actionClear_data_triggered();

		// View
		void on_actionView_mesh_facets_toggled();
		void on_actionView_mesh_edges_toggled();
		void on_actionView_mesh_normals_toggled();
		
	signals:
		void openRecentFile(QString filename);
};

#endif // _WINDOW_
