#include "../../include/Variable/variable_data.h"

/// Constructor for variables.
Variable_Data::Variable_Data(const std::string& NewName, 
                        std::size_t NewSize) 
:mName(NewName), 
mKey(0), 
mSize(NewSize), 
mpSourceVariable(this), 
mIsComponent(false) 
{
    mKey = GenerateKey(mName, mSize, mIsComponent, 0);
}

/// Constructor.
Variable_Data::Variable_Data(const std::string& NewName, 
                        std::size_t NewSize, 
                        const Variable_Data* pSourceVariable, 
                        char ComponentIndex)
: mName(NewName), 
mKey(0), 
mSize(NewSize), 
mpSourceVariable(pSourceVariable), 
mIsComponent(true) 
{
    mKey = GenerateKey(mpSourceVariable->Name(), mSize, mIsComponent, ComponentIndex);
}

/// Copy constructor
Variable_Data::Variable_Data(const Variable_Data& rOtherVariable)
: mName(rOtherVariable.mName), 
mKey(rOtherVariable.mKey), 
mSize(rOtherVariable.mSize),
mpSourceVariable(rOtherVariable.mpSourceVariable) , 
mIsComponent(rOtherVariable.mIsComponent) 
{}

Variable_Data::KeyType Variable_Data::GenerateKey(const std::string& Name,
                                                std::size_t Size, 
                                                bool IsComponent, 
                                                char ComponentIndex)
{
    std::uint64_t key = Size;
    key <<= 32;
    key += FNV1a32Hash::CalculateHash(Name.c_str());

    key <<= 1;
    key += IsComponent;
    key <<= 7;
    key += ComponentIndex;

    return key;
}

void* Variable_Data::Clone(const void* pSource) const
{
    return 0;
}

void* Variable_Data::Copy(const void* pSource, void* pDestination) const
{
    return 0;
}

void Variable_Data::Assign(const void* pSource, void* pDestination) const {}

void Variable_Data::AssignZero(void* pDestination) const {}

void Variable_Data::Destruct(void* pSource) const {}

void Variable_Data::Delete(void* pSource) const {}

void Variable_Data::Print(const void* pSource, std::ostream& rOStream) const {}

void Variable_Data::PrintData(const void* pSource, std::ostream& rOStream) const {}

void Variable_Data::Allocate(void** pData) const
{
}

/// NOTE: This function is for internal use and not
/// to change arbitrary any variable's key
void Variable_Data::SetKey(KeyType NewKey)
{
    mKey = NewKey;
}

std::string Variable_Data::Info() const
{
    std::stringstream buffer;
    buffer << mName << " variable data" <<" #" << static_cast<unsigned int>(mKey);
    return buffer.str();
}

void Variable_Data::PrintInfo(std::ostream& rOStream) const
{
    rOStream << mName << " variable data";
}

void Variable_Data::PrintData(std::ostream& rOStream) const
{
    rOStream <<" #" << static_cast<unsigned int>(mKey);
}