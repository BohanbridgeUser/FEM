#ifndef _LOAD_CONDITION_H_
#define _LOAD_CONDITION_H_
#include "boundary_condition.h"

class Load_Condition : public Boundary_Condition
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Load_Condition)

            typedef Boundary_Condition::GeometryType
                                                                GeometryType;
            typedef typename GeometryType::SizeType
                                                                    SizeType;
            typedef Boundary_Condition
                                                                    BaseType;
        /// @}


        /// @name Life Circle
        /// @{
            Load_Condition();

            Load_Condition(IndexType NewId,
                           GeometryType::Pointer pGeometry);
            
            Load_Condition(IndexType NewId,
                           GeometryType::Pointer pGeometry,
                           PropertiesType::Pointer pProperties);

            Load_Condition(const Load_Condition& another);

            virtual ~Load_Condition() override {};
        /// @}


        /// @name Operators
        /// @{

        /// @}


        /// @name Operations
        /// @{
            Condition::SharedPointer Create(IndexType NewId,
                                            NodesContainerType const& rThisNodes,
                                            Properties::Pointer pProperties)const override;

            Condition::SharedPointer Clone(IndexType NewId,
                                           NodesContainerType const& rThisNodes) const override;

            int Check(const Process_Info& rThisProcessInfo) const override;

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
            /**
             * Check dof for a vector variable
             */
            bool HasVariableDof(VariableVectorType& rVariable) const override
            {
                if(rVariable == ROTATION)
                    return false;
                else
                    return Boundary_Condition::HasVariableDof(rVariable);
            };

            /**
             * Initialize General Variables
             */
            void InitializeConditionVariables(ConditionVariables& rVariables,
                                              const Process_Info& rCurrentProcessInfo) override;

            /**
             * Calculate the External Load of the Condition
             */
            virtual void CalculateExternalLoad(ConditionVariables& rVariables);
            /**
             * Calculation of the External Forces Vector for a force or pressure vector
             */
            void CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                               ConditionVariables& rVariables,
                                               double& rIntegrationWeight) override;
            /**
             * Calculation of the External Forces Vector for a force or pressure vector
             */
            double& CalculateAndAddExternalEnergy(double& rEnergy,
                                                  ConditionVariables& rVariables,
                                                  double& rIntegrationWeight,
                                                  const Process_Info& rCurrentProcessInfo) override;

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