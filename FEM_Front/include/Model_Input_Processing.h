#include "Geometry/geometry.h"
#include "Node/node.h"
#include "types.h"

namespace Lotus{
    struct Model
    {
        std::vector<Point> points;
        std::vector<Vector> normals;
        std::vector<int> indices;
    };

    class Model_Input_Processing{
        public:
            typedef Geometry<Node>                  GeometryType;
            typedef GeometryType*                   GeometryPointerType;

            Model_Input_Processing();
            ~Model_Input_Processing();

            void pSetGeometry(GeometryPointerType geometry);
            GeometryPointerType pGetGeometry();

            static bool read_lotus_model(std::string filename, Model& model);
        private:
            GeometryPointerType mp_geometry;
    };
}