#include "../../include/solid_mechanics_element/small_displacement_element.h"

/// @brief public:
/// @name Type Define
/// @{


/// @}


/// @name Life Circle
/// @{
    Small_Displacement_Element::Small_Displacement_Element()
    :Solid_Element()
    {
    }
    Small_Displacement_Element::Small_Displacement_Element(IndexType NewId,
                                                        GeometryType::Pointer pThisGeometry)
    :Solid_Element(NewId,pThisGeometry)
    {

    }
    Small_Displacement_Element::Small_Displacement_Element(IndexType NewId,
                                                        GeometryType::Pointer pThisGeometry,
                                                        Properties::Pointer pThisProperties)
    :Solid_Element(NewId,pThisGeometry,pThisProperties)
    {

    }
    Small_Displacement_Element::Small_Displacement_Element(Small_Displacement_Element const& another)
    :Solid_Element(another)
    {
    }
    Small_Displacement_Element::~Small_Displacement_Element()
    {
    }

/// @}


/// @name Operators
/// @{
    Small_Displacement_Element& Small_Displacement_Element::operator=(Small_Displacement_Element const& another)
    {
        Solid_Element::operator=(another);
        return *this;
    }

/// @}


/// @name Operations
/// @{
    Element::SharedPointer Small_Displacement_Element::Create(IndexType NewId,
                                                              NodesContainerType const& rNodes,
                                                              Properties::Pointer pProperties) const
    {
        return std::make_shared<Small_Displacement_Element>(NewId,GetGeometry().Create(rNodes),pProperties);
    }
    Element::SharedPointer Small_Displacement_Element::Clone(IndexType NewId,
                                                             NodesContainerType const& rNodes) const
    {
        return std::make_shared<Small_Displacement_Element>(NewId,GetGeometry().Create(rNodes),pGetProperties());
    }
    
    void Small_Displacement_Element::SetElementData(ElementDataType& rVariables,
                                                    Constitutive_Law::Parameters& rValues,
                                                    const int & rPointNumber)
    {
        rValues.SetStrainVector(rVariables.StrainVector);
        rValues.SetStressVector(rVariables.StressVector);
        rValues.SetConstitutiveMatrix(rVariables.ConstitutiveMatrix);
        rValues.SetShapeFunctionsDerivatives(rVariables.DN_DX);
        rValues.SetShapeFunctionsValues(rVariables.N);

        if(rVariables.detJ<0)
        {
            std::cerr << " (small displacement) ELEMENT INVERTED |J|<0 : " << rVariables.detJ << std::endl;
            exit(0);
        }

        rValues.SetDeterminantF(rVariables.detF);
        rValues.SetDeformationGradientF(rVariables.F);
    }

    void Small_Displacement_Element::CalculateAndAddKuug(MatrixType& rLeftHandSideMatrix,
						                                 ElementDataType& rVariables,
						                                 double& rIntegrationWeight)

    {
        // small displacement element is linear no geometric stiffness present
    }

    void Small_Displacement_Element::InitializeElementData (ElementDataType & rVariables,
                                                            const Process_Info& rCurrentProcessInfo)
    {
        Solid_Element::InitializeElementData(rVariables, rCurrentProcessInfo);

        //set variables including all integration points values

        //Calculate Delta Position
        rVariables.DeltaPosition = this->CalculateTotalDeltaPosition(rVariables.DeltaPosition);

        //calculating the reference jacobian from initial cartesian coordinates to parent coordinates for all integration points [dx_n/d£]
        rVariables.J = GetGeometry().Jacobian( rVariables.J, mThisIntegrationMethod, rVariables.DeltaPosition );
    }

    void Small_Displacement_Element::CalculateKinematics(ElementDataType& rVariables,
                                                         double const& rPointNumber)
    {
        //Get the parent coodinates derivative [dN/d£]
        const GeometryType::ShapeFunctionGradientsType& DN_De = rVariables.GetShapeFunctionsGradients();
        //Get the shape functions for the order of the integration method [N]
        const Matrix& Ncontainer = rVariables.GetShapeFunctions();

        //Parent to reference configuration
        rVariables.StressMeasure = Constitutive_Law::StressMeasure_Cauchy;

        //Calculating the inverse of the jacobian and the parameters needed [d£/dx_n]
        Matrix InvJ;
        InvJ = rVariables.J[rPointNumber].inverse();

        //Compute cartesian derivatives  [dN/dx_n]
        rVariables.DN_DX = DN_De[rPointNumber] * InvJ;

        //Displacement Gradient H  [dU/dx_n]
        this->CalculateDisplacementGradient( rVariables.H, rVariables.DN_DX );

        //Set Shape Functions Values for this integration point
        rVariables.N = Ncontainer.row(rPointNumber);

        //Compute the deformation matrix B
        const GeometryType& rGeometry = GetGeometry();
        ElementUtility::CalculateLinearDeformationMatrix(rVariables.B,rGeometry,rVariables.DN_DX);

        //Compute infinitessimal strain
        this->CalculateInfinitesimalStrain( rVariables.H, rVariables.StrainVector );
    }
    void Small_Displacement_Element::CalculateDisplacementGradient(Matrix& rH, const Matrix& rDN_DX)
    {
        const SizeType number_of_nodes = GetGeometry().PointsNumber();
        const SizeType dimension       = GetGeometry().WorkingSpaceDimension();
        rH.setZero(dimension, dimension);

        if( dimension == 2 )
        {
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                const std::array<double, 3 > & Displacement  = GetGeometry()[i].FastGetSolutionStepValue(DISPLACEMENT);
                rH ( 0 , 0 ) += Displacement[0]*rDN_DX ( i , 0 );
                rH ( 0 , 1 ) += Displacement[0]*rDN_DX ( i , 1 );
                rH ( 1 , 0 ) += Displacement[1]*rDN_DX ( i , 0 );
                rH ( 1 , 1 ) += Displacement[1]*rDN_DX ( i , 1 );
            }
        }
        else if( dimension == 3 )
        {
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                const std::array<double, 3 > & Displacement  = GetGeometry()[i].FastGetSolutionStepValue(DISPLACEMENT);
                rH ( 0 , 0 ) += Displacement[0]*rDN_DX ( i , 0 );
                rH ( 0 , 1 ) += Displacement[0]*rDN_DX ( i , 1 );
                rH ( 0 , 2 ) += Displacement[0]*rDN_DX ( i , 2 );
                rH ( 1 , 0 ) += Displacement[1]*rDN_DX ( i , 0 );
                rH ( 1 , 1 ) += Displacement[1]*rDN_DX ( i , 1 );
                rH ( 1 , 2 ) += Displacement[1]*rDN_DX ( i , 2 );
                rH ( 2 , 0 ) += Displacement[2]*rDN_DX ( i , 0 );
                rH ( 2 , 1 ) += Displacement[2]*rDN_DX ( i , 1 );
                rH ( 2 , 2 ) += Displacement[2]*rDN_DX ( i , 2 );
            }
        }
        else
        {
            std::cerr << " something is wrong with the dimension displacement gradient " << std::endl;
            exit(0);
        }
    }

    void Small_Displacement_Element::CalculateInfinitesimalStrain(const Matrix& rH, Vector& rStrainVector )
    {
        const SizeType dimension  = GetGeometry().WorkingSpaceDimension();

        if( dimension == 2 )
        {
            //Infinitesimal Strain Calculation
            if ( rStrainVector.size() != 3 ) rStrainVector.resize( 3, false );

            rStrainVector[0] = rH( 0, 0 );

            rStrainVector[1] = rH( 1, 1 );

            rStrainVector[2] = (rH( 0, 1 ) + rH( 1, 0 )); // xy
        }
        else if( dimension == 3 )
        {
            //Infinitesimal Strain Calculation
            if ( rStrainVector.size() != 6 ) rStrainVector.resize( 6);
            rStrainVector[0] = rH( 0, 0 );
            rStrainVector[1] = rH( 1, 1 );
            rStrainVector[2] = rH( 2, 2 );
            rStrainVector[3] = ( rH( 0, 1 ) + rH( 1, 0 ) ); // xy
            rStrainVector[4] = ( rH( 1, 2 ) + rH( 2, 1 ) ); // yz
            rStrainVector[5] = ( rH( 0, 2 ) + rH( 2, 0 ) ); // xz
        }
        else
        {
            std::cerr << " something is wrong with the dimension infinitesimal strain " << std::endl;
            exit(0);
        }
    }

    int Small_Displacement_Element::Check(Process_Info const& rCurrentProcessInfo) const
    {
        int ErrorCode = 1;
        ErrorCode = Solid_Element::Check(rCurrentProcessInfo);

        for (SizeType i=0;i<this->GetGeometry().size();++i)
        {
            Node const& rNode = this->GetGeometry()[i];
            /// @brief Check Solution Step Data DISPLACEMENT
            if (!rNode.SolutionStepData().Has(DISPLACEMENT))
            {
                std::cerr << "Node " << i << " missing Variable DISPLACEMENT! in SolutionStepData \n";
                exit(0);
            }

            /// @brief Check Dofs
            if (!rNode.HasDofFor(DISPLACEMENT_X))
            {
                std::cerr << "Node " << i << "missing Dof of DISPLACEMENT_X !\n";
                exit(0);
            }
            if (!rNode.HasDofFor(DISPLACEMENT_Y))
            {
                std::cerr << "Node " << i << "missing Dof of DISPLACEMENT_X !\n";
                exit(0);
            }
            if ((rCurrentProcessInfo[SPACE_DIMENSION] == 3) && !rNode.HasDofFor(DISPLACEMENT_Z))
            {
                std::cerr << "Node " << i << "missing Dof of DISPLACEMENT_Z !\n";
                exit(0);
            }
        }

        Constitutive_Law::Features LawFeatures;
        this->GetProperties().GetValue( CONSTITUTIVE_LAW )->GetLawFeatures(LawFeatures);
        bool correct_strain_measure = false;
        for(unsigned int i=0; i<LawFeatures.mStrainMeasures.size(); i++)
        {
            if(LawFeatures.mStrainMeasures[i] == Constitutive_Law::StrainMeasure_Infinitesimal)
            correct_strain_measure = true;
        }
        if( correct_strain_measure == false )
        {
            std::cerr <<  "constitutive law is not compatible with the small displacements element type" << std::endl;
            exit(0);
        }

        // Check that the constitutive law has the correct dimension
        const SizeType dimension  = GetGeometry().WorkingSpaceDimension();
        if( dimension == 2 )
        {
            if( LawFeatures.mOptions.IsNot(Constitutive_Law::PLANE_STRAIN_LAW) && LawFeatures.mOptions.IsNot(Constitutive_Law::PLANE_STRESS_LAW) && LawFeatures.mOptions.IsNot(Constitutive_Law::AXISYMMETRIC_LAW) )
            {
                std::cerr <<  "wrong constitutive law used. This is a 2D element. Expected plane state or axisymmetric :: element id = " << this->Id() << std::endl;
                exit(0);
            }
        }
        return ErrorCode;
    }

/// @}


/// @name Access
/// @{


/// @}


/// @name Inquiry
/// @{


/// @}


/// @brief protected:
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


/// @brief private:
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


