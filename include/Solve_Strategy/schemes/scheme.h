#ifndef _SCHEME_H_
#define _SCHEME_H_
#include <Eigen/Eigen>
#include "../../define.h"
#include "../../dof.h"
#include "../../Model/model.h"
template<typename TSparseSpace, typename TDenseSpace>
class Scheme
{
    public:
        ///@name Define
        ///@{
            typedef Scheme<TSparseSpace,TDenseSpace> schemeType;
            LOTUS_POINTER_DEFINE(schemeType)

            typedef Dof                                                 DofType;

            typedef std::vector<DofType>                      DofsContainerType;

            typedef Model_Part::NodesContainerType           NodesContainerType;

            typedef Model_Part::ElementsContainerType     ElementsContainerType;

            typedef Model_Part::ConditionsContainerType ConditionsContainerType;
        ///@}

        ///@name Life Circle
        ///@{
            // Constructor
            explicit Scheme()
            {
                mSchemeIsInitialized = false;
                mElementsAreInitialized = false;
                mConditionsAreInitialized = false;
            }

            explicit Scheme(const Scheme& another)
            :mSchemeIsInitialized(another.mSchemeIsInitialized),
             mElementsAreInitialized(another.mElementsAreInitialized),
             mConditionsAraInitialized(another.mConditionsAraInitialized)
            {

            }

            // Destructor
            virtual ~Scheme()
            {
                
            }
        ///@}

        ///@name Operations
        ///@{
            
        ///@}
    protected:
        ///@name Static Member Variables
        ///@{

        ///@}
        ///@name Member Variables
        ///@{
            bool mSchemeIsInitialized;
            bool mElementsAreInitialized;
            bool mConditionsAraInitialized;
        ///@}
        ///@name Private Operators
        ///@{

        ///@}
        ///@name Private Operations
        ///@{

        ///@}
        ///@name Private  Access
        ///@{

        ///@}
        ///@name Private Inquiry
        ///@{

        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}
            
    private:
        ///@name Static Member Variables
        ///@{

        ///@}
        ///@name Member Variables
        ///@{

        ///@}
        ///@name Private Operators
        ///@{

        ///@}
        ///@name Private Operations
        ///@{

        ///@}
        ///@name Private  Access
        ///@{

        ///@}
        ///@name Private Inquiry
        ///@{

        ///@}
        ///@name Un accessible methods
        ///@{

        ///@}
};

#endif