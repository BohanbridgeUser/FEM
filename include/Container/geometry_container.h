#ifndef _GEOMETRY_CONTAINER_H_
#define _GEOMETRY_CONTAINER_H_
#include "../Geometry/geometry.h"
#include "../Container/pointer_hash_map_set.h"

#include <unordered_map>
#include <utility>


template<class TGeometryType>
class GeometryContainer
{
    public:
        /// @name Type Define
        /// @{
            class GetGeometryId
            {
                public:
                    std::size_t const& operator()(const TGeometryType& rGeometry) const
                    {
                        return rGeometry.Id();
                    }
            };

            LOTUS_POINTER_DEFINE(GeometryContainer);

            typedef std::size_t 
                                                                                IndexType;
            typedef std::size_t 
                                                                                 SizeType;
            typedef typename TGeometryType::Pointer 
                                                                      GeometryPointerType;

            typedef PointerHashMapSet<
            TGeometryType,
            std::hash<std::size_t>,
            GetGeometryId,
            GeometryPointerType
            >                                                           GeometriesMapType;
            /// Geometry Iterator
            typedef typename GeometriesMapType::iterator
                                                                         GeometryIterator;
            /// Const Geometry Iterator
            typedef typename GeometriesMapType::const_iterator 
                                                                 GeometryConstantIterator;
        /// @}


        /// @name Life Circle
        /// @{
            /// Default Constructor
            GeometryContainer()
                : mGeometries()
            {}
            /// Copy Constructor
            GeometryContainer(GeometryContainer const& rOther)
                : mGeometries(rOther.mGeometries)
            {}
            /// Components Constructor
            GeometryContainer(
                GeometriesMapType& NewGeometries)
                : mGeometries(NewGeometries)
            {}
            /// Destructor
            ~GeometryContainer() = default;

        /// @}


        /// @name Operators
        /// @{


        /// @}


        /// @name Operations
        /// @{
            GeometryContainer Clone()
            {
                typename GeometriesMapType::Pointer p_geometries(new GeometriesMapType(*mGeometries));

                return GeometryContainer(p_geometries);
            }
            void Clear()
            {
                mGeometries.clear();
            }
            /// Adds a geometry to the geometry container.
            GeometryIterator AddGeometry(GeometryPointerType pNewGeometry)
            {
                auto i = mGeometries.find(pNewGeometry->Id());
                if(i == mGeometries.end())
                    return mGeometries.insert(pNewGeometry);
                else
                {
                    std::cerr << "Geometry with Id: " << pNewGeometry->Id()
                        << " exists already.";
                        exit(0);
                }
            }
            /// Remove the geometry with given Id from geometry container
            void RemoveGeometry(IndexType GeometryId)
            {
                mGeometries.erase(GeometryId);
            }
            /// Remove the geometry with given name from geometry container
            void RemoveGeometry(std::string GeometryName)
            {
                auto index = TGeometryType::GenerateId(GeometryName);

                mGeometries.erase(index);
            }

        /// @}

        /// @name Access
        /// @{
            /// Returns the Geometry::Pointer corresponding to its Id
            GeometryPointerType pGetGeometry(IndexType GeometryId)
            {
                auto i = mGeometries.find(GeometryId);
                if(i == mGeometries.end())
                {
                    std::cerr<< " geometry index not found: " << GeometryId << ".";
                    exit(0); 
                }
                return (i.base()->second);
            }
            /// Returns the const Geometry::Pointer corresponding to its Id
            const GeometryPointerType pGetGeometry(IndexType GeometryId) const
            {
                auto i = mGeometries.find(GeometryId);
                if(i == mGeometries.end())
                {
                    std::cerr<< " geometry index not found: " << GeometryId << ".";
                    exit(0); 
                }
                return (i.base()->second);
            }
            /// Returns the Geometry::Pointer corresponding to its name
            GeometryPointerType pGetGeometry(std::string GeometryName)
            {
                auto hash_index = TGeometryType::GenerateId(GeometryName);
                auto i = mGeometries.find(hash_index);
                if(i == mGeometries.end())
                {
                    std::cerr<< " geometry index not found: " << GeometryName << ".";
                    exit(0); 
                }
                return (i.base()->second);
            }
            /// Returns the Geometry::Pointer corresponding to its name
            const GeometryPointerType pGetGeometry(std::string GeometryName) const
            {
                auto hash_index = TGeometryType::GenerateId(GeometryName);
                auto i = mGeometries.find(hash_index);
                if(i == mGeometries.end())
                {
                    std::cerr<< " geometry index not found: " << GeometryName << ".";
                    exit(0); 
                }
                return (i.base()->second);
            }
            /// Returns a reference geometry corresponding to the id
            TGeometryType& GetGeometry(IndexType GeometryId)
            {
                return *pGetGeometry(GeometryId);
            }
            /// Returns a const reference geometry corresponding to the id
            const TGeometryType& GetGeometry(IndexType GeometryId) const
            {
                return *pGetGeometry(GeometryId);
            }
            /// Returns a reference geometry corresponding to the name
            TGeometryType& GetGeometry(std::string GeometryName)
            {
                return *pGetGeometry(GeometryName);
            }
            /// Returns a const reference geometry corresponding to the name
            const TGeometryType& GetGeometry(std::string GeometryName) const
            {
                return *pGetGeometry(GeometryName);
            }

            GeometriesMapType& Geometries()
            {
                return mGeometries;
            }
            const GeometriesMapType& Geometries() const
            {
                return mGeometries;
            }

            GeometryIterator GeometriesBegin()
            {
                return mGeometries.begin();
            }
            GeometryConstantIterator GeometriesBegin() const
            {
                return mGeometries.begin();
            }
            GeometryIterator GeometriesEnd()
            {
                return mGeometries.end();
            }
            GeometryConstantIterator GeometriesEnd() const
            {
                return mGeometries.end();
            }

        /// @}

        /// @name Inquiry
        /// @{
            SizeType NumberOfGeometries() const
            {
                return mGeometries.size();
            }
            bool HasGeometry(IndexType GeometryId) const
            {
                return (mGeometries.find(GeometryId) != mGeometries.end());
            }
            bool HasGeometry(std::string GeometryName) const
            {
                auto hash_index = TGeometryType::GenerateId(GeometryName);

                return (mGeometries.find(hash_index) != mGeometries.end());
            }

        /// @}

        /// @name Input And Output
        /// @{
            std::string Info() const
            {
                return "GeometryContainer";
            }
            /// Print information about this object
            void PrintInfo(std::ostream& rOStream) const
            {
                rOStream << Info();
            }
            /// Print object's data.
            void PrintData(std::ostream& rOStream) const
            {
                rOStream << "Number of Geometries: " << mGeometries.size() << std::endl;
            }
            /// Print information about this object
            virtual void PrintInfo(std::ostream& rOStream, std::string const& PrefixString) const
            {
                rOStream << PrefixString << Info();
            }
            /// Print object's data
            virtual void PrintData(std::ostream& rOStream, std::string const& PrefixString ) const
            {
                rOStream << PrefixString << "Number of Geometries: " << mGeometries.size() << std::endl;
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
            GeometriesMapType mGeometries;

        /// @}


        /// @name Private Operatiors
        /// @{
            /// Assignment operator.
            GeometryContainer& operator=(const GeometryContainer& rOther)
            {
                mGeometries = rOther.mGeometries;
                return *this;
            }

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
template<class TGeometryType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const GeometryContainer<TGeometryType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
#endif