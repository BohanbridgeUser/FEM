#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include "../define.h"
#include "geometry_dimension.h"
#include "../Container/points_container.h"
#include "geometry_data.h"
#include "../Quadrature/integration_point.h"

#include <vector>
#include <array>
#include <memory>
template<class TPointType>
class Geometry 
{
    public:
        /// @name Define 
        /// @{ 
            enum class Geometry_Type {
                Origin,
                Line,
                Triangle,
                Quadrilateral,
                Tetrahedron,
                Hexahedron
            };

            LOTUS_POINTER_DEFINE(Geometry)
            typedef TPointType PointType;
            typedef std::vector<PointType> PointsContainerType;
            // @ Integration Points Define
            typedef Integration_Point<3> IntegrationPointType;
            typedef std::vector<IntegrationPointType> IntegrationPointsVector;
            typedef std::array<IntegrationPointsVector, 
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                    IntegrationPointsContainerType;
            
            // @ ShapeFunctionValueContainer Define
            typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                        ShapeFunctionsValueContainerType;

            // @ First derivatives/gradients
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                                ShapeFunctionsGradientsType;
            typedef std::array<ShapeFunctionsGradientsType,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                        ShapeFunctionsGradientsContainerType;

            /* JacobiansType */
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > JacobiansType;
        //}

        /// @name Life Circle 
        /// @{ 
            // Constructor  
                Geometry()
                {
                    number++;
                    ID = number;
                }
                Geometry(const int& id,PointsContainerType& Points):pPoints(Points),ID(id)
                {
                    number++;
                }
                Geometry(PointsContainerType& Points):pPoints(Points)
                {
                    number++;
                    ID = number;
                }
                Geometry(const Geometry<TPointType>& another):pPoints(another.pPoints)
                {
                    number++;
                    ID = number;
                }
                Geometry(Geometry<TPointType>&& another):pPoints(another.pPoints)
                {
                    number++;
                    ID = number;
                }
            // Destructor {
                virtual ~Geometry()
                {
                    if (number>0) number--;
                }    
        /// @}

        /// @name Operators
        /// @{
            Geometry<TPointType>& operator=(Geometry<TPointType>&& another)
            {
                pPoints = another.pPoints;
                return *this;
            }
            PointType& operator[](const int& index)
            {
                return pPoints[index];
            }
        /// @}

        /// @name Operations
        /// @{

        /// @}

        /// @name Access 
        /// @{
            PointType& GetValue(int index)
            {
                return pPoints[index];
            }
            PointType GetValue(int index) const
            {
                return pPoints[index];
            }
            PointsContainerType& pPointsVector()
            {
                return pPoints;
            }
            int& GetID()
            {
                return ID;
            }
            int GetID() const
            {
                return ID;
            }
            static Geometry_Type GetType()
            {
                return GeometryType;
            }
        /// @}

        /// @name Inquiry
        /// @{ 
            int GetPointsNum()const
            {
                return pPoints.size();
            }
            int WorkingSpaceDimension() const
            {
                return mGeometry_Dimension.GetWorkingSpaceDimension();
            }
            int LocalSpaceDimension() const
            {
                return mGeometry_Dimension.GetLocalSpaceDimension();
            }
        /// @}

        /// @name Input and Output
        /// @{
            friend std::ostream& operator<<(std::ostream& os,const Geometry& another)
            {
                for (auto p=another.pPoints.begin();p!=another.pPoints.end();++p)
                    os << *p;
                return os;
            }
        /// @}
    protected:
        int ID;
    private:
        PointsContainerType pPoints;
        static Geometry_Type GeometryType; 
        static int number;
        static const Geometry_Data mGeometry_Data;
        static const Geometry_Dimension mGeometry_Dimension;
};
template<typename TPointType> int Geometry<TPointType>::number = 0;
template<typename TPointType> typename Geometry<TPointType>::Geometry_Type Geometry<TPointType>::GeometryType = Geometry_Type::Origin;
template<typename TPointType> const Geometry_Dimension Geometry<TPointType>::mGeometry_Dimension = Geometry_Dimension(3,3);
#endif