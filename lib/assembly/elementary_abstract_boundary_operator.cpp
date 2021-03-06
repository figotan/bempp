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

#include "elementary_abstract_boundary_operator.hpp"

#include "context.hpp"
#include "discrete_sparse_boundary_operator.hpp"
#include "numerical_quadrature_strategy.hpp"

#include "../fiber/explicit_instantiation.hpp"
#include "../fiber/local_assembler_for_operators.hpp"

namespace Bempp
{

template <typename BasisFunctionType, typename ResultType>
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::
ElementaryAbstractBoundaryOperator(
        const shared_ptr<const Space<BasisFunctionType> >& domain,
        const shared_ptr<const Space<BasisFunctionType> >& range,
        const shared_ptr<const Space<BasisFunctionType> >& dualToRange,
        const std::string& label,
        int symmetry) :
    Base(domain, range, dualToRange, label, symmetry)
{
}

template <typename BasisFunctionType, typename ResultType>
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::
~ElementaryAbstractBoundaryOperator()
{
}

template <typename BasisFunctionType, typename ResultType>
shared_ptr<DiscreteBoundaryOperator<ResultType> >
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::
assembleWeakFormInternal(
        LocalAssembler& assembler,
        const AssemblyOptions& options) const
{
    return assembleWeakFormInternalImpl(assembler, options);
}

template <typename BasisFunctionType, typename ResultType>
shared_ptr<DiscreteBoundaryOperator<ResultType> >
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::
assembleWeakFormInternal(
        LocalAssembler& assembler,
        const Context<BasisFunctionType, ResultType>& context) const
{
    return assembleWeakFormInternalImpl2(assembler, context);
}

template <typename BasisFunctionType, typename ResultType>
shared_ptr<DiscreteBoundaryOperator<ResultType> >
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::
assembleWeakFormInternalImpl(
        LocalAssembler& assembler,
        const AssemblyOptions& options) const
{
    std::cout << "Warning: the variant of assembleWeakFormInternalImpl() with "
                 "an AssemblyOptions object passed as the second argument is "
                 "deprecated and will be removed in a future version of BEM++. "
                 "Use the other variant, accepting a Context object as the "
                 "second argument." << std::endl;
    // possibly use more conservative accuracy options
    NumericalQuadratureStrategy<BasisFunctionType, ResultType> quadStrategy; // fallback
    Context<BasisFunctionType, ResultType> context(
        make_shared_from_ref(quadStrategy), options);
    assembleWeakFormInternalImpl2(assembler, context);
}

template <typename BasisFunctionType, typename ResultType>
std::auto_ptr<typename ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::LocalAssembler>
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::makeAssembler(
        const QuadratureStrategy& quadStrategy,
        const shared_ptr<const GeometryFactory>& testGeometryFactory,
        const shared_ptr<const GeometryFactory>& trialGeometryFactory,
        const shared_ptr<const Fiber::RawGridGeometry<CoordinateType> >& testRawGeometry,
        const shared_ptr<const Fiber::RawGridGeometry<CoordinateType> >& trialRawGeometry,
        const shared_ptr<const std::vector<const Fiber::Basis<BasisFunctionType>*> >& testBases,
        const shared_ptr<const std::vector<const Fiber::Basis<BasisFunctionType>*> >& trialBases,
        const shared_ptr<const Fiber::OpenClHandler>& openClHandler,
        const ParallelizationOptions& parallelizationOptions,
        VerbosityLevel::Level verbosityLevel,
        bool cacheSingularIntegrals) const
{
    return makeAssemblerImpl(quadStrategy,
                             testGeometryFactory, trialGeometryFactory,
                             testRawGeometry, trialRawGeometry,
                             testBases, trialBases, openClHandler,
                             parallelizationOptions,
                             verbosityLevel,
                             cacheSingularIntegrals);
}

template <typename BasisFunctionType, typename ResultType>
std::auto_ptr<typename ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::LocalAssembler>
ElementaryAbstractBoundaryOperator<BasisFunctionType, ResultType>::makeAssembler(
        const QuadratureStrategy& quadStrategy,
        const AssemblyOptions& options) const
{
    typedef Fiber::RawGridGeometry<CoordinateType> RawGridGeometry;
    typedef std::vector<const Fiber::Basis<BasisFunctionType>*> BasisPtrVector;

    const bool verbose = (options.verbosityLevel() >= VerbosityLevel::DEFAULT);

    shared_ptr<RawGridGeometry> testRawGeometry, trialRawGeometry;
    shared_ptr<GeometryFactory> testGeometryFactory, trialGeometryFactory;
    shared_ptr<Fiber::OpenClHandler> openClHandler;
    shared_ptr<BasisPtrVector> testBases, trialBases;
    bool cacheSingularIntegrals;


    if (verbose)
        std::cout << "Collecting data for assembler construction..." << std::endl;
       this->collectDataForAssemblerConstruction(options,
                                        testRawGeometry, trialRawGeometry,
                                        testGeometryFactory, trialGeometryFactory,
                                        testBases, trialBases,
                                        openClHandler, cacheSingularIntegrals);
    if (verbose)
        std::cout << "Data collection finished." << std::endl;

    return makeAssemblerImpl(quadStrategy,
                             testGeometryFactory, trialGeometryFactory,
                             testRawGeometry, trialRawGeometry,
                             testBases, trialBases, openClHandler,
                             options.parallelizationOptions(),
                             options.verbosityLevel(),
                             cacheSingularIntegrals);
}

FIBER_INSTANTIATE_CLASS_TEMPLATED_ON_BASIS_AND_RESULT(ElementaryAbstractBoundaryOperator);

} // namespace Bempp
