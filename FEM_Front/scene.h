#ifndef _SCENE_H_
#define _SCENE_H_

// std
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <time.h>
#include <chrono>

// Qt
#include <QtOpenGL>
#include <QMessageBox>
#include <QPainter>
#include <QInputDialog>

// OpenGl
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

// CGAL
#include <CGAL/Qt/qglviewer.h>
#include "types.h"
#include "function.h"

#undef min
#undef max

struct MeshOptions
{
	// View
	float   input_point_size = 2.5f;
	float   color_point_size = 5.f;
	float   mesh_edge_width = 0.5f;
	float	vector_ratio = 0.1f;
};

class Scene
{
public:	
	typedef Lotus::Kernel							Kernel;	
	typedef Lotus::Point							Point;
	typedef Lotus::DataList							DataList;
	typedef Lotus::Function	  						Function;

private:

    Function        m_function;
	Point 			m_mouse_pos;
	MeshOptions 	m_mesh_options;

	/* Remember to check the path */
    const QString shader_path = "../../shader/";

	bool m_view_mesh_edges;
	bool m_view_mesh_facets;
	bool m_view_mesh_normals;
	
	// OpenGL
	bool 						gl_init;
	bool						are_buffers_initialized;
	QOpenGLFunctions	 		*gl;
	QOpenGLBuffer 				buffers[2];
    QOpenGLVertexArrayObject 	vao[2];
	QOpenGLShaderProgram 		rendering_program;
	QOpenGLShaderProgram		rendering_program_text;
	QOpenGLShaderProgram		rendering_program_func;
	QOpenGLShaderProgram		rendering_program_tet;
	QOpenGLShaderProgram		rendering_program_facet;
	DataList					mesh_facets; 	 	// vao[0] buffers[0]
	DataList					mesh_normals;  	 	// vao[0] buffers[1]

	// Shader Elements for Surface Text
	int 						mvpLocation_text;
	int							mvLocation_text;
	int 						lightLocation_text[5];
    int 						colorLocation_text;
	int 						poly_vertexLocation_text;
	int 						normalsLocation_text;
	int							colorLineLocation_text;
	int							widthLineLocation_text;
	int							vpLocation_text;
	int							flagEdgeLocation_text;
	// Shader Elements for Points / Vectors
	int 						points_vertexLocation;
    int 						mvpLocation;
    int 						colorLocation;
	// Shader Elements for Colored Funcs
	int							points_vertexLocation_func;
	int							mvpLocation_func;
	int							colorLocation_func;
	// Shader Elements for Colored Facets
	int 						mvpLocation_facet;
	int							mvLocation_facet;
	int 						lightLocation_facet[5];
	int 						colorLocation_facet;
	int 						poly_vertexLocation_facet;
	int 						normalsLocation_facet;
	int							colorLineLocation_facet;
	int							widthLineLocation_facet;
	int							vpLocation_facet;
	int							flagEdgeLocation_facet;

public:

	Scene();
	~Scene();

	/******************* I/O **********************/
	void load(const QString& filename);
	void get_bounding_shape(double& x, double& y, double& z, double& r);

	/******************* OpenGL **********************/
	void init_gl();
	void initialize_buffers();
	void compile_shaders();
	void attrib_buffers(CGAL::QGLViewer* viewer);
	void compute_elements(bool flag_mesh);
	void clear_elements(bool flag_mesh);

	/******************* Visualization **********************/
	// Meshes
	void toggle_view_mesh_facets();
	void toggle_view_mesh_edges();
	void toggle_view_mesh_normals();
	
	void render(CGAL::QGLViewer*);

	/******************* Update **********************/
	void set_mouse_pos(const Point& pos);

	/******************* Clear **********************/
	void clear_data();

	/******************* Options **********************/
	void setInput_point_size(double d);
	void setColor_point_size(double d);

	void setMesh_edge_width(double d);
	void setVector_ratio(double d);
};
 
#endif // _SCENE_H_