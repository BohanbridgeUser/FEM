#include "../../include/Property/accessor.h"

double Accessor::GetValue(
    const Variable<double>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    exit(0);
    return 0.0;
}

/***********************************************************************************/
/***********************************************************************************/

Accessor::Vector Accessor::GetValue(
    const Variable<Vector>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    exit(0);
    return Vector();
}

/***********************************************************************************/
/***********************************************************************************/

bool Accessor::GetValue(
    const Variable<bool>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return false;
}

/***********************************************************************************/
/***********************************************************************************/

int Accessor::GetValue(
    const Variable<int>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return 0;
}

/***********************************************************************************/
/***********************************************************************************/

Accessor::Matrix Accessor::GetValue(
    const Variable<Matrix>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return Matrix();
}

/***********************************************************************************/
/***********************************************************************************/

std::array<double, 3> Accessor::GetValue(
    const Variable<std::array<double, 3>>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return std::array<double, 3>{};
}

/***********************************************************************************/
/***********************************************************************************/

std::array<double, 6> Accessor::GetValue(
    const Variable<std::array<double, 6>>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return std::array<double, 6>{};
}

/***********************************************************************************/
/***********************************************************************************/

std::array<double, 4> Accessor::GetValue(
    const Variable<std::array<double, 4>>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return std::array<double, 4>{};
}

/***********************************************************************************/
/***********************************************************************************/

std::array<double, 9> Accessor::GetValue(
    const Variable<std::array<double, 9>>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return std::array<double, 9>{};
}

/***********************************************************************************/
/***********************************************************************************/

std::string Accessor::GetValue(
    const Variable<std::string>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const Process_Info& rProcessInfo
    ) const
{
    std::cerr << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return "string";
}

/***********************************************************************************/
/***********************************************************************************/

Accessor::UniquePointer Accessor::Clone() const
{
    return std::make_unique<Accessor>(*this);
}

