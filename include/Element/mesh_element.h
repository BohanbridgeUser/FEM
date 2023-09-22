#pragma once

// System includes

// External includes

// Project includes
#include "element.h"

/**
 * @class MeshElement
 * @brief This is pure geometric element. The only purpose for this definition is to create dummy elements
 * @details Inherits all method from base element, and overrides the Clone() and Create() methods
 */
class MeshElement
:public Element
{
public:

    ///@name Type Definitions
    ///@{

    /// We define the base class Element
    typedef Element BaseType;

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
    /// Counted pointer of MeshElement
    LOTUS_SHARED_POINTER_DEFINE(MeshElement);

    ///@}

public:

    ///@name Life Cycle
    ///@{

    /**
     * @brief Constructor.
     * @param NewId The Id of the new created element
     */
    MeshElement(IndexType NewId = 0);

    /**
     * @brief Constructor using an array of nodes
     * @param NewId The Id of the new created element
     * @param rThisNodes The array of nodes that will define the geometry that will define the element
     */
    MeshElement(
        IndexType NewId,
        const NodesArrayType& rThisNodes
        );

    /**
     * @brief Constructor using Geometry
     * @param NewId The Id of the new created element
     * @param pGeometry The pointer to the geometry that will define the element
     */
    MeshElement(
        IndexType NewId,
        GeometryType::Pointer pGeometry
        );

    /**
     * @brief Constructor using Properties
     * @param NewId The Id of the new created element
     * @param pGeometry The pointer to the geometry that will define the element
     * @param pProperties The pointer to the properties that will define the behaviour of the element
     */
    MeshElement(
        IndexType NewId,
        GeometryType::Pointer pGeometry,
        PropertiesType::Pointer pProperties
        );

    ///Copy constructor
    MeshElement(MeshElement const& rOther);

    /// Destructor.
    ~MeshElement() override;

    ///@}
    ///@name Operators
    ///@{

    /// Assignment operator.
    MeshElement& operator=(MeshElement const& rOther);

    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief Creates a new element pointer
     * @param NewId the ID of the new element
     * @param ThisNodes the nodes of the new element
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(
        IndexType NewId,
        NodesArrayType const& ThisNodes,
        PropertiesType::Pointer pProperties
        ) const override;

    /**
     * @brief Creates a new element pointer
     * @param NewId the ID of the new element
     * @param pGeom the geometry to be employed
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(
        IndexType NewId,
        GeometryType::Pointer pGeom,
        PropertiesType::Pointer pProperties
        ) const override;

    /**
     * @brief Creates a new element pointer and clones the previous element data
     * @param NewId the ID of the new element
     * @param ThisNodes the nodes of the new element
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Clone (
        IndexType NewId,
        NodesArrayType const& ThisNodes
        ) const override;

    // /**
    //  * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the double version)
    //  * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH AN ELEMENT IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
    //  * @param rRHSVector input variable containing the RHS vector to be assembled
    //  * @param rRHSVariable variable describing the type of the RHS vector to be assembled
    //  * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void AddExplicitContribution(
    //     const VectorType& rRHSVector,
    //     const Variable<VectorType>& rRHSVariable,
    //     const Variable<double >& rDestinationVariable,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the vector version)
    //  * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH AN ELEMENT IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
    //  * @param rRHSVector input variable containing the RHS vector to be assembled
    //  * @param rRHSVariable variable describing the type of the RHS vector to be assembled
    //  * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void AddExplicitContribution(
    //     const VectorType& rRHS,
    //     const Variable<VectorType>& rRHSVariable,
    //     const Variable<std::array<double,3> >& rDestinationVariable,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief This function is designed to make the element to assemble an rRHS vector identified by a variable rRHSVariable by assembling it to the nodes on the variable rDestinationVariable. (This is the matrix version)
    //  * @details The "AddExplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH AN ELEMENT IS ALLOWED TO WRITE ON ITS NODES. The caller is expected to ensure thread safety hence SET-/UNSET-LOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
    //  * @param rRHSVector input variable containing the RHS vector to be assembled
    //  * @param rRHSVariable variable describing the type of the RHS vector to be assembled
    //  * @param rDestinationVariable variable in the database to which the rRHSvector will be assembled
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void AddExplicitContribution(
    //     const MatrixType& rLHSMatrix,
    //     const Variable<MatrixType>& rLHSVariable,
    //     const Variable<MatrixType>& rDestinationVariable,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    //  void CalculateOnIntegrationPoints(
    //     const Variable<bool>& rVariable,
    //     std::vector<bool>& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<int>& rVariable,
    //     std::vector<int>& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<double>& rVariable,
    //     std::vector<double>& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<std::array<double, 3 > >& rVariable,
    //     std::vector< std::array<double, 3 > >& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<std::array<double, 6 > >& rVariable,
    //     std::vector< std::array<double, 6 > >& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<VectorType >& rVariable,
    //     std::vector< VectorType >& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<Matrix >& rVariable,
    //     std::vector< Matrix >& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    // /**
    //  * @brief Calculate a boolean Variable on the Element integration points
    //  * @param rVariable The variable we want to get
    //  * @param rOutput The values obtained in the integration points
    //  * @param rCurrentProcessInfo the current process info instance
    //  */
    // void CalculateOnIntegrationPoints(
    //     const Variable<Constitutive_Law::Pointer>& rVariable,
    //     std::vector<Constitutive_Law::Pointer>& rOutput,
    //     const Process_Info& rCurrentProcessInfo
    //     ) override;

    ///@}
    ///@name Input and output
    ///@{


    /// Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "Geometrical Element #" << Id();
        return buffer.str();
    }

    /// Print information about this object.

    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "Geometrical  Element #" << Id();
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

}; // Class MeshElement

