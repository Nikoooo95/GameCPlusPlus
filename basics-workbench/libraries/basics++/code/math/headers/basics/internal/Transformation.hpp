/*
 *  TRANSFORMATION
 *  Copyright © 2014+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801080018
 */

#ifndef BASICS_TRANSFORMATION_HEADER
#define BASICS_TRANSFORMATION_HEADER

    #include "Matrix.hpp"

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        class Transformation
        {
        public:

            typedef NUMERIC_TYPE Numeric_Type;
            typedef Numeric_Type Number;

            static  constexpr unsigned dimension = DIMENSION;
            static  constexpr unsigned size      = dimension + 1;

            typedef Matrix< size, size, Numeric_Type > Matrix;

        protected:

            Matrix matrix;

        public:

            Transformation()
            :
                matrix(Matrix::identity)
            {
            }

            Transformation(const Matrix & matrix)
            :
                matrix(matrix)
            {
            }

        public:

            Transformation operator * (const Transformation & other) const
            {
                return this->matrix *  other.matrix;
            }

            operator const Matrix & () const
            {
                return matrix;
            }

        };

        typedef Transformation< 2, int    > Transformation2i;
        typedef Transformation< 2, float  > Transformation2f;
        typedef Transformation< 2, double > Transformation2d;

        typedef Transformation< 3, int    > Transformation3i;
        typedef Transformation< 3, float  > Transformation3f;
        typedef Transformation< 3, double > Transformation3d;

    }

#endif
