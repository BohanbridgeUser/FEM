#ifndef _MODEL_H_
#define _MODEL_H_

#include "../define.h"
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_dimension.h"
#include "../Geometry/Point.h"
#include "../node.h"
#include "../Container/points_container.h"
#include "../Container/geometry_container.h"
#include "model_part.h"
#include <string>
#include <unordered_map>
#include <memory>

class Model
{
    public:
        ///@name Define 
        ///@{ 
        LOTUS_POINTER_DEFINE(Model)
        typedef Node NodeType;
        typedef Geometry<Dimension<3>> GeometryType;
        typedef Model_Part ModelPart;
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            Model()
            {

            }
            ~Model()
            {

            }
        ///@}

        ///@name Operations
        ///@{
            Model_Part& CreateModelPart(std::string& PartName);

            void Reset();

            Model_Part& GetModelPart(std::string& PartName);

            void DeleteModelPart(std::string& PartName);
        ///@}
    private:
        std::string Name;
        std::unordered_map< std::string, Model_Part::SharedPointer > mRootModelPartMap;
};
#endif
