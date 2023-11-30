#ifndef M_FUNCTION_H
#define M_FUNCTION_H

// #include <STEPControl_Reader.hxx>
// #include <TopoDS_Shape.hxx>
// #include <BRepTools.hxx>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/compute_normal.h>

#include "types.h"
#include "Model_Input_Processing.h"

namespace Lotus
{
    class Function
    {
    private:
        bool m_init;
        Polyhedron m_mesh;
        Model      m_model;

    public:
        Function() : m_init(false) {}

        ~Function()
        {
            reset();
        }

        void reset()
        {
            m_init = false;
        }

        bool load_mesh_from_file(std::string filename)
        {
            reset();

            if (!CGAL::Polygon_mesh_processing::IO::read_polygon_mesh(filename, m_mesh))
            {
                std::cout << "  Invalid file: " << filename << "." << std::endl;
                return false;
            }

            std::cout << "  Load mesh with " << m_mesh.size_of_vertices() << " vertices and " << m_mesh.size_of_facets() << " faces." << std::endl;
            m_init = true;

            return true;
        }

        bool load_lotus_from_file(std::string filename)
        {
            reset();

            if(!Lotus::Model_Input_Processing::read_lotus_model(filename, m_model))
            {
                std::cout << "  Invalid file: " << filename << "." << std::endl;
                return false;
            }

            std::cout << "  Load mesh with " << m_mesh.size_of_vertices() << " vertices and " << m_mesh.size_of_facets() << " faces." << std::endl;
            m_init = true;
            return true;
        }

        void update_mesh(DataList &mesh_facets, DataList &mesh_normals)
        {
            if (m_mesh.size_of_vertices() == 0)
                return;

            mesh_facets.reserve(m_mesh.size_of_facets() * 9);
            mesh_normals.reserve(m_mesh.size_of_facets() * 9);

            for (Facet_iterator face = m_mesh.facets_begin(); face != m_mesh.facets_end(); ++face)
            {
                Halfedge_facet_circulator he = face->facet_begin();
                Vector face_normal = CGAL::Polygon_mesh_processing::compute_face_normal(face, m_mesh);
                do
                {
                    Point p = he->vertex()->point();
                    mesh_facets.push_back((float)p.x());
                    mesh_facets.push_back((float)p.y());
                    mesh_facets.push_back((float)p.z());
                    mesh_normals.push_back((float)face_normal.x());
                    mesh_normals.push_back((float)face_normal.y());
                    mesh_normals.push_back((float)face_normal.z());
                } while (++he != face->facet_begin());
            }
        }
    }; // end of class Function

} // namespace Lotus

#endif