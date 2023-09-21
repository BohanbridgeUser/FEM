#ifndef _POINT3D_H_
#define _POINT3D_H_
#include "geometry.h"

template<typename TPointType>
class Point3D : public Geometry<TPointType>
{
    public:
        /// @name Type Define
        /// @{
            LOTUS_POINTER_DEFINE(Point3D)

            typedef Geometry<TPointType>
                                                                    BaseType;
            typedef Geometry_Data::IntegrationMethod 
                                                           IntegrationMethod;
            typedef TPointType 
                                                                   PointType;
            typedef typename BaseType::IndexType 
                                                                   IndexType;
            typedef typename BaseType::SizeType 
                                                                    SizeType;
            typedef  typename BaseType::PointsContainerType 
                                                             PointsArrayType;


            typedef typename BaseType::IntegrationPointType 
                                                        IntegrationPointType;
            typedef typename BaseType::IntegrationPointsVector 
                                                  IntegrationPointsArrayType;
            typedef typename BaseType::IntegrationPointsContainerType 
                                              IntegrationPointsContainerType;

            typedef typename BaseType::ShapeFunctionsValueContainerType 
                                           ShapeFunctionsValuesContainerType;

            typedef typename BaseType::ShapeFunctionGradientsType 
                                                 ShapeFunctionsGradientsType;
            typedef typename BaseType::ShapeFunctionsGradientsContainerType 
                                   ShapeFunctionsLocalGradientsContainerType;

            typedef typename BaseType::JacobiansType 
                                                               JacobiansType;
            
            typedef typename BaseType::NormalType 
                                                                  NormalType;
        /// @}


        /// @name Life Circle
        /// @{
            Point3D(typename PointType::Pointer pFirstPoint)
            :BaseType(PointsArrayType(), msGeometryData)
            {
                BaseType::pPointsVector().push_back(pFirstPoint);
            }
            Point3D(const PointsArrayType& ThisPoints)
            :BaseType(ThisPoints, msGeometryData)
            {
                if( BaseType::PointsNumber() != 1)
                {
                    std::cerr << "Invalid points number. Expected 2, given " << BaseType::PointsNumber() << std::endl;
                    exit(0);
                } 
            }
            /// Constructor with Geometry Id
            explicit Point3D(
                const IndexType GeometryId,
                const PointsArrayType& rThisPoints)
             : BaseType(GeometryId, rThisPoints, msGeometryData)
            {
                if( this->PointsNumber() != 1 )
                {
                    std::cerr << "Invalid points number. Expected 1, given " << this->PointsNumber() << std::endl;
                    exit(0);
                } 
            }
            /// Constructor with Geometry Name
            explicit Point3D(
                const std::string& rGeometryName,
                const PointsArrayType& rThisPoints)
             : BaseType(rGeometryName, rThisPoints, msGeometryData)
            {
                if(this->PointsNumber() != 1)
                {
                    std::cerr << "Invalid points number. Expected 1, given " << this->PointsNumber() << std::endl;
                    exit(0);
                } 
            }
            /** Copy constructor.
            Construct this geometry as a copy of given geometry.

            @note This copy constructor don't copy the points and new
            geometry shares points with given source geometry. It's
            obvious that any change to this new geometry's point affect
            source geometry's points too.
            */
            Point3D(Point3D const& rOther)
                : BaseType(rOther)
            {
            }

            /** Copy constructor from a geometry with other point type.
            Construct this geometry as a copy of given geometry which
            has different type of points. The given goemetry's
            TOtherPointType* must be implicity convertible to this
            geometry PointType.

            @note This copy constructor don't copy the points and new
            geometry shares points with given source geometry. It's
            obvious that any change to this new geometry's point affect
            source geometry's points too.
            */
            template<class TOtherPointType> 
            Point3D(Point3D<TOtherPointType> const& rOther)
            : BaseType(rOther)
            {
            }
            /// Destructor. Do nothing!!!
            ~Point3D() override {}

        /// @}


        /// @name Operators
        /// @{
             /** Assignment operator.

            @note This operator don't copy the points and this
            geometry shares points with given source geometry. It's
            obvious that any change to this geometry's point affect
            source geometry's points too.

            @see Clone
            @see ClonePoints
            */
            Point3D& operator=(const Point3D& rOther)
            {
                BaseType::operator=(rOther);
                return *this;
            }

            /** Assignment operator for geometries with different point type.

            @note This operator don't copy the points and this
            geometry shares points with given source geometry. It's
            obvious that any change to this geometry's point affect
            source geometry's points too.

            @see Clone
            @see ClonePoints
            */
            template<class TOtherPointType>
            Point3D& operator=(Point3D<TOtherPointType> const & rOther)
            {
                BaseType::operator=(rOther);
                return *this;
            }


        /// @}


        /// @name Operations
        /// @{
             /**
             * @brief Creates a new geometry pointer
             * @param NewGeometryId the ID of the new geometry
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            typename BaseType::SharedPointer Create(
                const IndexType NewGeometryId,
                PointsArrayType const& rThisPoints
                ) const override
            {
                return typename BaseType::SharedPointer( new Point3D( NewGeometryId, rThisPoints ) );
            }

            /**
             * @brief Creates a new geometry pointer
             * @param NewGeometryId the ID of the new geometry
             * @param rGeometry reference to an existing geometry
             * @return Pointer to the new geometry
             */
            typename BaseType::SharedPointer Create(
                const IndexType NewGeometryId,
                const BaseType& rGeometry
            ) const override
            {
                auto p_geometry = typename BaseType::SharedPointer( new Point3D( NewGeometryId, rGeometry.pPointsVector() ) );
                p_geometry->SetData(rGeometry.GetData());
                return p_geometry;
            }


        /// @}


        /// @name Access
        /// @{
            

        /// @}


        /// @name Inquiry
        /// @{
            /** This method calculate and return Length or charactereistic
            length of this geometry depending to it's dimension. For one
            dimensional geometry for example Line it returns length of it
            and for the other geometries it gives Characteristic length
            otherwise.
            @return double value contains length or Characteristic
            length
            @see Area()
            @see Volume()
            @see DomainSize()
            */
            double Length() const override
            {
                return 0.00;
            }
            /** This method calculate and return area or surface area of
            this geometry depending to it's dimension. For one dimensional
            geometry it returns zero, for two dimensional it gives area
            and for three dimensional geometries it gives surface area.
            @return double value contains area or surface
            area.
            @see Length()
            @see Volume()
            @see DomainSize()
            */
            double Area() const override
            {
                return 0.00;
            }
            /** This method calculate and return length, area or volume of
            this geometry depending to it's dimension. For one dimensional
            geometry it returns its length, for two dimensional it gives area
            and for three dimensional geometries it gives its volume.
            @return double value contains length, area or volume.
            @see Length()
            @see Area()
            @see Volume()
            */
            double DomainSize() const override
            {
                return 0.00;
            }
            /** EdgesNumber
            @return SizeType containes number of this geometry edges.
            */
            SizeType EdgesNumber() const override
            {
                return 1;
            }
            SizeType FacesNumber() const override
            {
                return 0;
            }


        /// @}

        /// @name Input And Output
        /// @{
            /** Turn back information as a string.
            @return String contains information about this geometry.
            @see PrintData()
            @see PrintInfo()
            */
            std::string Info() const override
            {
                return "Info:a point in 3D space ";
            }
            /** Print information about this object.
            @param rOStream Stream to print into it.
            @see PrintData()
            @see Info()
            */
            void PrintInfo(std::ostream& rOStream) const override
            {
                rOStream << "Info:a point in 3D space \n";
            }
            /** Print geometry's data into given stream. Prints it's points
            by the order they stored in the geometry and then center
            point of geometry.
            @param rOStream Stream to print into it.
            @see PrintInfo()
            @see Info()
            */
            void PrintData(std::ostream& rOStream) const override
            {
                rOStream << "Data: ";
                rOStream << this->pPointsVector()[0] << std::endl;
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
            static const Geometry_Data msGeometryData;

            static const Geometry_Dimension msGeometryDimension;

        /// @}


        /// @name Private Member Variables
        /// @{


        /// @}


        /// @name Private Operatiors
        /// @{


        /// @}


        /// @name Private Operations
        /// @{
            static const IntegrationPointsContainerType AllIntegrationPoints()
            {
                IntegrationPointsContainerType integration_points;
                return integration_points;
            }
            static const ShapeFunctionsValuesContainerType AllShapeFunctionsValues()
            {
                ShapeFunctionsValuesContainerType shape_functions_value;
                return shape_functions_value;
            }
            static const ShapeFunctionsLocalGradientsContainerType AllShapeFunctionsLocalGradients()
            {
                ShapeFunctionsLocalGradientsContainerType shape_functions_local_gradients;
                return shape_functions_local_gradients;
            }

        /// @}


        /// @name Private Access
        /// @{


        /// @}


        /// @name Private Inquiry
        /// @{


        /// @}

        /// @name friend class
        /// @{
            template<class TOtherPointType> friend class Point3D;
        /// @}
};

/// output stream function
template<class TPointType>
inline std::ostream& operator << (std::ostream& rOStream,
                                  const Point3D<TPointType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}

template<class TPointType>
const Geometry_Data Point3D<TPointType>::msGeometryData(
        &msGeometryDimension,
        Geometry_Data::IntegrationMethod::Gauss_Legendre_2,
        Point3D<TPointType>::AllIntegrationPoints(),
        Point3D<TPointType>::AllShapeFunctionsValues(),
        AllShapeFunctionsLocalGradients());

template<class TPointType>
const Geometry_Dimension Point3D<TPointType>::msGeometryDimension(3, 0);
#endif