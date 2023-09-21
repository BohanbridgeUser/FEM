#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_
#include "../define.h"
#include "geometry_dimension.h"
#include "../Container/points_container.h"
#include "geometry_data.h"
#include "../Quadrature/integration_point.h"
#include "../Container/data_value_container.h"
#include "../Utility/math_utility.h"
#include "../Container/pointer_vector.h"

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

            LOTUS_SHARED_POINTER_DEFINE(Geometry<TPointType>)
            typedef Geometry<TPointType>
                                                                            ClassType;
            typedef TPointType 
                                                                            PointType;
            typedef size_t
                                                                             SizeType;
            typedef size_t
                                                                            IndexType;
            typedef PointerVector<PointType> 
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
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                               Matrix;
            typedef Eigen::Matrix<double,Eigen::Dynamic,1>
                                                                               Vector;
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
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                          JacobianType;
            typedef std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> > 
                                                                         JacobiansType;
            typedef Vector
                                                                            NormalType;

            /// PointsArrayType typedefs
            typedef typename PointsContainerType::iterator 
                                                                             iterator;
            typedef typename PointsContainerType::const_iterator 
                                                                       const_iterator;
            typedef typename PointsContainerType::ptr_iterator 
                                                                         ptr_iterator;
            typedef typename PointsContainerType::ptr_const_iterator 
                                                                   ptr_const_iterator;
            typedef typename PointsContainerType::difference_type 
                                                                      difference_type;
        //}

        /// @name Life Circle 
        /// @{ 
            // Constructor  
                Geometry()
                :mGeometry_Data(&GeometryDataInstance())
                {
                    number++;
                    ID = number;
                }
                Geometry(const IndexType& id, 
                         PointsContainerType& Points)
                :pPoints(Points),
                 ID(id),
                 mGeometry_Data(&GeometryDataInstance())
                {
                    number++;
                }
                Geometry(const IndexType& id,
                         const PointsContainerType& Points,
                         const Geometry_Data& ThisGeometryData)
                :pPoints(Points),
                 ID(id),
                 mGeometry_Data(&ThisGeometryData)
                {
                    number++;
                }
                Geometry(const PointsContainerType& Points,
                         Geometry_Data const& pThisGeometryData = GeometryDataInstance())
                :pPoints(Points),
                mGeometry_Data(&pThisGeometryData)
                {
                    ID = ++number;
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
                Geometry(Geometry<TPointType>::Pointer another)
                :ID(another->ID),
                 pPoints(another->pPoints),
                 mGeometry_Data(another->mGeometry_Data),
                 mData(another->mData)
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
            const PointType& operator[](const int& index) const
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

             /// Sets Id of this Geometry
            void SetId(const IndexType Id)
            {
                // The first bit of the Id is used to detect if Id
                // is int or hash of name. Second bit defines if Id
                // is self assigned or not.
                if(IsIdGeneratedFromString(Id)
                    || IsIdSelfAssigned(Id))
                {
                    std::cerr<< "Id: " << Id << " out of range. The Id must me lower than 2^62 = 4.61e+18. "
                    << "Geometry being recognized as generated from string: " << IsIdGeneratedFromString(Id)
                    << ", self assigned: " << IsIdSelfAssigned(Id) << "."
                    << std::endl;
                    exit(0);
                }
                ID = Id;
            }
            /// Sets Id with the use of the name of this geometry
            void SetId(const std::string& rName)
            {
                ID = GenerateId(rName);
            }

    /// Gets the correspond
            /**
             * @brief Creates a new geometry pointer
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            virtual ClassType::SharedPointer Create(PointsContainerType const& rThisPoints) const
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
            virtual ClassType::SharedPointer Create(const IndexType NewGeometryId,PointsContainerType const& rThisPoints) const
            {
                return std::make_shared<ClassType>(NewGeometryId, rThisPoints, *mGeometry_Data);
            }
            /**
             * @brief Creates a new geometry pointer
             * @param rNewGeometryName the name of the new geometry
             * @param rThisPoints the nodes of the new geometry
             * @return Pointer to the new geometry
             */
            ClassType::SharedPointer Create(const std::string& rNewGeometryName,PointsContainerType const& rThisPoints) const
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
            virtual ClassType::SharedPointer Create(const ClassType& rGeometry) const
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
            virtual ClassType::SharedPointer Create(const IndexType NewGeometryId,const ClassType& rGeometry) const
            {
                auto p_geometry = std::make_shared<ClassType>(NewGeometryId, rGeometry.pPoints, *mGeometry_Data);
                p_geometry->SetData(rGeometry.GetData());
                return p_geometry;
            }
            /**
             * @brief Creates a new geometry pointer
             * @param rNewGeometryName the name of the new geometry
             * @param rGeometry Reference to an existing geometry
             * @return Pointer to the new geometry
             */
            ClassType::SharedPointer Create(const std::string& rNewGeometryName,const ClassType& rGeometry) const
            {
                auto p_geom = this->Create(0, rGeometry);
                p_geom->SetId(rNewGeometryName);
                return p_geom;
            }

            virtual int Check() const
            {
                return 0;
            }
        
            /**
             * @name Jacobian 
             * @brief Calculate Jacobian
            */
            JacobiansType& Jacobian( JacobiansType& rResult ) const
            {
                Jacobian( rResult, mGeometry_Data->GetIntegrationMethod() );
                return rResult;
            }
            virtual JacobiansType& Jacobian( JacobiansType& rResult,
                                     IntegrationMethod ThisMethod ) const
            {
                if( rResult.size() != this->IntegrationPointsNumber( ThisMethod ) )
                    rResult.resize( this->IntegrationPointsNumber( ThisMethod ));
                for ( unsigned int pnt = 0; pnt < this->IntegrationPointsNumber( ThisMethod ); pnt++ ) {
                    this->Jacobian( rResult[pnt], pnt, ThisMethod);
                }
                return rResult;
            }
            virtual JacobianType& Jacobian( JacobianType& rResult, IndexType IntegrationPointIndex, IntegrationMethod ThisMethod ) const
            {
                const SizeType working_space_dimension = this->WorkingSpaceDimension();
                const SizeType local_space_dimension = this->LocalSpaceDimension();
                if(rResult.rows() != working_space_dimension || rResult.cols() != local_space_dimension)
                    rResult.resize( working_space_dimension, local_space_dimension);

                const Matrix& r_shape_functions_gradient_in_integration_point = ShapeFunctionsLocalGradients( ThisMethod )[ IntegrationPointIndex ];

                rResult.setZero(rResult.rows(),rResult.cols());
                const SizeType points_number = this->PointsNumber();
                for (IndexType i = 0; i < points_number; ++i ) 
                {
                    const std::array<double, 3>& r_coordinates = (*this)[i].Coordinates();
                    for(IndexType k = 0; k< working_space_dimension; ++k) 
                    {
                        const double value = r_coordinates[k];
                        for(IndexType m = 0; m < local_space_dimension; ++m) 
                        {
                            rResult(k,m) += value * r_shape_functions_gradient_in_integration_point(i,m);
                        }
                    }
                }
                return rResult;
            }
            /** Jacobian in given point. This method calculate jacobian
            matrix in given point.

            @param rCoordinates point which jacobians has to
            be calculated in it.

            @return Matrix of double which is jacobian matrix \f$ J \f$ in given point.

            @see DeterminantOfJacobian
            @see InverseOfJacobian
            */
            virtual JacobianType& Jacobian( JacobianType& rResult, PointType const& rCoordinates ) const
            {
                const SizeType working_space_dimension = this->WorkingSpaceDimension();
                const SizeType local_space_dimension = this->LocalSpaceDimension();
                const SizeType points_number = this->PointsNumber();
                if(rResult.rows() != working_space_dimension || rResult.cols() != local_space_dimension)
                    rResult.resize( working_space_dimension, local_space_dimension );
                JacobianType shape_functions_gradients(points_number, local_space_dimension);
                ShapeFunctionsLocalGradients( shape_functions_gradients, rCoordinates );
                rResult.setZero();
                for (IndexType i = 0; i < points_number; ++i ) 
                {
                    const std::array<double, 3>& r_coordinates = (*this)[i].Coordinates();
                    for(IndexType k = 0; k< working_space_dimension; ++k) 
                    {
                        const double value = r_coordinates[k];
                        for(IndexType m = 0; m < local_space_dimension; ++m) 
                        {
                            rResult(k,m) += value * shape_functions_gradients(i,m);
                        }
                    }
                }
                return rResult;
            }   
            /** Jacobians for given  method. This method
            calculate jacobians matrices in all integrations points of
            given integration method.

            @param ThisMethod integration method which jacobians has to
            be calculated in its integration points.

            @return JacobiansType a Vector of jacobian
            matrices \f$ J_i \f$ where \f$ i=1,2,...,n \f$ is the integration
            point index of given integration method.

            @param DeltaPosition Matrix with the nodes position increment which describes
            the configuration where the jacobian has to be calculated.

            @see DeterminantOfJacobian
            @see InverseOfJacobian
            */
            virtual JacobiansType& Jacobian( JacobiansType& rResult, IntegrationMethod ThisMethod, Matrix & DeltaPosition ) const
            {
                if( rResult.size() != this->IntegrationPointsNumber( ThisMethod ) )
                    rResult.resize( this->IntegrationPointsNumber( ThisMethod ));

                for ( unsigned int pnt = 0; pnt < this->IntegrationPointsNumber( ThisMethod ); pnt++ ) {
                    this->Jacobian( rResult[pnt], pnt, ThisMethod, DeltaPosition);
                }
                return rResult;
            }
            /** Jacobian in specific integration point of given integration
            method. This method calculate jacobian matrix in given
            integration point of given integration method.

            @param IntegrationPointIndex index of integration point which jacobians has to
            be calculated in it.

            @param ThisMethod integration method which jacobians has to
            be calculated in its integration points.

            @param rDeltaPosition Matrix with the nodes position increment which describes
            the configuration where the jacobian has to be calculated.

            @return Matrix<double> Jacobian matrix \f$ J_i \f$ where \f$
            i \f$ is the given integration point index of given
            integration method.

            @see DeterminantOfJacobian
            @see InverseOfJacobian
            */
            virtual Matrix& Jacobian( Matrix& rResult, IndexType IntegrationPointIndex, IntegrationMethod ThisMethod, const Matrix& rDeltaPosition ) const
            {
                const SizeType working_space_dimension = this->WorkingSpaceDimension();
                const SizeType local_space_dimension = this->LocalSpaceDimension();
                if(rResult.rows() != working_space_dimension || rResult.cols() != local_space_dimension)
                    rResult.resize( working_space_dimension, local_space_dimension);

                const Matrix& r_shape_functions_gradient_in_integration_point = ShapeFunctionsLocalGradients( ThisMethod )[ IntegrationPointIndex ];

                rResult.setZero();
                const SizeType points_number = this->PointsNumber();
                for (IndexType i = 0; i < points_number; ++i ) {
                    const std::array<double, 3>& r_coordinates = (*this)[i].Coordinates();
                    for(IndexType k = 0; k< working_space_dimension; ++k) {
                        const double value = r_coordinates[k] - rDeltaPosition(i,k);
                        for(IndexType m = 0; m < local_space_dimension; ++m) {
                            rResult(k,m) += value * r_shape_functions_gradient_in_integration_point(i,m);
                        }
                    }
                }
                return rResult;
            }
            Vector& DeterminantOfJacobian(Vector& rResult)const
            {
                DeterminantOfJacobian( rResult, mGeometry_Data->GetIntegrationMethod() );
                return rResult;
            }
            virtual Vector& DeterminantOfJacobian(Vector& rResult, IntegrationMethod const& rThisMethod) const
            {
                if(rResult.size() != this->IntegrationPointsNumber(rThisMethod))
                    rResult.resize(this->IntegrationPointsNumber(rThisMethod));
                
                Matrix J(this->WorkingSpaceDimension(),this->LocalSpaceDimension());
                for (unsigned int pnt=0;pnt<this->IntegrationPointsNumber(rThisMethod);++pnt)
                {
                    this->Jacobian(J,pnt,rThisMethod);
                    rResult[pnt] = GerneralizedDet(J);
                }
                return rResult;
            }
            virtual Matrix& ShapeFunctionsLocalGradients( Matrix& rResult, const PointType& rPoint ) const
            {
                std::cerr << "Calling base class ShapeFunctionsLocalGradients method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
                return rResult;
            }
            

            /// Gets the corresponding hash-Id to a string name
            static inline IndexType GenerateId(const std::string& rName)
            {
                // Create id hash from provided name.
                std::hash<std::string> string_hash_generator;
                auto id = string_hash_generator(rName);

                // Sets first bit to one.
                SetIdGeneratedFromString(id);

                // Sets second bit to zero.
                SetIdNotSelfAssigned(id);

                return id;
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
            PointsContainerType const& pPointsVector() const
            {
                return pPoints;
            }
            IndexType& GetID()
            {
                return ID;
            }
            IndexType GetID() const
            {
                return ID;
            }
            IndexType const& Id() const
            {
                return ID;
            }
            static Geometry_Type GetType()
            {
                return GeometryType;
            }
            IntegrationMethod GetDefaultIntegrationMethod() const
            {
                return mGeometry_Data->DefaultIntegrationMethod();
            }
            int PointsNumber() const
            {
                return pPoints.size();
            }
            const Geometry_Data& GetGeometryData() const
            {
                return *mGeometry_Data;
            }
            const IntegrationPointsVector& IntegrationPoints(const IntegrationMethod& ThisMethod)const
            {
                return mGeometry_Data->IntegrationPoints(ThisMethod);
            }
            int IntegrationPointsNumber(const IntegrationMethod& ThisIntegrationMethod) const
            {
                return mGeometry_Data->IntegrationPoints(ThisIntegrationMethod).size();
            }
            const ShapeFunctionValueType&
            ShapeFunctionsValues(const IntegrationMethod& ThisIntegrationMethod) const
            {
                return mGeometry_Data->ShapeFunctionsValues(ThisIntegrationMethod);
            }
            const ShapeFunctionGradientsType& 
            ShapeFunctionsLocalGradients( const IntegrationMethod& ThisMethod ) const
            {
                return mGeometry_Data->ShapeFunctionsLocalGradients( ThisMethod );
            }

        /// @}


        /// @name Inquiry
        /// @{ 
            int size()const
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
             /**
             * @brief This method calculate and return length, area or volume of this geometry depending to it's dimension.
             * @details For one dimensional geometry it returns its length, for two dimensional it gives area and for three dimensional geometries it gives its volume.
             * @return double value contains length, area or volume.
             * @see Length()
             * @see Area()
             * @see Volume()
             */
            virtual double DomainSize() const 
            {
                const SizeType local_dimension = this->LocalSpaceDimension();
                if (local_dimension == 1) { // 1D geometry
                    return this->Length();
                } else if (local_dimension == 2) { // 2D geometry
                    return this->Area();
                } else { // 3D geometry
                    return this->Volume();
                }
                return 0.0;
            }


            /** This method calculate and return Length or charactereistic
             * length of this geometry depending to it's dimension. For one
             * dimensional geometry for example Line it returns length of it
             * and for the other geometries it gives Characteristic length
             * otherwise.
             *
             * @return double value contains length or Characteristic
             * length
             *
             * @see Area()
             * @see Volume()
             * @see DomainSize()
             */
            virtual double Length() const 
            {
                std::cerr << "Calling base class 'Length' method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
                return 0.0;
            }

            /**
             * @brief This method calculate and return area or surface area of this geometry depending to it's dimension.
             * @details For one dimensional geometry it returns length, for two dimensional it gives area and for three dimensional geometries it gives surface area.
             * @return double value contains area or surface area.
             * @see Length()
             * @see Volume()
             * @see DomainSize()
             */
            virtual double Area() const 
            {
                std::cerr << "Calling base class 'Area' method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
                return 0.0;
            }

            /**
             * @brief This method calculate and return volume of this geometry.
             * @details For one and two dimensional geometry it returns zero and for three dimensional it gives volume of geometry.
             * @return double value contains volume.
             * @see Length()
             * @see Area()
             * @see DomainSize()
             */
            virtual double Volume() const 
            {
                std::cerr << "Calling base class 'Volume' method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
                return 0.0;
            }

            /**
             * @brief This method gives you number of all edges of this geometry.
             * @details For example, for a hexahedron, this would be 12
             * @return SizeType containes number of this geometry edges.
             * @see EdgesNumber()
             * @see Edges()
             * @see GenerateEdges()
             * @see FacesNumber()
             * @see Faces()
             * @see GenerateFaces()
             */
            virtual SizeType EdgesNumber() const
            {
                std::cerr << "Calling base class EdgesNumber method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
            }
             /**
             * @brief Returns the number of faces of the current geometry.
             * @details This is only implemented for 3D geometries, since 2D geometries only have edges but no faces
             * @see EdgesNumber
             * @see Edges
             * @see Faces
             */
            virtual SizeType FacesNumber() const
            {
                std::cerr << "Calling base class FacesNumber method instead of derived class one. Please check the definition of derived class. " << *this << std::endl;
                exit(0);
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
            virtual std::string Info() const
            {
                std::stringstream sstream;
                sstream << "Geometry Class\n";
                return sstream.str();
            }
            virtual void PrintInfo(std::ostream& os) const
            {
                os << "Geometry # " << ID << std::endl;
            }
            virtual void PrintData(std::ostream& os) const
            {
                os << "Geometry # " << ID << std::endl;
                for(auto it=pPoints.begin();it<pPoints.end();++it)
                {
                    os << "Point " << it-pPoints.begin() << " " << (*it);
                }
            }
        /// @}
    protected:
        /// @name Protected Static Member 
        /// @{

        /// @}

        /// @name Protected Member 
        /// @{
            IndexType ID;
        /// @}
        
        /// @name Protected Operators
        /// @{

        /// @}

        /// @name Protected Operations
        /// @{
            /**
             * @brief Checks if the geometry points are valid
             * Checks if the geometry points are valid from the pointer value
             * Points are not valid when the pointer value is null
             * @return true All points are valid
             * @return false At least one point has nullptr value
             */
            // bool AllPointsAreValid() const
            // {
            //     return std::none_of(pPoints.begin(), pPoints.end(), [](const auto& ppPoint){return ppPoint == nullptr;});
            // }

        /// @}
    private:
        PointsContainerType pPoints;
        static Geometry_Type GeometryType; 
        static int number;
        Geometry_Data const* mGeometry_Data;
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
template<typename TPointType> 
int Geometry<TPointType>::number = 0;
template<typename TPointType> 
typename Geometry<TPointType>::Geometry_Type Geometry<TPointType>::GeometryType = Geometry_Type::Origin;
template<typename TPointType> 
const Geometry_Dimension Geometry<TPointType>::mGeometry_Dimension = Geometry_Dimension(3,3);
#endif