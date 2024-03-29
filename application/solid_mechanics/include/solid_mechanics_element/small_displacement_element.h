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
            Small_Displacement_Element();

            Small_Displacement_Element(IndexType NewId,
                                       GeometryType::Pointer pThisGeometry);

            Small_Displacement_Element(IndexType NewId,
                                       GeometryType::Pointer pThisGeometry,
                                       Properties::Pointer pThisProperties);
            
            Small_Displacement_Element(Small_Displacement_Element const& another);

            ~Small_Displacement_Element() override;

        /// @}


        /// @name Operators
        /// @{
            Small_Displacement_Element& operator=(Small_Displacement_Element const& another);

        /// @}


        /// @name Operations
        /// @{
            Element::SharedPointer Create(IndexType NewId,
                                          NodesContainerType const& rNodes,
                                          Properties::Pointer pProperties) const override;

            Element::SharedPointer Clone(IndexType NewId,
                                         NodesContainerType const& rNodes) const override;

            int Check(Process_Info const& rCurrentProcessInfo)const override;



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
            /**
             * Calculation of the Geometric Stiffness Matrix
             */

            void CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
                                     ElementDataType& rVariables,
                                     double& rIntegrationWeight) override;
            /**
             * Set Variables of the Element to the Parameters of the Constitutive Law
             */
            void SetElementData(ElementDataType& rVariables,
                                Constitutive_Law::Parameters& rValues,
                                const int & rPointNumber) override;

            /**
             * Calculate Element Kinematics
             */
            void CalculateKinematics(ElementDataType& rVariables,
                                     const double& rPointNumber) override;

            /**
             * Initialize Element General Variables
             */
            void InitializeElementData(ElementDataType & rVariables,
                                       const Process_Info& rCurrentProcessInfo) override;


            /**
             * Calculation of the Infinitesimal Strain Vector
             */
            virtual void CalculateInfinitesimalStrain(const Matrix& rH,
                                                      Vector& rStrainVector);

            /**
             * Calculation of the Displacement Gradient H
             */
            void CalculateDisplacementGradient(Matrix& rH,
                                               const Matrix& rDN_DX);


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