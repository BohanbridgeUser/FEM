#include "Model_Input_Processing.h"

namespace Lotus{
    Model_Input_Processing::Model_Input_Processing():mp_geometry(nullptr){}

    Model_Input_Processing::~Model_Input_Processing()
    {
        if(mp_geometry != nullptr)
            delete mp_geometry;
    }

    void Model_Input_Processing::pSetGeometry(GeometryPointerType geometry)
    {
        mp_geometry = geometry;
    }

    typename Model_Input_Processing::GeometryPointerType Model_Input_Processing::pGetGeometry()
    {
        return mp_geometry;
    }

    bool Model_Input_Processing::read_lotus_model(std::string filename, Model& model)
    {
        return true;
    }
}