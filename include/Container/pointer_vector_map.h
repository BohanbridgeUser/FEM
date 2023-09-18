
#ifndef _POINTER_VECTOR_MAP_H_
#define _POINTER_VECTOR_MAP_H_



// System includes
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <cstddef>

// External includes


// Project includes
#include "../define.h"
#include "pointer_vector_map_iterator.h"


/// Pointer_Vector_Map is a sorted associative container like stl map but using a vector to store pointers to its data.
/** Pointer_Vector_Map is a sorted associative container like stl map
    but using a vector to store pointers its data.

    This Container unlike the boost one does not free the memory by
    itself and relies on using of counted pointers or manual
    deleting.
 */
template<class TKeyType,class TDataType,

         class TCompareType = std::less<TKeyType>,
         class TPointerType = std::shared_ptr<TDataType>,
         class TContainerType = std::vector<std::pair<TKeyType, TPointerType> > >
class Pointer_Vector_Map final
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of Pointer_Vector_Map
    LOTUS_POINTER_DEFINE(Pointer_Vector_Map);

    /// Key type for searching in this container.
    typedef TKeyType 
                                                                                    key_type;

    /// data type stores in this container.
    typedef TDataType 
                                                                                   data_type;
    typedef std::pair<TKeyType, TPointerType> 
                                                                                  value_type;
    typedef TCompareType 
                                                                                 key_compare;
    typedef TPointerType 
                                                                                     pointer;
    typedef TDataType& 
                                                                                   reference;
    typedef const TDataType& 
                                                                             const_reference;
    typedef TContainerType 
                                                                               ContainerType;

    typedef PointerVectorMapIterator<typename TContainerType::iterator, TDataType>                
                                                                                    iterator;
    typedef PointerVectorMapIterator<typename TContainerType::const_iterator, TDataType>          
                                                                              const_iterator;
    typedef PointerVectorMapIterator<typename TContainerType::reverse_iterator, TDataType>        
                                                                            reverse_iterator;
    typedef PointerVectorMapIterator<typename TContainerType::const_reverse_iterator, TDataType>  
                                                                      const_reverse_iterator;

    typedef typename TContainerType::size_type 
                                                                                   size_type;
    typedef typename TContainerType::iterator 
                                                                               pair_iterator;
    typedef typename TContainerType::const_iterator 
                                                                         pair_const_iterator;
    typedef typename TContainerType::reverse_iterator 
                                                                       pair_reverse_iterator;
    typedef typename TContainerType::const_reverse_iterator 
                                                                 pair_const_reverse_iterator;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    Pointer_Vector_Map()
    :mData(),
     mSortedPartSize(size_type()), 
     mMaxBufferSize(100) 
     {}

    Pointer_Vector_Map(const Pointer_Vector_Map& rOther)
    :mData(rOther.mData),
     mSortedPartSize(rOther.mSortedPartSize), 
     mMaxBufferSize(rOther.mMaxBufferSize) 
     {}

    explicit Pointer_Vector_Map(const TContainerType& rContainer) 
    :mData(rContainer),
     mSortedPartSize(size_type()), 
     mMaxBufferSize(100)
    {
        Sort();
        std::unique(mData.begin(), mData.end(), EqualKeyTo());
    }

    /// Destructor.
    ~Pointer_Vector_Map() {}


    ///@}
    ///@name Operators
    ///@{

    Pointer_Vector_Map& operator=(const Pointer_Vector_Map& rOther)
    {
        mData = rOther.mData;
        mSortedPartSize = rOther.mSortedPartSize;

        return *this;
    }

    TDataType& operator[](const key_type& Key)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end)
        {
            mSortedPartSize++;
            return *(mData.insert(sorted_part_end, value_type(Key, TPointerType(new TDataType)))->second);
        }

        if (Key != i->first)
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                mData.push_back(value_type(Key, TPointerType(new TDataType)));
                return *((--mData.end())->second);
            }

        return *(i->second);
    }

    pointer operator()(const key_type& Key)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end)
        {
            mSortedPartSize++;
            return (mData.insert(sorted_part_end, value_type(Key, TPointerType(new TDataType)))->second);
        }

        if (Key != i->first)
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                mData.push_back(value_type(Key, TPointerType(new TDataType)));
                return ((--mData.end())->second);
            }

        return (i->second);
    }


    ///@}
    ///@name Operations
    ///@{

    iterator                   begin()
    {
        return iterator( mData.begin() );
    }
    const_iterator             begin() const
    {
        return const_iterator( mData.begin() );
    }
    iterator                   end()
    {
        return iterator( mData.end() );
    }
    const_iterator             end() const
    {
        return const_iterator( mData.end() );
    }
    reverse_iterator           rbegin()
    {
        return reverse_iterator( mData.rbegin() );
    }
    const_reverse_iterator     rbegin() const
    {
        return const_reverse_iterator( mData.rbegin() );
    }
    reverse_iterator           rend()
    {
        return reverse_iterator( mData.rend() );
    }
    const_reverse_iterator     rend() const
    {
        return const_reverse_iterator( mData.rend() );
    }
    pair_iterator               pair_begin()
    {
        return mData.begin();
    }
    pair_const_iterator         pair_begin() const
    {
        return mData.begin();
    }
    pair_iterator               pair_end()
    {
        return mData.end();
    }
    pair_const_iterator         pair_end() const
    {
        return mData.end();
    }
    pair_reverse_iterator       pair_rbegin()
    {
        return mData.rbegin();
    }
    pair_const_reverse_iterator pair_rbegin() const
    {
        return mData.rbegin();
    }
    pair_reverse_iterator       pair_rend()
    {
        return mData.rend();
    }
    pair_const_reverse_iterator pair_rend() const
    {
        return mData.rend();
    }

    reference        front()       /* nothrow */
    {
        assert( !empty() );
        return *(mData.front()->second);
    }
    const_reference  front() const /* nothrow */
    {
        assert( !empty() );
        return *(mData.front()->second);
    }
    reference        back()        /* nothrow */
    {
        assert( !empty() );
        return *(mData.back()->second);
    }
    const_reference  back() const  /* nothrow */
    {
        assert( !empty() );
        return *(mData.back()->second);
    }

    size_type size() const
    {
        return mData.size();
    }

    size_type max_size() const
    {
        return mData.max_size();
    }

    key_compare key_comp() const
    {
        return TCompareType();
    }

    void swap(Pointer_Vector_Map& rOther)
    {
        mData.swap(rOther.mData);
    }

    void push_back(value_type x)
    {
        mData.push_back(x);
    }

    iterator insert(key_type const& Key, const TDataType& rData)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end)
        {
            mSortedPartSize++;
            return (mData.insert(sorted_part_end, value_type(Key, TPointerType(new TDataType(rData)))));
        }

        if (Key != i->first)
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                mData.push_back(value_type(Key, TPointerType(new TDataType)));
                return iterator(--mData.end());
            }
        *(i->second) = rData;
        return i;
    }

    iterator insert(key_type const& Key, const TPointerType pData)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end)
        {
            mSortedPartSize++;
            return mData.insert(sorted_part_end, value_type(Key, pData));
        }

        if (Key != i->first)
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                mData.push_back(value_type(Key, pData));
                return iterator(--mData.end());
            }

        *(i->second) = *pData;
        return i;
    }

    iterator erase(iterator pos)
    {
        return iterator(mData.erase(pos.base()));
    }

    iterator erase( iterator first, iterator last )
    {
        return iterator( mData.erase( first.base(), last.base() ) );
    }

    iterator erase(const key_type& k)
    {
        return erase(find(k));
    }

    void clear()
    {
        mData.clear();
    }

    iterator find(const key_type& Key)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end || (Key != i->first))
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
                return mData.end();

        return i;
    }

    const_iterator find(const key_type& Key) const
    {
        pair_const_iterator sorted_part_end(mData.begin() + mSortedPartSize);

        pair_const_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end || (Key != i->first))
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
                return mData.end();

        return const_iterator(i);
    }

    size_type count(const key_type& Key)
    {
        return find(Key) == mData.end() ? 0 : 1;
    }


    TDataType& at(const key_type& Key)
    {
        pair_iterator sorted_part_end;

        if(mData.size() - mSortedPartSize >= mMaxBufferSize)
        {
            Sort();
            sorted_part_end = mData.end();
        }
        else
            sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end){
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                std::cerr << Key << " not found in this map" << std::endl;
                exit(0);
            }
        }

        return *(i->second);
    }

    TDataType& at(const key_type& Key) const
    {
        pair_iterator sorted_part_end;
        sorted_part_end	= mData.begin() + mSortedPartSize;

        pair_iterator i(std::lower_bound(mData.begin(), sorted_part_end, Key, CompareKey()));
        if (i == sorted_part_end){
            if((i = std::find_if(sorted_part_end, mData.end(), EqualKeyTo(Key))) == mData.end())
            {
                std::cerr << Key << " not found in this map" << std::endl;
                exit(0);
            }
        }

        return *(i->second);
    }


    void Sort()
    {
        std::sort(mData.begin(), mData.end(), CompareKey());
        mSortedPartSize = mData.size();
    }

    ///@}
    ///@name Access
    ///@{

    /** Gives a reference to underly normal container. */
    TContainerType& GetContainer()
    {
        return mData;
    }

    /** Gives a constant reference to underly normal container. */
    const TContainerType& GetContainer() const
    {
        return mData;
    }

    /**
     * @brief Get the maximum size of buffer used in the container.
     */
    size_type GetMaxBufferSize() const 
    {
        return mMaxBufferSize;
    }

    /** 
     * @brief Set the maximum size of buffer used in the container.
     * @details This container uses a buffer which keep data unsorted. After buffer size arrived to the MaxBufferSize it will sort all container and empties buffer.
     * @param NewSize Is the new buffer maximum size. 
     */
    void SetMaxBufferSize(const size_type NewSize)
    {
        mMaxBufferSize = NewSize;
    }

    /**
     * @brief Get the sorted part size of buffer used in the container.
     */
    size_type GetSortedPartSize() const 
    {
        return mSortedPartSize;
    }

    /** 
     * @brief Set the sorted part size of buffer used in the container.
     * @param NewSize Is the new buffer maximum size. 
     */
    void SetSortedPartSize(const size_type NewSize)
    {
        mSortedPartSize = NewSize;
    }

    ///@}
    ///@name Inquiry
    ///@{

    bool empty() const
    {
        return mData.empty();
    }

    bool IsSorted() const
    {
        return (mSortedPartSize == mData.size());
    }

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const
    {
        std::stringstream buffer;
        buffer << "Pointer vector map (size = " << size() << ") : ";

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
        for(typename TContainerType::const_iterator i = mData.begin() ; i != mData.end() ; i++)
            rOStream << "(" << i->first << " , " << *(i->second) << ")" << std::endl;
    }


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    class CompareKey
    {
    public:
        bool operator()(value_type const& a, key_type b) const
        {
            return TCompareType()(a.first, b);
        }
        bool operator()(key_type a, value_type const& b) const
        {
            return TCompareType()(a, b.first);
        }
        bool operator()(value_type const& a, value_type const& b) const
        {
            return TCompareType()(a.first, b.first);
        }
    };
//     class CompareValue : public std::binary_function<value_type&, value_type&, bool>{
//     public:
//       bool operator()(value_type& a, value_type& b) const
//       {return TCompareType()(a.first, b.first);}
//     };
    class EqualKeyTo
    {
        key_type mKey;
    public:
        explicit EqualKeyTo(key_type k) : mKey(k) {}
        bool operator()(value_type const& a, value_type const& b) const
        {
            return a.first == b.first;
        }
        bool operator()(value_type const& a) const
        {
            return a.first == mKey;
        }
    };

    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{

    TContainerType mData;
    size_type  mSortedPartSize;
    size_type mMaxBufferSize;

    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{

    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{


    ///@}

}; // Class Pointer_Vector_Map

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

/// output stream function
template<class TDataType,
         class TGetKeyType,
         class TCompareType,
         class TPointerType,
         class TContainerType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Pointer_Vector_Map<TDataType,TGetKeyType, TCompareType, TPointerType, TContainerType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}


#endif
