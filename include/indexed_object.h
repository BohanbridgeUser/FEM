#ifndef _INDEXED_OBJECT_H_
#define _INDEXED_OBJECT_H_

#include "define.h"

#include <iostream>

class Indexed_Object
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Indexed_Object)
            typedef size_t
                                                                    IndexType;
            typedef size_t
                                                                   ResultType;
            typedef ResultType
                                                                  result_type;

        /// @}


        /// @name Life Circle
        /// @{
            /// Default constructor.
            explicit Indexed_Object(IndexType NewId = 0) : mId(NewId) {}

            /// Destructor.
            virtual ~Indexed_Object() {}

            /// Copy constructor.
            Indexed_Object(Indexed_Object const& rOther) : mId(rOther.mId) {}

        /// @}


        /// @name Operators
        /// @{
            Indexed_Object& operator=(Indexed_Object const& rOther)
            {
                mId = rOther.mId;

                return *this;
            }

            template<class TObjectType>
            IndexType operator()(TObjectType const& rThisObject) const
            {
                return rThisObject.Id();
            }

        /// @}


        /// @name Operations
        /// @{
            virtual void SetId(IndexType NewId)
            {
                mId = NewId;
            }


        /// @}


        /// @name Access
        /// @{
            IndexType Id() const
            {
                return mId;
            }

            IndexType GetId() const
            {
                return mId;
            }
            IndexType& DepricatedIdAccess()
            {
                return mId;
            }

        /// @}


        /// @name Inquiry
        /// @{


        /// @}

        /// @name Input and Output
        /// @{
            /// Turn back information as a string.
            virtual std::string Info() const
            {
                std::stringstream buffer;
                buffer << "indexed object # "
                    << mId;
                return buffer.str();
            }

            /// Print information about this object.
            virtual void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << Info();
            }

            /// Print object's data.
            virtual void PrintData(std::ostream& rOStream) const
            {
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

            IndexType mId; /// The Id held by the object

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
/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Indexed_Object& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
#endif