#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include "../define.h"
#include "geometry_dimension.h"
#include "../Container/points_container.h"
#include "geometry_data.h"
#include "../Quadrature/integration_point.h"
#include "../Container/data_value_container.h"
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
            typedef Geometry<TPointType>
                                                                            ClassType;
            typedef TPointType 
                                                                            PointType;
            typedef size_t
                                                                             SizeType;
            typedef size_t
                                                                            IndexType;
            typedef std::vector<PointType> 
                                                                  PointsContainerType;
            // @ Integration Points Define
            typedef Geometry_Data::IntegrationMethod
                                                                    IntegrationMethod;
            typedef Integration_Point<3> 
                                                                 IntegrationPointType;
            typedef std::vector<IntegrationPointType> 
                                                              IntegrationPointsVector;
            typedef std::array<IntegrationPointsVector, 
                                static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)>
                                                       IntegrationPointsContainerType;
            // @ ShapeFunctionValueContainer Define
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                               ShapeFunctionValueType;
            typedef std::array<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                                     ShapeFunctionsValueContainerType;

            // @ First derivatives/gradients
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> >
                                                          ShapeFunctionGradientsType;
            typedef std::array<ShapeFunctionGradientsType,
                                    static_cast<int>(Geometry_Data::IntegrationMethod::NumberofIntegrationMethods)> 
                                                  ShapeFunctionsGradientsContainerType;

            /* JacobiansType */
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > 
                                                                         JacobiansType;
            
        //}

        /// @name Life Circle 
        /// @{ 
            // Constructor  
                Geometry()
                {
                    number++;
                    ID = number;
                }
                Geometry(const int& id, 
                         PointsContainerType& Points)
                :pPoints(Points),
                 ID(id)
                {
                    number++;
                }
                Geometry(const int& id,
                         const PointsContainerType& Points,
                         const Geometry_Data& ThisGeometryData)
                :pPoints(Points),
                 ID(id),
                 mGeometry_Data(ThisGeometryData)
                {
                    number++;
                }
                Geometry(const PointsContainerType &ThisPoints,
                         Geometry_Data const *pThisGeometryData = &GeometryDataInstance())
                :mGeometry_Data(pThisGeometryData),
                 pPoints(ThisPoints)
                {
                    ID = ++number;
                }

                Geometry(PointsContainerType& Points)
                :pPoints(Points)
                {
                    number++;
                    ID = number;
                }
                Geometry(const Geometry<TPointType>& another)
                :ID(another.ID),
                 pPoints(another.pPoints),
                 mGeometry_Data(another.mGeometry_Data),
                 mData(another.mData)
                {
                    number++;
                }
                Geometry(Geometry<TPointType>&& another)
                :ID(another.ID),
                 pPoints(another.pPoints),
                 mGeometry_Data(another.mGeometry_Data),
                 mData(another.mData)
                {
                    number++;
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
            void SetData(const Data_Value_Container& mThisData)
            {
                mData = mThisData;
            }
            /**
             * @brief Creates a new geometry pointer
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            virtual Pointer Create(
                PointsContainerType const& rThisPoints
            ) const
            {
                // Create geometry
                auto p_geom = this->Create(0, rThisPoints);

                // Generate Id
                IndexType id = p_geom->GetID();

                // Sets second bit to zero.
                p_geom->SetIdSelfAssigned(id);

                // Sets first bit to zero.
                p_geom->SetIdNotGeneratedFromString(id);

                // Sets Id
                p_geom->SetIdWithoutCheck(id);

                return p_geom;
            }
            /**
             * @brief Creates a new geometry pointer
             * @param NewGeometryId the ID of the new geometry
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            virtual Pointer Create(
                const IndexType NewGeometryId,
                PointsContainerType const& rThisPoints
            ) const
            {
                return Pointer(new ClassType( NewGeometryId, rThisPoints, mGeometry_Data));
            }

            /**
             * @brief Creates a new geometry pointer
             * @param rNewGeometryName the name of the new geometry
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            Pointer Create(
                const std::string& rNewGeometryName,
                PointsContainerType const& rThisPoints
                ) const
            {
                auto p_geom = this->Create(0, rThisPoints);
                p_geom->SetId(rNewGeometryName);
                return p_geom;
            }

            /**
             * @brief Creates a new geometry pointer
             * @param rGeometry Reference to an existing geometry
             * @return Pointer to the new geometry
             */
            virtual Pointer Create(
                const ClassType& rGeometry
            ) const
            {
                // Create geometry
                auto p_geom = this->Create(0, rGeometry);

                // Generate Id
                IndexType id = (p_geom->GetID());

                // Sets second bit to zero.
                p_geom->SetIdSelfAssigned(id);

                // Sets first bit to zero.
                p_geom->SetIdNotGeneratedFromString(id);

                // Sets Id
                p_geom->SetIdWithoutCheck(id);

                return p_geom;
            }

            /**
             * @brief Creates a new geometry pointer
             * @param NewGeometryId the ID of the new geometry
             * @param rGeometry Reference to an existing geometry
             * @return Pointer to the new geometry
             */
            virtual Pointer Create(
                const IndexType NewGeometryId,
                const ClassType& rGeometry
            ) const
            {
                auto p_geometry = Pointer( new Geometry( NewGeometryId, rGeometry.pPoints, mGeometry_Data));
                p_geometry->SetData(rGeometry.GetData());
                return p_geometry;
            }

            /**
             * @brief Creates a new geometry pointer
             * @param rNewGeometryName the name of the new geometry
             * @param rGeometry Reference to an existing geometry
             * @return Pointer to the new geometry
             */
            Pointer Create(
                const std::string& rNewGeometryName,
                const ClassType& rGeometry
                ) const
            {
                auto p_geom = this->Create(0, rGeometry);
                p_geom->SetId(rNewGeometryName);
                return p_geom;
            }
        /// @}

        /// @name Access 
        /// @{
            Data_Value_Container& GetData()
            {
                return mData;
            }
            const Data_Value_Container& GetData() const
            {
                return mData;
            }
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
            IntegrationMethod GetDefaultIntegrationMethod() const
            {
                return mGeometry_Data.DefaultIntegrationMethod();
            }
            int PointsNumber() const
            {
                return pPoints.size();
            }
            const Geometry_Data& GetGeometryData() const
            {
                return mGeometry_Data;
            }
            const IntegrationPointsVector& IntegrationPoints(const IntegrationMethod& ThisMethod)const
            {
                return mGeometry_Data.IntegrationPoints(ThisMethod);
            }
            const ShapeFunctionValueType&
            ShapeFunctionsValues(const IntegrationMethod& ThisIntegrationMethod) const
            {
                return mGeometry_Data.ShapeFunctionsValues(ThisIntegrationMethod);
            }
            const ShapeFunctionGradientsType& 
            ShapeFunctionsLocalGradients( const IntegrationMethod& ThisMethod ) const
            {
                return mGeometry_Data.ShapeFunctionsLocalGradients( ThisMethod );
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
        const Geometry_Data mGeometry_Data;
        static const Geometry_Dimension mGeometry_Dimension;
        Data_Value_Container mData;

        /// @name Operations
        /// @{
            static const Geometry_Data& GeometryDataInstance()
            {
                IntegrationPointsContainerType integration_points = {};
                ShapeFunctionsValueContainerType shape_functions_values = {};
                ShapeFunctionsGradientsContainerType shape_functions_local_gradients = {};
                static Geometry_Data s_geometry_data(
                                        &mGeometry_Dimension,
                                        Geometry_Data::IntegrationMethod::Gauss_Legendre_1,
                                        integration_points,
                                        shape_functions_values,
                                        shape_functions_local_gradients);
                return s_geometry_data;
            }

            void SetIdWithoutCheck(const IndexType Id)
            {
                ID = Id;
            }
            /// Checks first bit in Id. 0 -> id; 1 -> name/ string
            static inline bool IsIdGeneratedFromString(IndexType Id)
            {
                return Id & (IndexType(1) << (sizeof(IndexType) * 8 - 1));
            }

            /// Sets first bit in Id to 1 -> name/ string
            static inline void SetIdGeneratedFromString(IndexType& Id)
            {
                Id |= (IndexType(1) << (sizeof(IndexType) * 8 - 1));
            }

            /// Sets first bit in Id to 0 -> no name/ string
            static inline void SetIdNotGeneratedFromString(IndexType& Id)
            {
                Id &= ~(IndexType(1) << (sizeof(IndexType) * 8 - 1));
            }

            /// Checks second bit in Id. 0 -> defined id; 1 -> self assigned
            static inline bool IsIdSelfAssigned(IndexType Id)
            {
                return Id & (IndexType(1) << (sizeof(IndexType) * 8 - 2));
            }

            /// Sets second bit in Id to 1 -> self assigned
            static inline void SetIdSelfAssigned(IndexType& Id)
            {
                Id |= (IndexType(1) << (sizeof(IndexType) * 8 - 2));
            }

            /// Sets second bit in Id to 0 -> not self assigned
            static inline void SetIdNotSelfAssigned(IndexType& Id)
            {
                Id &= ~(IndexType(1) << (sizeof(IndexType) * 8 - 2));
            }
        /// @}
};
template<typename TPointType> int Geometry<TPointType>::number = 0;
template<typename TPointType> typename Geometry<TPointType>::Geometry_Type Geometry<TPointType>::GeometryType = Geometry_Type::Origin;
template<typename TPointType> const Geometry_Dimension Geometry<TPointType>::mGeometry_Dimension = Geometry_Dimension(3,3);
#endif