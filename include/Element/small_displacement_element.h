#ifndef _SMALL_DISPLACEMENT_ELEMENT_H_
#define _SMALL_DISPLACEMENT_ELEMENT_H_
#include "solid_element.h"

class Small_Displacement_Element : public Solid_Element
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Small_Displacement_Element)

            typedef GeometryType::SizeType
                                                                            SizeType;
            typedef Constitutive_Law
                                                                 ConstitutiveLawType;
            typedef ConstitutiveLawType::Pointer
                                                          ConstitutiveLawPointerType;
            typedef ConstitutiveLawType::StressMeasure
                                                                   StressMeasureType;
            
        /// @}


        /// @name Life Circle
        /// @{


        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{


        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


        /// @}


    protected:
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


    private:
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
};


#endif