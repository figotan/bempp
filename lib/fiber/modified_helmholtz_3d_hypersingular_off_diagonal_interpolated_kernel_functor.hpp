// Copyright (C) 2011-2012 by the BEM++ Authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef fiber_modified_helmholtz_3d_hypersingular_off_diagonal_interpolated_kernel_functor_hpp
#define fiber_modified_helmholtz_3d_hypersingular_off_diagonal_interpolated_kernel_functor_hpp

#include "../common/common.hpp"

#include "geometrical_data.hpp"
#include "hermite_interpolator.hpp"
#include "initialize_interpolator_for_modified_helmholtz_3d_kernels.hpp"
#include "scalar_traits.hpp"

#include "../common/complex_aux.hpp"

namespace Fiber
{

/** \ingroup modified_helmholtz_3d
 *  \ingroup functors
 *  \brief Kernel functor for the hypersingular operator associated with the
 *  modified Helmholtz equation in 3D, applicable for test and trial points
 *  lying on nonadjacent elements.
 *
 *  Uses interpolation to speed up kernel evaluation.
 *
 *  \tparam ValueType Type used to represent the values of the kernel. It can
 *  be one of: \c float, \c double, <tt>std::complex<float></tt> and
 *  <tt>std::complex<double></tt>. Note that setting \p ValueType to a real
 *  type implies that the wave number will also be purely real.
 *
 *  \see modified_helmholtz_3d
 */

template <typename ValueType_>
class ModifiedHelmholtz3dHypersingularOffDiagonalInterpolatedKernelFunctor
{
public:
    typedef ValueType_ ValueType;
    typedef typename ScalarTraits<ValueType>::RealType CoordinateType;

    explicit ModifiedHelmholtz3dHypersingularOffDiagonalInterpolatedKernelFunctor(
            ValueType waveNumber,
            CoordinateType maxDist, int interpPtsPerWavelength) :
        m_waveNumber(waveNumber)
    {
        initializeInterpolatorForModifiedHelmholtz3dKernels(
                    waveNumber, maxDist, interpPtsPerWavelength, m_interpolator);
    }

    int kernelCount() const { return 1; }
    int kernelRowCount(int /* kernelIndex */) const { return 1; }
    int kernelColCount(int /* kernelIndex */) const { return 1; }

    void addGeometricalDependencies(size_t& testGeomDeps, size_t& trialGeomDeps) const {
        testGeomDeps |= GLOBALS | NORMALS;
        trialGeomDeps |= GLOBALS | NORMALS;
    }

    ValueType waveNumber() const { return m_waveNumber; }

    template <template <typename T> class CollectionOf2dSlicesOfNdArrays>
    void evaluate(
            const ConstGeometricalDataSlice<CoordinateType>& testGeomData,
            const ConstGeometricalDataSlice<CoordinateType>& trialGeomData,
            CollectionOf2dSlicesOfNdArrays<ValueType>& result) const {
        const int coordCount = 3;
        const ValueType waveNumber = m_waveNumber;

        CoordinateType distanceSq = 0.;
        CoordinateType nTest_diff = 0;
        CoordinateType nTrial_diff = 0;
        CoordinateType nTest_nTrial = 0.;
        for (int coordIndex = 0; coordIndex < coordCount; ++coordIndex) {
            CoordinateType diff = trialGeomData.global(coordIndex) -
                    testGeomData.global(coordIndex);
            distanceSq += diff * diff;
            nTest_diff += diff * testGeomData.normal(coordIndex);
            nTrial_diff += diff * trialGeomData.normal(coordIndex);
            nTest_nTrial += testGeomData.normal(coordIndex) *
                    trialGeomData.normal(coordIndex);
        }
        CoordinateType distance = sqrt(distanceSq);
        ValueType kr = waveNumber * distance;
        ValueType v = m_interpolator.evaluate(distance);
        const CoordinateType ONE = 1., THREE = 3.;
        result[0](0, 0) =
                static_cast<CoordinateType>(1.0 / (4.0 * M_PI)) /
                (distance * distanceSq * distanceSq) *
                (-distanceSq * nTest_nTrial * (ONE + kr) +
                 nTest_diff * nTrial_diff *
                 (THREE + THREE * kr + kr * kr)) * v;
    }

    CoordinateType estimateRelativeScale(CoordinateType distance) const {
        return exp(-realPart(m_waveNumber) * distance);
    }

private:
    /** \cond PRIVATE */
    ValueType m_waveNumber;
    HermiteInterpolator<ValueType> m_interpolator;
    /** \endcond */
};

} // namespace Fiber

#endif
