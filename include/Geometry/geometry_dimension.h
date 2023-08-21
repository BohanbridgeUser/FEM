#ifndef _GEOMETRY_DIMENSION_H_
#define _GEOMETRY_DIMENSION_H_
#include "../define.h"
class Geometry_Dimension
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Geometry_Dimension);
        /// @}


        /// @name Life Circle
        /// @{
            Geometry_Dimension()
            {

            }
            Geometry_Dimension(int mThisWorkingSpaceDimension, int mThisLocalSpaceDimension)
            :mWorkingSpaceDimension(mThisLocalSpaceDimension),
             mLocalSpaceDimension(mThisLocalSpaceDimension)
            {
            
            }
            Geometry_Dimension(const Geometry_Dimension& another)
            :mWorkingSpaceDimension(another.mWorkingSpaceDimension),
             mLocalSpaceDimension(another.mLocalSpaceDimension)
            {
            
            }
            ~Geometry_Dimension()
            {

            }
        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            void SetDimension(const int& mThisWorkingSpaceDimension, const int& mThisLocalSpaceDimension)
            {
                SetWorkingSpaceDimension(mThisWorkingSpaceDimension);
                SetLocalSpaceDimensio(mThisLocalSpaceDimension);
            }
        /// @}


        /// @name Access
        /// @{
            int& GetWorkingSpaceDimension()
            {
                return mWorkingSpaceDimension;
            }
            int GetWorkingSpaceDimension() const
            {
                return mWorkingSpaceDimension;
            }
            int& GetLocalSpaceDimension()
            {
                return mLocalSpaceDimension;
            }
            int GetLocalSpaceDimension() const
            {
                return mLocalSpaceDimension;
            }
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
            int mWorkingSpaceDimension;
            int mLocalSpaceDimension;
        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            void SetWorkingSpaceDimension(const int& mThisWorkingSpaceDimension)
            {
                mWorkingSpaceDimension = mThisWorkingSpaceDimension;
            }
            void SetLocalSpaceDimensio(const int& mThisLocalSpaceDimension)
            {
                mLocalSpaceDimension = mThisLocalSpaceDimension;
            }
        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};
#endif