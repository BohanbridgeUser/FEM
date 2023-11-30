#include "scene.h"

// Qt
#include <QStringList>
#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QPainter>

// OpenGL
#include <QOpenGLShader>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

Scene::Scene()
{
    // options by default
    m_view_mesh_edges = true;
    m_view_mesh_facets = true;
    m_view_mesh_normals = false;

    // default params
    gl_init = false;
}

Scene::~Scene()
{
    m_function.reset();

    for (int i = 0; i < 2; i++)
        vao[i].destroy();

    for (int i = 0; i < 2; i++)
        buffers[i].destroy();
}

/******************* Visualization **********************/

// Meshes
void Scene::toggle_view_mesh_facets() { m_view_mesh_facets = !m_view_mesh_facets; }
void Scene::toggle_view_mesh_edges() { m_view_mesh_edges = !m_view_mesh_edges; }
void Scene::toggle_view_mesh_normals() { m_view_mesh_normals = !m_view_mesh_normals; }

void Scene::render(CGAL::QGLViewer *viewer)
{
    if (!gl_init)
        init_gl();

    if (!are_buffers_initialized)
        initialize_buffers();

    gl->glEnable(GL_DEPTH_TEST);
    QColor color;

    attrib_buffers(viewer);

    if (m_view_mesh_facets && mesh_facets.size() > 0)
    {
        vao[0].bind();
        rendering_program_text.bind();
        color.setRgbF(0.5f, 0.5f, 1.f);
        QColor line_color;
        line_color.setRgbF(0.f, 0.f, 0.f);
        rendering_program_text.setUniformValue(colorLineLocation_text, line_color);
        rendering_program_text.setUniformValue(widthLineLocation_text, m_mesh_options.mesh_edge_width);
        rendering_program_text.setUniformValue(colorLocation_text, color);
        gl->glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh_facets.size() / 3));
        rendering_program_text.release();
        vao[0].release();
    }
}

/******************* I/O **********************/

void Scene::load(const QString &filename)
{
    if (filename.contains(".obj", Qt::CaseInsensitive))
    {
        m_function.load_mesh_from_file(qPrintable(filename));
    }
    else if(filename.contains(".lotus", Qt::CaseInsensitive))
    {
        m_function.load_lotus_from_file(qPrintable(filename));
    }
    else
        std::cout << "Format not supported!" << std::endl;
    compute_elements(true);
}

void Scene::get_bounding_shape(double &x, double &y, double &z, double &r)
{
    // do something
    x = 0.;
    y = 0.;
    z = 0.;
    r = 1.;
}

/******************* Update **********************/
void Scene::set_mouse_pos(const Point &pos) { m_mouse_pos = pos; }

/******************* OpenGL **********************/
void Scene::init_gl()
{
    gl = new QOpenGLFunctions();
    gl->initializeOpenGLFunctions();

    compile_shaders();
    gl_init = true;
}

void Scene::compile_shaders()
{
    for (int i = 0; i < 2; i++)
        buffers[i].create();

    for (int i = 0; i < 2; i++)
        vao[i].create();

    //----------- Point / Vector Rendering Program -------------//
    QOpenGLShader *vertex_shader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vertex_shader->compileSourceFile(shader_path + "point.vs"))
        std::cerr << "Compiling vertex source failed" << std::endl;
    QOpenGLShader *fragment_shader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fragment_shader->compileSourceFile(shader_path + "point.fs"))
        std::cerr << "Compiling fragment source FAILED" << std::endl;

    if (!rendering_program.addShader(vertex_shader))
        std::cerr << "Adding vertex shader failed" << std::endl;
    if (!rendering_program.addShader(fragment_shader))
        std::cerr << "adding fragment shader FAILED" << std::endl;

    if (!rendering_program.link())
        std::cerr << "linking Program FAILED" << std::endl;

    rendering_program.bind();

    //----------- Facet Rendering Program -------------//
    QOpenGLShader *vertex_shader_facet = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vertex_shader_facet->compileSourceFile(shader_path + "facet.vs"))
        std::cerr << "Compiling vertex source facet failed" << std::endl;
    QOpenGLShader *geometry_shader_facet = new QOpenGLShader(QOpenGLShader::Geometry);
    if (!geometry_shader_facet->compileSourceFile(shader_path + "facet.gs"))
        std::cerr << "Compiling geometry source facet failed" << std::endl;
    QOpenGLShader *fragment_shader_facet = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fragment_shader_facet->compileSourceFile(shader_path + "facet.fs"))
        std::cerr << "Compiling fragment source facet FAILED" << std::endl;

    if (!rendering_program_text.addShader(vertex_shader_facet))
        std::cerr << "Adding vertex shader facet failed" << std::endl;
    if (!rendering_program_text.addShader(geometry_shader_facet))
        std::cerr << "Adding geometry shader facet failed" << std::endl;
    if (!rendering_program_text.addShader(fragment_shader_facet))
        std::cerr << "adding fragment shader facet FAILED" << std::endl;

    if (!rendering_program_text.link())
        std::cerr << "linking facet Program FAILED" << std::endl;

    rendering_program_text.bind();

    //----------- Function Rendering Program -------------//
    QOpenGLShader *vertex_shader_func = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vertex_shader_func->compileSourceFile(shader_path + "func.vs"))
        std::cerr << "Compiling vertex source failed" << std::endl;
    QOpenGLShader *fragment_shader_func = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fragment_shader_func->compileSourceFile(shader_path + "func.fs"))
        std::cerr << "Compiling fragment source FAILED" << std::endl;

    if (!rendering_program_func.addShader(vertex_shader_func))
        std::cerr << "Adding vertex shader failed" << std::endl;
    if (!rendering_program_func.addShader(fragment_shader_func))
        std::cerr << "adding fragment shader FAILED" << std::endl;

    if (!rendering_program_func.link())
        std::cerr << "linking Program FAILED" << std::endl;

    rendering_program_func.bind();

    //----------- Colored Facet Rendering Program -------------//
    QOpenGLShader *vertex_shader_facet_color = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!vertex_shader_facet_color->compileSourceFile(shader_path + "facet_color.vs"))
        std::cerr << "Compiling vertex source facet color failed" << std::endl;
    QOpenGLShader *geometry_shader_facet_color = new QOpenGLShader(QOpenGLShader::Geometry);
    if (!geometry_shader_facet_color->compileSourceFile(shader_path + "facet_color.gs"))
        std::cerr << "Compiling geometry source facet color failed" << std::endl;
    QOpenGLShader *fragment_shader_facet_color = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!fragment_shader_facet_color->compileSourceFile(shader_path + "facet_color.fs"))
        std::cerr << "Compiling fragment source facet color FAILED" << std::endl;

    if (!rendering_program_facet.addShader(vertex_shader_facet_color))
        std::cerr << "Adding vertex shader facet color failed" << std::endl;
    if (!rendering_program_facet.addShader(geometry_shader_facet_color))
        std::cerr << "Adding geometry shader facet color failed" << std::endl;
    if (!rendering_program_facet.addShader(fragment_shader_facet_color))
        std::cerr << "adding fragment shader facet color FAILED" << std::endl;

    if (!rendering_program_facet.link())
        std::cerr << "linking facet color Program FAILED" << std::endl;

    rendering_program_facet.bind();
}

void Scene::initialize_buffers()
{
    // Mesh Surface
    vao[0].bind();
    buffers[0].bind();
    buffers[0].allocate(mesh_facets.data(),
                        static_cast<int>(mesh_facets.size() * sizeof(float)));
    poly_vertexLocation_text = rendering_program_text.attributeLocation("vertex");
    rendering_program_text.bind();
    rendering_program_text.enableAttributeArray(poly_vertexLocation_text);
    rendering_program_text.setAttributeBuffer(poly_vertexLocation_text, GL_FLOAT, 0, 3);
    buffers[0].release();

    buffers[1].bind();
    buffers[1].allocate(mesh_normals.data(),
                        static_cast<int>(mesh_normals.size() * sizeof(float)));
    normalsLocation_text = rendering_program_text.attributeLocation("normal");
    rendering_program_text.bind();
    rendering_program_text.enableAttributeArray(normalsLocation_text);
    rendering_program_text.setAttributeBuffer(normalsLocation_text, GL_FLOAT, 0, 3);
    buffers[1].release();

    rendering_program_text.release();
    vao[0].release();

    are_buffers_initialized = true;
}

void Scene::attrib_buffers(CGAL::QGLViewer *viewer)
{
    QMatrix4x4 mvpMatrix;
    QMatrix4x4 mvMatrix;
    QMatrix4x4 vpMatrix;
    double mat[16];
    viewer->camera()->getModelViewProjectionMatrix(mat);
    for (int i = 0; i < 16; i++)
        mvpMatrix.data()[i] = (float)mat[i];
    viewer->camera()->getModelViewMatrix(mat);
    for (int i = 0; i < 16; i++)
        mvMatrix.data()[i] = (float)mat[i];
    int mat1[4];
    viewer->camera()->getViewport(mat1);
    vpMatrix.fill(0.);
    vpMatrix.data()[0] = (float)mat1[2] * 0.5;
    vpMatrix.data()[3] = (float)mat1[2] * 0.5 + mat1[0];
    vpMatrix.data()[5] = (float)mat1[3] * 0.5;
    vpMatrix.data()[7] = (float)mat1[3] * 0.5 + mat1[1];
    vpMatrix.data()[10] = 0.5;
    vpMatrix.data()[11] = 0.5;
    vpMatrix.data()[15] = 1.;

    // Point / Vector Rendering
    rendering_program.bind();
    mvpLocation = rendering_program.uniformLocation("mvp_matrix");
    colorLocation = rendering_program.uniformLocation("color");
    rendering_program.setUniformValue(mvpLocation, mvpMatrix);
    rendering_program.release();

    // Facet Rendering
    QVector4D ambient(0.25f, 0.20725f, 0.20725f, 0.922f);
    QVector4D diffuse(1.0f, 0.829f, 0.829f, 0.922f);
    QVector4D specular(0.6f, 0.6f, 0.6f, 1.0f);
    QVector4D position(0.0f, 0.0f, 3.0f, 1.0f);
    GLfloat shininess = 11.264f;

    rendering_program_text.bind();
    mvpLocation_text = rendering_program_text.uniformLocation("mvp_matrix");
    mvLocation_text = rendering_program_text.uniformLocation("mv_matrix");
    colorLocation_text = rendering_program_text.uniformLocation("color");
    colorLineLocation_text = rendering_program_text.uniformLocation("line_color");
    widthLineLocation_text = rendering_program_text.uniformLocation("line_width");
    vpLocation_text = rendering_program_text.uniformLocation("ViewportMatrix");
    flagEdgeLocation_text = rendering_program_text.uniformLocation("flag_edge");

    lightLocation_text[0] = rendering_program_text.uniformLocation("light_pos");
    lightLocation_text[1] = rendering_program_text.uniformLocation("light_diff");
    lightLocation_text[2] = rendering_program_text.uniformLocation("light_spec");
    lightLocation_text[3] = rendering_program_text.uniformLocation("light_amb");
    lightLocation_text[4] = rendering_program_text.uniformLocation("spec_power");

    rendering_program_text.setUniformValue(lightLocation_text[0], position);
    rendering_program_text.setUniformValue(lightLocation_text[1], diffuse);
    rendering_program_text.setUniformValue(lightLocation_text[2], specular);
    rendering_program_text.setUniformValue(lightLocation_text[3], ambient);
    rendering_program_text.setUniformValue(lightLocation_text[4], shininess);
    rendering_program_text.setUniformValue(mvpLocation_text, mvpMatrix);
    rendering_program_text.setUniformValue(mvLocation_text, mvMatrix);
    rendering_program_text.setUniformValue(vpLocation_text, vpMatrix);
    rendering_program_text.setUniformValue(flagEdgeLocation_text, static_cast<int>(m_view_mesh_edges));
    rendering_program_text.release();

    // Facet Color Rendering
    rendering_program_facet.bind();
    mvpLocation_facet = rendering_program_facet.uniformLocation("mvp_matrix");
    mvLocation_facet = rendering_program_facet.uniformLocation("mv_matrix");
    colorLineLocation_facet = rendering_program_facet.uniformLocation("line_color");
    widthLineLocation_facet = rendering_program_facet.uniformLocation("line_width");
    vpLocation_facet = rendering_program_facet.uniformLocation("ViewportMatrix");
    flagEdgeLocation_facet = rendering_program_facet.uniformLocation("flag_edge");

    lightLocation_facet[0] = rendering_program_facet.uniformLocation("light_pos");
    lightLocation_facet[1] = rendering_program_facet.uniformLocation("light_diff");
    lightLocation_facet[2] = rendering_program_facet.uniformLocation("light_spec");
    lightLocation_facet[3] = rendering_program_facet.uniformLocation("light_amb");
    lightLocation_facet[4] = rendering_program_facet.uniformLocation("spec_power");

    rendering_program_facet.setUniformValue(lightLocation_facet[0], position);
    rendering_program_facet.setUniformValue(lightLocation_facet[1], diffuse);
    rendering_program_facet.setUniformValue(lightLocation_facet[2], specular);
    rendering_program_facet.setUniformValue(lightLocation_facet[3], ambient);
    rendering_program_facet.setUniformValue(lightLocation_facet[4], shininess);
    rendering_program_facet.setUniformValue(mvpLocation_facet, mvpMatrix);
    rendering_program_facet.setUniformValue(mvLocation_facet, mvMatrix);
    rendering_program_facet.setUniformValue(vpLocation_facet, vpMatrix);
    rendering_program_facet.setUniformValue(flagEdgeLocation_facet, static_cast<int>(m_view_mesh_edges));
    rendering_program_facet.release();

    // Func Rendering
    rendering_program_func.bind();
    mvpLocation_func = rendering_program_func.uniformLocation("mvp_matrix");
    rendering_program_func.setUniformValue(mvpLocation_func, mvpMatrix);
    rendering_program_func.release();
}

void Scene::compute_elements(bool flag_mesh)
{
    clear_elements(flag_mesh);

    if (flag_mesh)
    {
        m_function.update_mesh(mesh_facets, mesh_normals);
    }

    are_buffers_initialized = false;
}

void Scene::clear_elements(bool flag_mesh)
{
    if (flag_mesh)
    {
        mesh_facets.clear();
        mesh_normals.clear();
    }
}

void Scene::clear_data()
{
}

/******************* Options **********************/

void Scene::setInput_point_size(double d)
{
    m_mesh_options.input_point_size = (float)d;
}

void Scene::setColor_point_size(double d)
{
    m_mesh_options.color_point_size = (float)d;
}

void Scene::setMesh_edge_width(double d)
{
    m_mesh_options.mesh_edge_width = (float)d;
}

void Scene::setVector_ratio(double d)
{
    m_mesh_options.vector_ratio = (float)d;
}
