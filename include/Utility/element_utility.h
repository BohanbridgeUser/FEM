#ifndef _ELEMENT_UTILITY_H_
#define _ELEMENT_UTILITY_H_

#include "../Node/node.h"
#include "../Geometry/geometry.h"


class ElementUtility
{
    /**
     * @brief Calculate Total Delta Position
     * @param rDeltaPosition, matrix storing the displacement or position increment from origin, returned parameter
     * @param rGeometry, geometry where the gradient is calculated
     */
    typedef Geometry<Node> 
                                                                GeometryType;
    typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>
                                                                      Matrix;
    typedef size_t
                                                                    SizeType;
    public:
        static inline void CalculateTotalDeltaPosition(Matrix & rDeltaPosition, const GeometryType& rGeometry)
        {
            const SizeType number_of_nodes = rGeometry.PointsNumber();
            const SizeType dimension  = rGeometry.WorkingSpaceDimension();

            if(rDeltaPosition.rows() != number_of_nodes || rDeltaPosition.cols() !=  dimension)
                rDeltaPosition.resize(number_of_nodes,dimension);

            //noalias(rDeltaPosition) = ZeroMatrix(number_of_nodes,dimension);

            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                const std::array<double, 3 > & CurrentDisplacement  = rGeometry[i].FastGetSolutionStepValue(DISPLACEMENT);

                for ( SizeType j = 0; j < dimension; j++ )
                {
                    rDeltaPosition(i,j) = CurrentDisplacement[j];
                }
            }
        }
        /**
     * @brief Calculate Linear deformation matrix BL
     * @param rDeformationMatrix, matrix form, returned parameter
     * @param rGeometry, geometry where the gradient is calculated
     * @param rDN_DX, shape functions derivatives
     */
    static inline void CalculateLinearDeformationMatrix(Matrix& rDeformationMatrix, const GeometryType& rGeometry, const Matrix& rDN_DX)
    {

        const SizeType number_of_nodes  = rGeometry.PointsNumber();
        const SizeType dimension        = rGeometry.WorkingSpaceDimension();
        unsigned int voigt_size         = dimension * (dimension +1) * 0.5;

        if ( rDeformationMatrix.rows() != voigt_size || rDeformationMatrix.cols() != dimension*number_of_nodes )
            rDeformationMatrix.resize(voigt_size, dimension*number_of_nodes);


        if( dimension == 2 )
        {
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                unsigned int index = 2 * i;
                rDeformationMatrix( 0, index + 0 ) = rDN_DX( i, 0 );
                rDeformationMatrix( 0, index + 1 ) = 0.0;
                rDeformationMatrix( 1, index + 0 ) = 0.0;
                rDeformationMatrix( 1, index + 1 ) = rDN_DX( i, 1 );
                rDeformationMatrix( 2, index + 0 ) = rDN_DX( i, 1 );
                rDeformationMatrix( 2, index + 1 ) = rDN_DX( i, 0 );
            }
        }
        else if( dimension == 3 )
        {
            for ( SizeType i = 0; i < number_of_nodes; i++ )
            {
                unsigned int index = 3 * i;
                rDeformationMatrix( 0, index + 0 ) = rDN_DX( i, 0 );
                rDeformationMatrix( 0, index + 1 ) = 0.0;
                rDeformationMatrix( 0, index + 2 ) = 0.0;

                rDeformationMatrix( 1, index + 0 ) = 0.0;
                rDeformationMatrix( 1, index + 1 ) = rDN_DX( i, 1 );
                rDeformationMatrix( 1, index + 2 ) = 0.0;

                rDeformationMatrix( 2, index + 0 ) = 0.0;
                rDeformationMatrix( 2, index + 1 ) = 0.0;
                rDeformationMatrix( 2, index + 2 ) = rDN_DX( i, 2 );

                rDeformationMatrix( 3, index + 0 ) = rDN_DX( i, 1 );
                rDeformationMatrix( 3, index + 1 ) = rDN_DX( i, 0 );
                rDeformationMatrix( 3, index + 2 ) = 0.0;

                rDeformationMatrix( 4, index + 0 ) = 0.0;
                rDeformationMatrix( 4, index + 1 ) = rDN_DX( i, 2 );
                rDeformationMatrix( 4, index + 2 ) = rDN_DX( i, 1 );

                rDeformationMatrix( 5, index + 0 ) = rDN_DX( i, 2 );
                rDeformationMatrix( 5, index + 1 ) = 0.0;
                rDeformationMatrix( 5, index + 2 ) = rDN_DX( i, 0 );
            }
        }
        else
        {
            std::cerr << " something is wrong with the dimension when computing linear DeformationMatrix " << std::endl;
            exit(0);
        }
    }
};

#endif