#ifndef _SOLID_ELEMENT_H_
#define _SOLID_ELEMENT_H_
#include "element.h"
#include <Eigen/Eigen>
class Solid_Element : public Element
{
    public:
        /// @name Type Define
        /// @{
            typedef Constitutive_Law 
                                                        ConstitutiveLawType;
            typedef Properties  
                                                             PropertiesType;
            typedef Element
                                                                   BaseType;
            ///Pointer type for constitutive laws
            typedef ConstitutiveLawType::Pointer 
                                                 ConstitutiveLawPointerType;
            ///StressMeasure from constitutive laws
            typedef ConstitutiveLawType::StressMeasure 
                                                          StressMeasureType;
            ///Type definition for integration methods
            typedef Geometry_Data::IntegrationMethod 
                                                          IntegrationMethod;
            typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> 
                                                                     Matrix;
            typedef Eigen::Vector<double,Eigen::Dynamic> 
                                                                     Vector;

            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_RHS_VECTOR );
            LOTUS_DEFINE_LOCAL_FLAGS( COMPUTE_LHS_MATRIX );
            LOTUS_DEFINE_LOCAL_FLAGS( FINALIZED_STEP );

            struct ElementData
            {
            private:
                //variables including all integration points
                const GeometryType::ShapeFunctionsGradientsType* pDN_De;
                const Matrix* pNcontainer;
                const Process_Info* pProcessInfo;
            public:
                StressMeasureType StressMeasure;
                double  Tau;
                double  IntegrationWeight;
                //for axisymmetric use only
                double  CurrentRadius;
                double  ReferenceRadius;
                //general variables for large displacement use
                double  detF;
                double  detF0;
                double  detH; //Wildcard ( detF(0 to n+1) )
                double  detJ;
                Vector  StrainVector;
                Vector  StressVector;
                Vector  N;
                Matrix  B;
                Matrix  H;    //Wildcard ( Displacement Gradient, F(0 to n+1), B-bar, Velocity Gradient...)
                Matrix  F;    //Incremental Deformation Gradient (n to n+1)
                Matrix  F0;   //Historical Deformation Gradient  (0 to n)
                Matrix  DN_DX;
                Matrix  ConstitutiveMatrix;
                //variables including all integration points
                GeometryType::JacobiansType J;
                GeometryType::JacobiansType j;
                Matrix  DeltaPosition;

                /**
                 * sets the value of a specified pointer variable
                 */
                void SetShapeFunctionsGradients(const GeometryType::ShapeFunctionsGradientsType &rDN_De)
                {
                    pDN_De=&rDN_De;
                };
                void SetShapeFunctions(const Matrix& rNcontainer)
                {
                    pNcontainer=&rNcontainer;
                };
                void SetProcessInfo(const Process_Info& rProcessInfo)
                {
                    pProcessInfo=&rProcessInfo;
                };

                /**
                 * returns the value of a specified pointer variable
                 */
                const GeometryType::ShapeFunctionsGradientsType& GetShapeFunctionsGradients()
                {
                    return *pDN_De;
                };

                const Matrix& GetShapeFunctions()
                {
                    return *pNcontainer;
                };

                const Process_Info& GetProcessInfo()
                {
                    return *pProcessInfo;
                };

                void Initialize( const unsigned int& voigt_size,
                    const unsigned int& dimension,
                    const unsigned int& number_of_nodes )
                {
                    StressMeasure = Constitutive_Law::StressMeasure_PK2;

                    //stabilization
                    Tau = 0;

                    //time step
                    IntegrationWeight = 1;

                    //radius
                    CurrentRadius = 0;
                    ReferenceRadius = 0;

                    //jacobians
                    detF  = 1;
                    detF0 = 1;
                    detH  = 1;
                    detJ  = 1;

                    //vectors
                    StrainVector.resize(voigt_size);
                    StrainVector.setZero(voigt_size);
                    StressVector.resize(voigt_size);
                    StressVector.setZero(voigt_size);
                    N.resize(number_of_nodes);
                    N.setZero(voigt_size);

                    //matrices
                    B.resize(voigt_size, dimension*number_of_nodes);
                    H.resize(dimension,dimension);
                    F.resize(dimension,dimension);
                    F0.resize(dimension,dimension);
                    DN_DX.resize(number_of_nodes, dimension);
                    ConstitutiveMatrix.resize(voigt_size, voigt_size);
                    DeltaPosition.resize(number_of_nodes, dimension);

                    B.setZero();
                    H.setZero();
                    F.setZero();
                    F0.setZero();
                    DN_DX.setZero();
                    ConstitutiveMatrix.setZero();
                    DeltaPosition.setZero();

                    //others
                    J.resize(1);
                    j.resize(1);
                    J[0].resize(dimension,dimension);
                    j[0].resize(dimension,dimension);
                    J[0].setZero();
                    J[0].setZero();

                    //pointers
                    pDN_De = NULL;
                    pNcontainer = NULL;
                    pProcessInfo = NULL;
                }
            };

            struct LocalSystemComponents
            {
                private:
                    //for calculation local system with compacted LHS and RHS
                    Matrix* mpLeftHandSideMatrix;
                    Vector* mpRightHandSideVector;
                public:
                    //calculation flags
                    Flags  CalculationFlags;
                    /**
                     * sets the value of a specified pointer variable
                     */
                    void SetLeftHandSideMatrix( Matrix& rLeftHandSideMatrix ) { mpLeftHandSideMatrix = &rLeftHandSideMatrix; };
                    void SetRightHandSideVector( Vector& rRightHandSideVector ) { mpRightHandSideVector = &rRightHandSideVector; };
                    /**
                     * returns the value of a specified pointer variable
                     */
                    Matrix& GetLeftHandSideMatrix() { return *mpLeftHandSideMatrix; };
                    Vector& GetRightHandSideVector() { return *mpRightHandSideVector; };
            };

        /// @}


        /// @name Life Circle
        /// @{
            Solid_Element()
            {

            }
            Solid_Element(int NewId, GeometryType::Pointer pGeometry)
            :BaseType(NewId,pGeometry)
            {

            }

            Solid_Element(int NewId, GeometryType::Pointer pGeometry, typename PropertiesType::Pointer pProperties)
            :BaseType(NewId,pGeometry,*pProperties)
            {

            }

            ///Copy constructor
            Solid_Element(Solid_Element& another)
            :BaseType(another),
             mThisIntegrationMethod(another.mThisIntegrationMethod),
             mConstitutiveLawVector(another.mConstitutiveLawVector)
            {

            }
            Solid_Element(Solid_Element&& another)
            :BaseType(another),
             mThisIntegrationMethod(another.mThisIntegrationMethod),
             mConstitutiveLawVector(another.mConstitutiveLawVector)
            {
            
            }
            /// Destructor.
            ~Solid_Element() override;
        /// @}


        /// @name Operators
        /// @{
            Solid_Element& operator=(const Solid_Element& another)
            {
                this->mpGeometry = another.mpGeometry;
                this->mThisIntegrationMethod = another.mThisIntegrationMethod;

                this->mConstitutiveLawVector.clear();
                this->mConstitutiveLawVector.resize(another.mConstitutiveLawVector.size());
                for(auto item:another.mConstitutiveLawVector)
                    this->mConstitutiveLawVector.push_back(item);
                return *this; 
            }
        /// @}


        /// @name Operations
        /// @{
            void GetDofList(DofsVectorType& rElementalDofList, const Process_Info& rCurrentProcessInfo) const override
            {
                rElementalDofList.resize(0);
                const int dimension = GetGeometry().WorkingSpaceDimension();

                for (int i=0;i<GetGeometry().GetPointsNum();++i)
                {
                    
                }
            }

        /// @}


        /// @name Access
        /// @{
            Solid_Element::IntegrationMethod GetIntegrationMethod() const
            {
                return mThisIntegrationMethod;
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
            IntegrationMethod mThisIntegrationMethod;
            std::vector<Constitutive_Law::Pointer> mConstitutiveLawVector;
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


        /// @}


        /// @name Private Operatiors
        /// @{


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

#endif