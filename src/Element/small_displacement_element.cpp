#include "../../include/Element/small_displacement_element.h"

/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    Small_Displacement_Element::Small_Displacement_Element()
    :Solid_Element()
    {
    }
    Small_Displacement_Element::Small_Displacement_Element(IndexType NewId,
                                                        GeometryType::Pointer pThisGeometry)
    :Solid_Element(NewId,pThisGeometry)
    {

    }
    Small_Displacement_Element::Small_Displacement_Element(IndexType NewId,
                                                        GeometryType::Pointer pThisGeometry,
                                                        Properties::Pointer pThisProperties)
    :Solid_Element(NewId,pThisGeometry,pThisProperties)
    {

    }
    Small_Displacement_Element::Small_Displacement_Element(Small_Displacement_Element const& another)
    :Solid_Element(another)
    {
    }
    Small_Displacement_Element::~Small_Displacement_Element()
    {
    }

/// @}


/// @name Operators
/// @{
    Small_Displacement_Element& Small_Displacement_Element::operator=(Small_Displacement_Element const& another)
    {
        Solid_Element::operator=(another);
        return *this;
    }

/// @}


/// @name Operations
/// @{
    Element::SharedPointer Small_Displacement_Element::Create(IndexType NewId,
                                                              GeometryType::Pointer rNodes,
                                                              Properties::Pointer pProperties) const
    {
        return std::make_shared<Small_Displacement_Element>(NewId,&(*GetGeometry().Create(rNodes)),pProperties);
    }
    Element::SharedPointer Small_Displacement_Element::Clone(IndexType NewId,
                                                             NodesContainerType const& rNodes) const
    {
        return std::make_shared<Small_Displacement_Element>(NewId,&(*GetGeometry().Create(rNodes)),pGetProperties());
    }
    int Small_Displacement_Element::Check(Process_Info const& rCurrentProcessInfo) const
    {
        
    }

/// @}


/// @name Access
/// @{


/// @}


/// @name Inquiry
/// @{


/// @}


/// @brief protected:
/// @name Protected Static Member Variables
/// @{


/// @}


/// @name Protected Member Variables
/// @{


/// @}


/// @name Protected Operatiors
/// @{


/// @}


/// @name Protected Operations
/// @{


/// @}


/// @name Protected Access
/// @{


/// @}


/// @name Protected Inquiry
/// @{


/// @}


/// @brief private:
/// @name Private Static Member Variables
/// @{


/// @}


/// @name Private Member Variables
/// @{


/// @}


/// @name Private Operatiors
/// @{


/// @}


/// @name Private Operations
/// @{


/// @}


/// @name Private Access
/// @{


/// @}


/// @name Private Inquiry
/// @{


/// @}


