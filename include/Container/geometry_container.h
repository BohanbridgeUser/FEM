#ifndef _GEOMETRY_CONTAINER_H_
#define _GEOMETRY_CONTAINER_H_
#include "../Geometry/geometry.h"

#include <unordered_map>
#include <utility>
template<typename TGeometryType>
class Geometry_Container
{
    public:
        /// @name Define 
        /// @{
            LOTUS_POINTER_DEFINE(Geometry_Container<TGeometryType>)
            typedef std::unordered_map<int, TGeometryType* ,std::hash<int> > 
                                                                            ContainerType;
            typedef int 
                                                                                 Key_Type;
        /// @}
        
        /// @name Life Circle
        /// @{
            /* Constructor */ 
            Geometry_Container()
            {
                
            }
            Geometry_Container(const Geometry_Container<TGeometryType>& another):Container(another.Container)
            {

            }

            /* Destructor */
            ~Geometry_Container()
            {

            }
        /// @}
        
        /// @name Operators
        /// @{
          
        /// @}

        /// @name Operations
        /// @{
            void Insert(TGeometryType& item)
            {
                Container.insert(std::pair<int,TGeometryType*>(item.GetID(),&item));
            }
            void Erase(const Key_Type& k)
            {
                Container.erase(k);
            }
            TGeometryType& At(const Key_Type& k)
            {
                return *(Container.at(k));
            }
            int Size()const 
            {
                return Container.size();
            }
        /// @}

        /// @name Access
        /// @{

        /// @}

        /// @name Inquiry
        /// @{

        /// @}

        /// @name Output
        /// @{
            
        /// @}
    private:
        ContainerType Container; 
};

#endif