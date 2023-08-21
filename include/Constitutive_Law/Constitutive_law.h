#ifndef _CONSTITUTIVE_LAW_H_
#define _CONSTITUTIVE_LAW_H_
#include "../define.h"

class Constitutive_Law
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Constitutive_Law)

            enum StrainMeasure
            {
                StrainMeasure_Infinitesimal,   //strain measure small displacements
                StrainMeasure_GreenLagrange,   //strain measure reference configuration
                StrainMeasure_Almansi,         //strain measure current configuration

                // True strain:
                StrainMeasure_Hencky_Material, //strain measure reference configuration
                StrainMeasure_Hencky_Spatial,  //strain measure current   configuration

                // Deformation measures:
                StrainMeasure_Deformation_Gradient, //material deformation gradient as a strain measure
                StrainMeasure_Right_CauchyGreen,    //right cauchy-green tensor as a strain measure
                StrainMeasure_Left_CauchyGreen,     //left  cauchy-green tensor as a strain measure
                StrainMeasure_Velocity_Gradient     //spatial velocity gradient as a strain measure
            };

            enum StressMeasure
            {
                StressMeasure_PK1,            //stress related to reference configuration non-symmetric
                StressMeasure_PK2,            //stress related to reference configuration
                StressMeasure_Kirchhoff,      //stress related to current   configuration
                StressMeasure_Cauchy          //stress related to current   configuration
            };
        /// @}


        /// @name Life Circle
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