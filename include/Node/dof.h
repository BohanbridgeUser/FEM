#ifndef _DOF_H_
#define _DOF_H_

#include "node.h"
#include "node_data.h"
#include "../Variable/variable.h"
#include "../Variable/variables_list_data_value_container.h"


template<class TDataType, class TVariableType = Variable<TDataType> >
struct DofTrait
{
    static const int Id;
};

template<class TDataType>
struct DofTrait<TDataType,Variable<TDataType> >
{
    static const int Id = 0;
};

template<class TDataType>
class Dof
{
    public:
        ///@name Define 
        ///@{ 
            LOTUS_POINTER_DEFINE(Dof<TDataType>)
            typedef Dof<TDataType>
                                                    ClassType;
            typedef size_t                         
                                                    IndexType;
            typedef size_t                         
                                               EquationIdType;
            typedef Node_Data                   
                                                 NodeDataType;
            typedef Variables_List_Data_Value_Container 
                               SolutionStepsDataContainerType;
            typedef Node_Data
                                                    NodalData;
        ///@}

        ///@ Lift Circle 
        ///@ {
            /** Constructor. This constructor takes all necessary
            information to construct a degree of freedom. Also default
            values are used to make it easier to define for simple cases.

            @param rThisVariable Variable which this degree of freedom
            holds. This variable considered as unknown of problem to solved
            and fixing by Fix() method also applied to it. It must be a
            TDataType variable or component not a vector. For example
            DISPLACEMENT_X in structural element.

            @see Node
            @see Variable
            */
            template<class TVariableType>
            Dof(NodeDataType* pThisNodalData,
                const TVariableType& rThisVariable)
                : mIsFixed(false),
                mVariableType(DofTrait<TDataType, TVariableType>::Id),
                mReactionType(DofTrait<TDataType, Variable<TDataType> >::Id),
                mEquationId(IndexType()),
                mpNodalData(pThisNodalData)
            {
                if(!pThisNodalData->GetSolutionStepData().Has(rThisVariable))
                {
                    std::cerr<< "The Dof-Variable " << rThisVariable.Name() << " is not "
                    << "in the list of variables" << std::endl;
                    exit(0);
                }
                mIndex = mpNodalData->GetSolutionStepData().pGetVariablesList()->AddDof(&rThisVariable);
            }
            /** Constructor. This constructor takes the same input
            as the previous one, but add the reaction on the DoF
            declaration


            @param rThisVariable Variable which this degree of freedom
            holds. This variable considered as unknown of problem to solved
            and fixing by Fix() method also applied to it. It must be a
            TDataType variable or component not a vector. For example
            DISPLACEMENT_X in structural element.


            @param rThisReaction This is the right hand side variable in
            the system of equation correspounding to variable this dof
            holding. For example THERMAL_FLOW in thermal element. It will
            be none as default.


            @see Node
            @see Variable
            */
            template<class TVariableType, class TReactionType>
            Dof(NodeDataType* pThisNodalData,
                const TVariableType& rThisVariable,
                const TReactionType& rThisReaction)
                : mIsFixed(false),
                mVariableType(DofTrait<TDataType, TVariableType>::Id),
                mReactionType(DofTrait<TDataType, TReactionType>::Id),
                mEquationId(IndexType()),
                mpNodalData(pThisNodalData)
            {
                if(!pThisNodalData->GetSolutionStepData().Has(rThisVariable))
                {
                    std::cerr<< "The Dof-Variable " << rThisVariable.Name() << " is not "
                    << "in the list of variables" << std::endl;
                    exit(0);
                }

                if(!pThisNodalData->GetSolutionStepData().Has(rThisVariable))
                {
                    std::cerr<< "The Reaction-Variable " << rThisReaction.Name() << " is not "
                    << "in the list of variables" << std::endl;
                    exit(0);
                }
                mIndex = mpNodalData->GetSolutionStepData().pGetVariablesList()->AddDof(&rThisVariable, &rThisReaction);
            }
            //This default constructor is needed for serializer
            Dof()
                : mIsFixed(false),
                mVariableType(DofTrait<TDataType, Variable<TDataType> >::Id),
                mReactionType(DofTrait<TDataType, Variable<TDataType> >::Id),
                mIndex(),
                mEquationId(IndexType()),
                mpNodalData()
            {
            }
            /// Copy constructor.
            Dof(Dof const& rOther)
                : mIsFixed(rOther.mIsFixed),
                mVariableType(rOther.mVariableType),
                mReactionType(rOther.mReactionType),
                mIndex(rOther.mIndex),
                mEquationId(rOther.mEquationId),
                mpNodalData(rOther.mpNodalData)
            {
            }
            /// Destructor.
            ~Dof() {}
        ///@}

        /// @name Operators
        /// @{
            Dof& operator=(Dof const& rOther)
            {
                mIsFixed = rOther.mIsFixed;
                mEquationId = rOther.mEquationId;
                mpNodalData = rOther.mpNodalData;
                mIndex = rOther.mIndex;
                mVariableType = rOther.mVariableType;
                mReactionType = rOther.mReactionType;
                return *this;
            }
            template<class TVariableType>
            typename TVariableType::Type& operator()(const TVariableType& rThisVariable, IndexType SolutionStepIndex = 0)
            {
                return GetSolutionStepValue(rThisVariable, SolutionStepIndex);
            }
            template<class TVariableType>
            typename TVariableType::Type const& operator()(const TVariableType& rThisVariable, IndexType SolutionStepIndex = 0) const
            {
                return GetSolutionStepValue(rThisVariable, SolutionStepIndex);
            }
            TDataType& operator()(IndexType SolutionStepIndex = 0)
            {
                return GetSolutionStepValue(SolutionStepIndex);
            }
            TDataType const& operator()(IndexType SolutionStepIndex = 0) const
            {
                return GetSolutionStepValue(SolutionStepIndex);
            }
            TDataType& operator[](IndexType SolutionStepIndex)
            {
                return GetSolutionStepValue(SolutionStepIndex);
            }
            TDataType const& operator[](IndexType SolutionStepIndex) const
            {
                return GetSolutionStepValue(SolutionStepIndex);
            }
        /// @}

        /// @name Operations
        /// @{
            /** Sets the Equation Id to the desired value
             */
            void SetEquationId(EquationIdType NewEquationId)
            {
                mEquationId = NewEquationId;
            }
            template<class TReactionType>
            void SetReaction(TReactionType const& rReaction)
            {
                mReactionType = DofTrait<TDataType, TReactionType>::Id;
                mpNodalData->GetSolutionStepData().pGetVariablesList()->SetDofReaction(&rReaction, mIndex);
            }
            /** Fixes the Dof
             */
            void FixDof()
            {
                mIsFixed=true;
            }
            /** Frees the degree of freedom
             */
            void FreeDof()
            {
                mIsFixed=false;
            }
            void SetNodalData(NodalData* pNewNodalData)
            {
                auto p_variable = &GetVariable();
                auto p_reaction = mpNodalData->GetSolutionStepData().pGetVariablesList()->pGetDofReaction(mIndex);
                mpNodalData = pNewNodalData;
                if(p_reaction != nullptr){
                    mIndex = mpNodalData->GetSolutionStepData().pGetVariablesList()->AddDof(p_variable, p_reaction);
                } else{
                    mIndex = mpNodalData->GetSolutionStepData().pGetVariablesList()->AddDof(p_variable);
                }
            }

        /// @}

        /// @name Access
        /// @{
            IndexType Id() const
            {
                return mpNodalData->GetId();
            }
            IndexType GetId() const
            {
                return mpNodalData->GetId();
            }
            EquationIdType EquationId() const
            {
                return mEquationId;
            }

            const Variable_Data& GetVariable() const
            {
                return mpNodalData->GetSolutionStepData().GetVariablesList().GetDofVariable(mIndex);
            }
            /** Returns reaction variable of this degree of freedom. */
            const Variable_Data& GetReaction() const
            {
                auto p_reaction = mpNodalData->GetSolutionStepData().GetVariablesList().pGetDofReaction(mIndex);
                return (p_reaction == nullptr) ? msNone : *p_reaction;
            }

            TDataType& GetSolutionStepValue(IndexType SolutionStepIndex = 0)
            {
                return GetReference(GetVariable(), mpNodalData->GetSolutionStepData(), SolutionStepIndex, mVariableType);
            }
            TDataType const& GetSolutionStepValue(IndexType SolutionStepIndex = 0) const
            {
                return GetReference(GetVariable(), mpNodalData->GetSolutionStepData(), SolutionStepIndex, mVariableType);
            }
            template<class TVariableType>
            typename TVariableType::Type& GetSolutionStepValue(const TVariableType& rThisVariable, IndexType SolutionStepIndex = 0)
            {
                return mpNodalData->GetSolutionStepData().GetValue(rThisVariable, SolutionStepIndex);
            }
            template<class TVariableType>
            typename TVariableType::Type const& GetSolutionStepValue(const TVariableType& rThisVariable, IndexType SolutionStepIndex = 0) const
            {
                return mpNodalData->GetSolutionStepData().GetValue(rThisVariable, SolutionStepIndex);
            }
            TDataType& GetSolutionStepReactionValue(IndexType SolutionStepIndex = 0)
            {
                return GetReference(GetReaction(), mpNodalData->GetSolutionStepData(), SolutionStepIndex, mReactionType);
            }
            TDataType const& GetSolutionStepReactionValue(IndexType SolutionStepIndex = 0) const
            {
                return GetReference(GetReaction(), mpNodalData->GetSolutionStepData(), SolutionStepIndex, mReactionType);
            }

            SolutionStepsDataContainerType* GetSolutionStepsData()
            {
                return &(mpNodalData->GetSolutionStepData());
            }

        /// @}

        /// @name Inquiry
        /// @{
            bool IsFree()
            {
                return mIsFixed != 1;
            }
            bool IsFixed()
            {
                return mIsFixed == 1;
            }
             bool HasReaction() const
            {
                return (mpNodalData->GetSolutionStepData().pGetVariablesList()->pGetDofReaction(mIndex) != nullptr);
            }

        /// @}
        /// @name Input And Output
        /// @{
            /// Turn back information as a string.
            std::string Info() const
            {
                std::stringstream buffer;

                if(IsFixed())
                    buffer << "Fix " << GetVariable().Name() << " degree of freedom";
                else
                    buffer << "Free " << GetVariable().Name() << " degree of freedom";

                return buffer.str();
            }

            /// Print information about this object.
            void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << Info();
            }

            /// Print object's data.
            void PrintData(std::ostream& rOStream) const
            {
                rOStream << "    Variable               : " << GetVariable().Name() << std::endl;
                rOStream << "    Reaction               : " << GetReaction().Name() << std::endl;
                if(IsFixed())
                    rOStream << "    IsFixed                : True" << std::endl;
                else
                    rOStream << "    IsFixed                : False" << std::endl;
                rOStream << "    Equation Id            : " << mEquationId << std::endl;
            }

        /// @}
    private:
        /// @name Private Static Member
        /// @{
            static const Variable<TDataType> msNone;
            static constexpr int msIsFixedPosition = 63;
        /// @}

        /// @name Private Member
        /// @{
            IndexType mIndex : 6;
            int mIsFixed : 1;
            int mVariableType : 4;
            int mReactionType : 4;
            EquationIdType mEquationId : 48;
            NodeDataType* mpNodalData;
        /// @}
        
        /// @name Private Operators
        /// @{

        /// @}

        /// @name Private Operations
        /// @{
            double& GetReference(const Variable_Data& rThisVariableData, Variables_List_Data_Value_Container& rData,const IndexType& rThisSolutionStep, const int& rReactionType)
            {
                if(!rReactionType) 
                    return rData.GetValue(static_cast< Variable<double> const&>(rThisVariableData),rThisSolutionStep);
                else
                {
                    std::cerr << "Not supported type for Dof" << std::endl;
                    exit(0);
                }
            }
            double const& GetReference(const Variable_Data& rThisVariableData,const Variables_List_Data_Value_Container& rData,const IndexType& rThisSolutionStep, const int& rReactionType)const
            {
                if(!rReactionType) return rData.GetValue(static_cast< Variable<double> const&>(rThisVariableData),rThisSolutionStep);
                else
                {
                    std::cerr << "Not supported type for Dof" << std::endl;
                    exit(0);
                }
            }
        /// @}
};
template<class TDataType> const Variable<TDataType> Dof<TDataType>::msNone("NONE");

///@}
///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

/// input stream function
template<class TDataType>
inline std::istream& operator >> (std::istream& rIStream,
                                  Dof<TDataType>& rThis);


/// output stream function
template<class TDataType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Dof<TDataType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);


    return rOStream;
}
///@}
///@name Operations
///@{


/// Greater than operator
template<class TDataType>
inline bool operator > ( Dof<TDataType> const& First,
                         Dof<TDataType> const& Second)
{
    if(First.Id() == Second.Id())
        return (First.GetVariable().Key() > Second.GetVariable().Key());

    return (First.Id() > Second.Id());
}

/// Less than operator
template<class TDataType>
inline bool operator < ( Dof<TDataType> const& First,
                         Dof<TDataType> const& Second)
{
    if(First.Id() == Second.Id())
        return (First.GetVariable().Key() < Second.GetVariable().Key());

    return (First.Id() < Second.Id());
}

/// Greater equal operator
template<class TDataType>
inline bool operator >= ( Dof<TDataType> const& First,
                          Dof<TDataType> const& Second)
{
    if(First.Id() == Second.Id())
        return (First.GetVariable().Key() >= Second.GetVariable().Key());

    return (First.Id() > Second.Id());
}

/// Less equal operator
template<class TDataType>
inline bool operator <= ( Dof<TDataType> const& First,
                          Dof<TDataType> const& Second)
{
    if(First.Id() == Second.Id())
        return (First.GetVariable().Key() <= Second.GetVariable().Key());

    return (First.Id() < Second.Id());
}

/// Equal operator
template<class TDataType>
inline bool operator == ( Dof<TDataType> const& First,
                          Dof<TDataType> const& Second)
{
    return ((First.Id() == Second.Id()) && (First.GetVariable().Key() == Second.GetVariable().Key()));
}

#endif