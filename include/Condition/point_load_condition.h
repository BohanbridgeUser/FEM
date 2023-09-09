#ifndef _POINT_LOAD_CONDITION_H_
#define _POINT_LOAD_CONDITION_H_
#include "load_condition.h"

class Point_Load_Condition : public Load_Condition
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Point_Load_Condition)

        /// @}


        /// @name Life Circle
        /// @{
            Point_Load_Condition();
            Point_Load_Condition(IndexType& NewId,
                                 GeometryType::Pointer pThisGeometry);
            Point_Load_Condition(IndexType& NewId,
                                 GeometryType::Pointer pThisGeometry,
                                 Properties::Pointer pThisProperties);
            Point_Load_Condition(Point_Load_Condition const& another);
            ~Point_Load_Condition() override {};
        /// @}

        /// @name Operators
        /// @{


        /// @}

        /// @name Operations
        /// @{
            Condition::SharedPointer Create(IndexType NewId,
                                            NodesContainerType const& rThisNodes,
                                            Properties::Pointer pProperties) const override;

            Condition::SharedPointer Clone(IndexType NewId,
                                           NodesContainerType const& rThisNodes) const override;
            
            int Check(Process_Info const& rCurrentProcessInfo) const override;
        /// @}


        /// @name Access
        /// @{


        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input and Output
        /// @{
            std::string Info()const
            {
                std::stringstream buffer;
                buffer << "Point Load Condition!\n";
                return buffer.str();
            }
            void PrintInfo(std::ostream& os) const
            {
                os << "Point Load Condition " << Id();
            }
            void PrintData(std::ostream& os)const
            {
                GetGeometry().PrintData(os);
            }
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
            void InitializeConditionVariables(ConditionVariables& rVariable,
                                              Process_Info const& rCurrentProcessInfo)override;

            void CalculateKinematics(ConditionVariables& rVariable,
                                     double const& rPointNumber) override;

            void CalculateExternalLoad(ConditionVariables& rVariable) override;

            void CalculateConditionSystem(LocalSystemComponents& rLocalSystem,
                                          const Process_Info& rCurrentProcessInfo);
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