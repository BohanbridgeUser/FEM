#ifndef _ACCESSOR_H_
#define _ACCESSOR_H_

#include "../Geometry/geometry.h"
#include "../process_info.h"
#include "../Node/node.h"

class Properties;

/**
 * @class Accessor
 * @brief This class defines the way a certain property is accessed.
 */
class Accessor
{
public:
    ///@name Type Definitions
    ///@{

    /// Geometry type definition
    typedef Geometry<Node> 
                                                     GeometryType;
    typedef Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic>
                                                           Matrix;
    typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                           Vector;

    /// Pointer definition of Accessor
    LOTUS_POINTER_DEFINE(Accessor);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor
    Accessor() = default;

    /// Destructor.
    virtual ~Accessor() = default;

    /// Copy constructor
    Accessor(const Accessor& rOther) {}

    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief Custom method to retrieve double type properties
     * @param rVariable The variable considered (double type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The double type properties
     */
    virtual double GetValue(
        const Variable<double>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve Vector type properties
     * @param rVariable The variable considered (Vector type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The Vector type properties
     */
    virtual Vector GetValue(
        const Variable<Vector>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve bool type properties
     * @param rVariable The variable considered (bool type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The bool type properties
     */
    virtual bool GetValue(
        const Variable<bool>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve int type properties
     * @param rVariable The variable considered (int type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The int type properties
     */
    virtual int GetValue(
        const Variable<int>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve Matrix type properties
     * @param rVariable The variable considered (Matrix type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The Matrix type properties
     */
    virtual Matrix GetValue(
        const Variable<Matrix>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve std::array<double, 3 > type properties
     * @param rVariable The variable considered (std::array<double, 3 > type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The std::array<double, 3 > type properties
     */
    virtual std::array<double, 3> GetValue(
        const Variable<std::array<double, 3>>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve std::array<double, 6 > type properties
     * @param rVariable The variable considered (std::array<double, 6 > type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The std::array<double, 6 > type properties
     */
    virtual std::array<double, 6> GetValue(
        const Variable<std::array<double, 6>>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve std::array<double, 6 > type properties
     * @param rVariable The variable considered (std::array<double, 6 > type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The std::array<double, 6 > type properties
     */
    virtual std::array<double, 4> GetValue(
        const Variable<std::array<double, 4>>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve std::array<double, 6 > type properties
     * @param rVariable The variable considered (std::array<double, 6 > type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The std::array<double, 6 > type properties
     */
    virtual std::array<double, 9> GetValue(
        const Variable<std::array<double, 9>>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    /**
     * @brief Custom method to retrieve string type properties
     * @param rVariable The variable considered (string type properties)
     * @param rProperties The properties considered
     * @param rGeometry The geometry considered
     * @param rShapeFunctionVector The shape function of the GP considered
     * @param rProcessInfo The process info considered
     * @return The string type properties
     */
    virtual std::string GetValue(
        const Variable<std::string>& rVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const Process_Info& rProcessInfo
        ) const;

    // Getting a pointer to the class
    virtual Accessor::UniquePointer Clone() const;

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        std::stringstream buffer;
        buffer << "Accessor" ;

        return buffer.str();
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const  {rOStream << "Accessor";}

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const {rOStream << "virtual method of the base Accessor class";}

    ///@}

private:

    ///@name Member Variables
    ///@{

};
#endif