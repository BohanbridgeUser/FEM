#ifndef _TYPES_
#define _TYPES_

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_data_structure_3.h>
#include <CGAL/Polyhedron_3.h>

// std
#include <vector>
#include <unordered_set>

namespace Lotus
{
    /* 数字结构 */
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

    /* 数据结构 */
    typedef Kernel::FT FT;
    typedef Kernel::Point_3 Point;
    typedef Kernel::Vector_3 Vector;
    typedef Kernel::Triangle_3 Triangle;

    typedef std::pair<Point, Vector> Point_with_normal;
    typedef std::vector<Point_with_normal> PwnList;
    typedef std::vector<Point> PointList;
    typedef std::vector<float> DataList;
    typedef std::vector<int> IntList;
    typedef std::vector<double> DoubleList;

    // Polyhedron
    typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
    typedef Polyhedron::Facet_iterator Facet_iterator;
    typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;

} // namespace cad2mesh

#endif