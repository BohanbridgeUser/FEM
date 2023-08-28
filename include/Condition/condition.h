#ifndef _CONDITION_H_
#define _CONDITION_H_
#include "../define.h"
#include "../Container/lotus_flags.h"
#include "../Variable/variables.h"
#include "../Geometry/geometry.h"
#include "../Geometry/geometry_data.h"
#include "../Geometry/geometry_object.h"
#include "../Property/properties.h"
#include "../Node/dof.h"
#include "../process_info.h"
#include "../Utility/smart_pointer.h"
class Condition : public Geometry_Object
{
    public: 
        ///@name Define
        ///@{
            LOTUS_POINTER_DEFINE(Condition)
            typedef Condition                               
                                                                     ConditionType;
            typedef Geometry_Object 
                                                                          BaseType;
            typedef Node 
                                                                          NodeType;
            typedef Geometry<NodeType> 
                                                                      GeometryType;
            typedef Geometry_Data::IntegrationMethod 
                                                                 IntegrationMethod;
            typedef Geometry_Data 
                                                                  GeometryDataType; 
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                        MatrixType;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                        VectorType;
            typedef Properties
                                                                    PropertiesType;
            typedef Node::DofPointersContainerType
                                                                    DofsVectorType;
            typedef std::vector<size_t>
                                                              EquationIdVectorType;
            typedef GeometryType::PointsContainerType
                                                                NodesContainerType;
        ///@}

        /// @name Life Circle
        /// @{
            // Constructor
            Condition():Geometry_Object()
            {
                
            }
            Condition(GeometryType* mpThisGeometry)
            :Geometry_Object(mpThisGeometry),
             mpProperties()
            {
            
            }
            Condition(GeometryType* mpThisGeometry, Properties& ThisProperties)
            :Geometry_Object(mpThisGeometry),
             mpProperties(&ThisProperties)
            {
            
            }
            Condition(IndexType NewID, GeometryType* mpThisGeometry)
            :Geometry_Object(NewID,mpThisGeometry)
            {

            }
            Condition(IndexType NewID, GeometryType* mpThisGeometry, Properties& ThisProperties)
            :Geometry_Object(NewID,mpThisGeometry),
             mpProperties(&ThisProperties)
            {
            
            }
            Condition(const Condition& another)
            :Geometry_Object(another.mpGeometry),
             mpProperties(another.mpProperties)
            {

            }
            Condition(Condition&& another)
            :Geometry_Object(&(another.GetGeometry())),
             mpProperties(&(another.GetProperties()))
            {

            }
            // Destructor
            virtual ~Condition()
            {
            
            }
        /// @}

        /// @name Operations
        /// @{
            virtual Condition::SharedPointer Create(IndexType NewId,
                                              NodesContainerType const& ThisNodes,
                                              PropertiesType::Pointer pProperties ) const
            {
                return make_shared<Condition>(NewId, GetGeometry().Create(ThisNodes), *pProperties);
            }

            Properties& GetProperties()
            {
                return *mpProperties;
            }
            const Properties& GetProperties()const
            {
                return *mpProperties;
            }
            
            virtual int Check()
            {
                return 0;
            }

            virtual void Initialize(Process_Info& rCurrentProcessInfo)
            {
                
            }

            virtual void InitializeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }

            virtual void FinalizeSolutionStep(Process_Info& rCurrentProcessInfo)
            {

            }
            /**
             * @brief This class is derived from Geometry_Object. It have methods follow:
             * GeometryType& GetGeometry() 
             * GeometryType GetGeometry() const
            */
        /// @}

        /// @name Access
        /// @{
            
            /**
             * determines the condition list of DOFs
             * @param ConditionDofList the list of DOFs
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void GetDofList(DofsVectorType& rElementalDofList,
                                    const Process_Info& rCurrentProcessInfo) const
            {
                if (rElementalDofList.size() != 0) {
                    rElementalDofList.resize(0);
                }
            }
            
            /**
             * this determines the condition equation ID vector for all condition
             * DOFs
             * @param rResult the condition equation ID vector
             * @param rCurrentProcessInfo the current process info instance
             */
            virtual void EquationIdVector(EquationIdVectorType& rResult,
                                        const Process_Info& rCurrentProcessInfo) const
            {
                if (rResult.size() != 0) {
                    rResult.resize(0);
                }
            }
  
            /**
             * Getting method to obtain the variable which defines the degrees of freedom
             */
            virtual void GetValuesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

            /**
             * Getting method to obtain the time derivative of variable which defines the degrees of freedom
             */
            virtual void GetFirstDerivativesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

            /**
             * Getting method to obtain the second time derivative of variable which defines the degrees of freedom
             */
            virtual void GetSecondDerivativesVector(VectorType& values, int Step = 0) const
            {
                if (values.size() != 0) {
                    values.resize(0, false);
                }
            }

        /// @}
    private:
        Properties* mpProperties;
};

#endif