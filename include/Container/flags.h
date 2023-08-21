#ifndef _FLAGS_H_
#define _FLAGS_H_
#include "../define.h"

class Flags
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Flags)

            typedef int64_t BlockType;

            typedef int64_t FlagType;
            
            typedef size_t IndexType;

            enum FlagList
            {
                Flag0 = BlockType(1),
                Flag1 = BlockType(1) << 1,
                Flag2 = BlockType(1) << 2,
                Flag3 = BlockType(1) << 3,
                Flag4 = BlockType(1) << 4,
                Flag5 = BlockType(1) << 5,
                Flag6 = BlockType(1) << 6,
                Flag7 = BlockType(1) << 7,

                Flag8 = BlockType(1) << 8,
                Flag9 = BlockType(1) << 9,
                Flag10 = BlockType(1) << 10,
                Flag11 = BlockType(1) << 11,
                Flag12 = BlockType(1) << 12,
                Flag13 = BlockType(1) << 13,
                Flag14 = BlockType(1) << 14,
                Flag15 = BlockType(1) << 15,

                Flag16 = BlockType(1) << 16,
                Flag17 = BlockType(1) << 17,
                Flag18 = BlockType(1) << 18,
                Flag19 = BlockType(1) << 19,
                Flag20 = BlockType(1) << 20,
                Flag21 = BlockType(1) << 21,
                Flag22 = BlockType(1) << 22,
                Flag23 = BlockType(1) << 23,

                Flag24 = BlockType(1) << 24,
                Flag25 = BlockType(1) << 25,
                Flag26 = BlockType(1) << 26,
                Flag27 = BlockType(1) << 27,
                Flag28 = BlockType(1) << 28,
                Flag29 = BlockType(1) << 29,
                Flag30 = BlockType(1) << 30,
                Flag31 = BlockType(1) << 31,
            };
        /// @}


        /// @name Life Circle
        /// @{
            /* Constructor */
            Flags():mIsDefined(BlockType()),mFlags(BlockType())
            {

            }

            
            Flags(const Flags& another)
            :mIsDefined(another.mIsDefined),
             mFlags(another.mFlags)
            {
            
            }
            /* Destructor */
            virtual ~Flags()
            {

            }

            /* Create */
            static Flags Create(IndexType ThisPosition, bool Value = true)
            {
                Flags flags;
                flags.SetPosition(ThisPosition,Value);
                return flags;
            }
        /// @}

        /// @name Operators
        /// @{
            Flags& operator=(const Flags& another)
            {
                mIsDefined = another.mIsDefined;
                mFlags = another.mFlags;
                return *this;
            }

            operator bool()const
            {
                return mFlags;
            }

            friend bool operator==(const Flags& Left, const Flags& Right );

            friend bool operator!=(const Flags& Left, const Flags& Right );

            friend Flags operator|(const Flags& Left, const Flags& Right );

            friend Flags operator&(const Flags& Left, const Flags& Right );

            const Flags& operator|=(const Flags& Other );

            const Flags& operator&=(const Flags& Other );

        /// @}

        /// @name Operations
        /// @{
            /**
             * @brief This is a bithack. Using bit to represnt different flags.
             * @param mIsdefined
             * @param mFlags
             * @details mIsdefined sets position bit to 1. mFlags sets position bit to 1.
            */
            void SetPosition(IndexType ThisPosition, bool Value = true)
            {
                mIsDefined |= (BlockType(1) << ThisPosition);

                mFlags &= ~(BlockType(1) << ThisPosition);
                mFlags |= (BlockType(Value) << ThisPosition);
            }

            void AssignFlags(Flags const& rOther)
            {
                mIsDefined = rOther.mIsDefined;
                mFlags = rOther.mFlags;
            }

            void Set(const Flags ThisFlag)
            {
                mIsDefined |= ThisFlag.mIsDefined;
                mFlags = (mFlags & ~ThisFlag.mIsDefined) | (ThisFlag.mIsDefined & ThisFlag.mFlags);
            }

            void Set(const Flags ThisFlag, bool Value)
            {
                mIsDefined |= ThisFlag.mIsDefined;
                mFlags = (mFlags & ~ThisFlag.mIsDefined) | (ThisFlag.mIsDefined * BlockType(Value));
            }

            /**
             * @name Reset bits by using @param ThisFlag
             * @details 
            */
            void Reset(const Flags ThisFlag)
            {
                mIsDefined &= (~ThisFlag.mIsDefined);
                mFlags &= (~ThisFlag.mIsDefined); 
            }

            void Flip(const Flags ThisFlag)
            {
                mIsDefined |= ThisFlag.mIsDefined;
                mFlags ^= (ThisFlag.mIsDefined);
            }

            bool GetPosition(IndexType Position) const
            {
                return (mFlags & (BlockType(1) << Position));
            }

            void FlipPosition(IndexType Position)
            {
                mIsDefined |= (BlockType(1) << Position);
                mFlags ^= (BlockType(1) << Position);
            }

            void ClearPosition(IndexType Position)
            {
                mIsDefined &= ~((BlockType(1) << Position));
                mFlags &= ~(BlockType(1) << Position);
            }
        
            void Clear()
            {
                mIsDefined = BlockType();
                mFlags = BlockType();
            }
        
            static const Flags AllDefined()
            {
                return Flags(~0,0);
            }

            static const Flags AllTrue()
            {
                return Flags(~0,~0);
            }
        /// @}

        /// @name Inquiry
        /// @{
            bool Is(Flags const & rOther) const
            {
                return (mFlags & rOther.mFlags) | ((rOther.mIsDefined ^ rOther.mFlags) & (~mFlags));
            }

            bool IsDefined(Flags const & rOther) const
            {
                return (mIsDefined & rOther.mIsDefined);
            }

            bool IsNot(Flags const& rOther) const
            {
                return !((mFlags & rOther.mFlags) | ((rOther.mIsDefined ^ rOther.mFlags) & (~mFlags)));
            }

            bool IsNotDefined(Flags const& rOther) const
            {
                return !(mIsDefined & rOther.mIsDefined);
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
            BlockType mIsDefined;
            BlockType mFlags;

        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{


        /// @}


        /// @name Private Access
        /// @{
            Flags(BlockType DefinedFlags, BlockType SetFlags)
            :mIsDefined(DefinedFlags), mFlags(SetFlags)
            {
                
            }

        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}
};

#endif