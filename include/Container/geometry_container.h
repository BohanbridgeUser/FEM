#ifndef _GEOMETRY_CONTAINER_H_
#define _GEOMETRY_CONTAINER_H_
#include "../Geometry/geometry.h"
#include <unordered_map>
#include <utility>
template<typename TGeometryType>
class Geometry_Container
{
    public:
        LOTUS_POINTER_DEFINE(Geometry_Container<TGeometryType>)
        typedef std::unordered_map<decltype(std::declval<TGeometryType>().GetType()), TGeometryType* ,std::hash<int> > ContainerType;
        typedef decltype(std::declval<TGeometryType>().GetType()) Key_Type;
        // @ Constructor { 
        Geometry_Container()
        {
            
        }
        Geometry_Container(const Geometry_Container<TGeometryType>& another):Container(another.Container)
        {

        }
        //}

        // @ Destructor {
        ~Geometry_Container()
        {

        }
        // }

        // @ Utility { 
        void insert(TGeometryType& item)
        {
            Container.insert(std::pair<int,TGeometryType*>(item.GetID(),&item));
        }
        void erase(const Key_Type& k)
        {
            Container.erase(k);
        }
        TGeometryType& at(const Key_Type& k)
        {
            return *Container.at(k);
        }
        int size()const 
        {
            return Container.size();
        }
        //}

    private:
        ContainerType Container; 
};

#endif