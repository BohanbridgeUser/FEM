//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

#pragma once

// System includes

// External includes

// Project includes
#include "condition.h"

/**
 * @class MeshCondition
 * @brief This is pure geometric condition. The only purpose for this definition is to create dummy conditions
 * @details Inherits all method from base condition, and overrides the Clone() and Create() methods
 */
class MeshCondition
    : public Condition
{
public:

    ///@name Type Definitions
    ///@{

    /// We define the base class Condition
    typedef Condition BaseType;

    /// Dfinition of the index type
    typedef BaseType::IndexType IndexType;

    /// Definition of the size type
    typedef BaseType::SizeType SizeType;

    /// Definition of the node type
    typedef BaseType::NodeType NodeType;

    /// Definition of the properties type
    typedef BaseType::PropertiesType PropertiesType;

    /// Definition of the geometry type with given NodeType
    typedef BaseType::GeometryType GeometryType;

    /// Definition of nodes container type, redefined from GeometryType
    typedef BaseType::NodesContainerType NodesArrayType;

    typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                MatrixType;
    typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                    Matrix;
    typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                    Vector;
    typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                VectorType;      
    /// Counted pointer of MeshCondition
    LOTUS_SHARED_POINTER_DEFINE(MeshCondition);

    ///@}

public:

    ///@name Life Cycle
    ///@{

    /**
     * @brief Constructor.
     * @param NewId The Id of the new created condition
     */
    MeshCondition(IndexType NewId = 0);

    /**
     * @brief Constructor using an array of nodes
     * @param NewId The Id of the new created condition
     * @param rThisNodes The array of nodes that will define the geometry that will define the condition
     */
    MeshCondition(
        IndexType NewId,
        const NodesArrayType& rThisNodes
        );

    /**
     * @brief Constructor using Geometry
     * @param NewId The Id of the new created condition
     * @param pGeometry The pointer to the geometry that will define the condition
     */
    MeshCondition(
        IndexType NewId,
        GeometryType::Pointer pGeometry
        );

    /**
     * @brief Constructor using Properties
     * @param NewId The Id of the new created condition
     * @param pGeometry The pointer to the geometry that will define the condition
     * @param pProperties The pointer to the properties that will define the behaviour of the condition
     */
    MeshCondition(
        IndexType NewId,
        GeometryType::Pointer pGeometry,
        PropertiesType::Pointer pProperties
        );

    ///Copy constructor
    MeshCondition(MeshCondition const& rOther);

    /// Destructor.
    ~MeshCondition() override;

    ///@}
    ///@name Operators
    ///@{

    /// Assignment operator.
    MeshCondition& operator=(MeshCondition const& rOther);

    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief Creates a new condition pointer
     * @param NewId the ID of the new condition
     * @param ThisNodes the nodes of the new condition
     * @param pProperties the properties assigned to the new condition
     * @return a Pointer to the new condition
     */
    Condition::Pointer Create(
        IndexType NewId,
        NodesArrayType const& ThisNodes,
        PropertiesType::Pointer pProperties
        ) const override;

    /**
     * @brief Creates a new condition pointer
     * @param NewId the ID of the new condition
     * @param pGeom the geometry to be employed
     * @param pProperties the properties assigned to the new condition
     * @return a Pointer to the new condition
     */
    Condition::Pointer Create(
        IndexType NewId,
        GeometryType::Pointer pGeom,
        PropertiesType::Pointer pProperties
        ) const override;

    /**
     * @brief Creates a new condition pointer and clones the previous condition data
     * @param NewId the ID of the new condition
     * @param ThisNodes the nodes of the new condition
     * @param pProperties the properties assigned to the new condition
     * @return a Pointer to the new condition
     */
    Condition::Pointer Clone (
        IndexType NewId,
        NodesArrayType const& ThisNodes
        ) const override;

    /**
     * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the double version)
     * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
     * @param rRHSVector input variable containing the RHS vector to be assembled
     * @param rRHSVariable variable describing the type of the RHS vector to be assembled
     * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
     * @param rCurrentProcessInfo the current process info instance
     */
    void AddExplicitContribution(
        const VectorType& rRHSVector,
        const Variable<VectorType>& rRHSVariable,
        const Variable<double >& rDestinationVariable,
        const Process_Info& rCurrentProcessInfo
        ) override;

    /**
     * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the vector version)
     * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
     * @param rRHSVector input variable containing the RHS vector to be assembled
     * @param rRHSVariable variable describing the type of the RHS vector to be assembled
     * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
     * @param rCurrentProcessInfo the current process info instance
     */
    void AddExplicitContribution(
        const VectorType& rRHS,
        const Variable<VectorType>& rRHSVariable,
        const Variable<std::array<double,3> >& rDestinationVariable,
        const Process_Info& rCurrentProcessInfo
        ) override;

    /**
     * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the matrix version)
     * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH A CONDITION IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
     * @param rRHSVector input variable containing the RHS vector to be assembled
     * @param rRHSVariable variable describing the type of the RHS vector to be assembled
     * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
     * @param rCurrentProcessInfo the current process info instance
     */
    void AddExplicitContribution(
        const MatrixType& rLHSMatrix,
        const Variable<MatrixType>& rLHSVariable,
        const Variable<Matrix>& rDestinationVariable,
        const Process_Info& rCurrentProcessInfo
        ) override;

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "Geometrical Condition #" << Id();
        return buffer.str();
    }

    /// Print information about this object.

    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "Geometrical Condition #" << Id();
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
        GetGeometry().PrintData(rOStream);
    }

    ///@}

private:

    ///@name Serialization
    ///@{

    ///@}

}; // Class MeshCondition
