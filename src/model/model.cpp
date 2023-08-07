#include "../../include/Model/model.h"

Model_Part& Model::CreateModelPart(std::string& PartName)
{
    Model_Part::SharedPointer p_model_part(new Model_Part(PartName,*this));
    if (mRootModelPartMap.find("root") == mRootModelPartMap.end()) 
    {
        Model_Part::SharedPointer p_model_part_temp(new Model_Part(std::string("root"),*this));
        mRootModelPartMap.insert({"root", p_model_part_temp});
    }
    mRootModelPartMap.insert({PartName, p_model_part});
    return *(mRootModelPartMap[PartName].get());
}

void Model::Reset()
{
    mRootModelPartMap.clear();
}

void Model::DeleteModelPart(std::string& PartName)
{
    if (mRootModelPartMap.find(PartName) == mRootModelPartMap.end()) return;
    else mRootModelPartMap.erase(PartName);
}

Model_Part& Model::GetModelPart(std::string& PartName)
{
    auto temp = mRootModelPartMap.find(PartName);
    if (temp == mRootModelPartMap.end()) {
        std::cout << "No Model Part:" << PartName << std::endl;
        exit(0);
    } else {
        return *(temp->second);
    }
}