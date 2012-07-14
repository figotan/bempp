# Copyright (C) 2011-2012 by the BEM++ Authors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


import bempp.core as core

def createGridFactory():
    """Return a GridFactory object"""
    return core.GridFactory

def createNumericalQuadratureStrategy(basisFunctionType, resultType, accuracyOptions=None):
    """Construct a NumericalQuadratureStrategy object.

    *Arguments:*
        basisFunctionType (string)
            Type used to represent values of basis functions.

        resultType (string)
            Type used to represent values of boundary-element integrals.

        accuracyOptions (AccuracyOptions)
            Determines quadrature order. If set to None, default quadrature orders
            are used.

        The following combinations of basisFunctionType and resultType are allowed:

            basisFunctionType     resultType
            --------------------------------
            "float32"             "float32" or "complex64"
            "float64"             "float64" or "complex128"
            "complex64"           "complex64"
            "complex128"          "complex128"
    """
    basisFunctionType = core.checkType(basisFunctionType)
    resultType = core.checkType(resultType)
    if accuracyOptions is None:
        accuracyOptions = core.AccuracyOptions()
    name = 'NumericalQuadratureStrategy'
    return core.constructObjectTemplatedOnBasisAndResult(
        name, basisFunctionType, resultType, accuracyOptions)


def createAssemblyOptions(*args,**kwargs):
    """Return an AssemblyOptions object"""
    return core.AssemblyOptions(*args,**kwargs)

def createContext(factory, assemblyOptions):
    """Return an operator assembly context"""
    name = 'Context'
    return core.constructObjectTemplatedOnBasisAndResult(
        name, factory.basisFunctionType(), factory.resultType(),
        factory, assemblyOptions)

def createPiecewiseConstantScalarSpace(context, grid):
    """Return a space of piecewise constant scalar functions"""
    name = 'PiecewiseConstantScalarSpace'
    return core.constructObjectTemplatedOnBasis(name, context.basisFunctionType(), grid)

def createPiecewiseLinearContinuousScalarSpace(context, grid):
    """Return space of piecewise linear continuous scalar functions"""
    name = 'PiecewiseLinearContinuousScalarSpace'
    return core.constructObjectTemplatedOnBasis(name, context.basisFunctionType(), grid)

def createLaplace3dSingleLayerBoundaryOperator(context, domain, range, dualToRange):
    """Return a single-layer-potential operator for the Laplace equation in 3D."""
    return core._constructOperator(
    "laplace3dSingleLayerBoundaryOperator", context, domain, range, dualToRange)

def createLaplace3dDoubleLayerBoundaryOperator(context, domain, range, dualToRange):
    """Return a double-layer-potential operator for the Laplace equation in 3D."""
    return core._constructOperator(
    "laplace3dDoubleLayerBoundaryOperator", context, domain, range, dualToRange)

def createLaplace3dAdjointDoubleLayerBoundaryOperator(context, domain, range, dualToRange):
    """Return an adjoint double-layer-potential operator for the Laplace equation in 3D."""
    return core._constructOperator(
    "laplace3dAdjointDoubleLayerBoundaryOperator", context, domain, range, dualToRange)

def createLaplace3dHypersingularBoundaryOperator(context, domain, range, dualToRange):
    """Return a hypersingular operator for the Laplace equation in 3D."""
    return core._constructOperator(
    "laplace3dHypersingularBoundaryOperator", context, domain, range, dualToRange)

def createHelmholtz3dSingleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return a single-layer-potential operator for the Helmholtz equation in 3D."""
    return core._constructHelmholtzOperator(
        "helmholtz3dSingleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createHelmholtz3dDoubleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return a double-layer-potential operator for the Helmholtz equation in 3D."""
    return core._constructHelmholtzOperator(
        "helmholtz3dDoubleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createHelmholtz3dAdjointDoubleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return an adjoint double-layer-potential operator for the Helmholtz equation in 3D."""
    return core._constructHelmholtzOperator(
        "helmholtz3dAdjointDoubleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createHelmholtz3dHypersingularBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return a hypersingular operator for the Helmholtz equation in 3D."""
    return core._constructHelmholtzOperator(
        "helmholtz3dHypersingularBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createModifiedHelmholtz3dSingleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return a single-layer-potential operator for the modified Helmholtz equation in 3D."""
    return core._constructModifiedHelmholtzOperator(
        "modifiedHelmholtz3dSingleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createModifiedHelmholtz3dDoubleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return a double-layer-potential operator for the modified Helmholtz equation in 3D."""
    return core._constructModifiedHelmholtzOperator(
        "modifiedHelmholtz3dDoubleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createModifiedHelmholtz3dAdjointDoubleLayerBoundaryOperator(
        context, domain, range, dualToRange, waveNumber):
    """Return an adjoint double-layer-potential operator for the modified Helmholtz equation in 3D."""
    return core._constructModifiedHelmholtzOperator(
        "modifiedHelmholtz3dAdjointDoubleLayerBoundaryOperator", context,
        domain, range, dualToRange, waveNumber)

def createModifiedHelmholtz3dHypersingularBoundaryOperator(
         domain, range, dualToRange, waveNumber, resultType=None):
     """Return a hypersingular operator for the modified Helmholtz equation in 3D."""
     return core._constructModifiedHelmholtzOperator(
         "modifiedHelmholtz3dHypersingularBoundaryOperator", context, domain, range, dualToRange,
         waveNumber, resultType)


def createIdentityOperator(context, domain, range, dualToRange):
    """Return an identity operator."""
    return core._constructOperator(
        "identityOperator", context, domain, range, dualToRange)

def __gridFunctionFromFunctor(
        functorType,
        context, space, dualSpace, function,
        argumentDimension, resultDimension):
    basisFunctionType = core.checkType(context.basisFunctionType())
    resultType = core.checkType(context.resultType())
    if (basisFunctionType != space.basisFunctionType() or
            basisFunctionType != dualSpace.basisFunctionType()):
        raise TypeError("BasisFunctionType of context, space and dualSpace must be the same")

    functor = core.constructObjectTemplatedOnValue(
        "Python" + functorType,
        resultType, function, argumentDimension, resultDimension)
    result = core.constructObjectTemplatedOnBasisAndResult(
        "gridFunctionFromPython" + functorType,
        basisFunctionType, resultType,
        context, space, dualSpace, functor)
    result._context = context
    result._space = space
    result._dualSpace = dualSpace
    return result

def gridFunctionFromSurfaceNormalDependentFunction(
        context, space, dualSpace, function,
        argumentDimension=3, resultDimension=1):
    return __gridFunctionFromFunctor(
        "SurfaceNormalDependentFunctor",
        context, space, dualSpace, function, 
        argumentDimension, resultDimension)

def gridFunctionFromSurfaceNormalIndependentFunction(
        context, space, dualSpace, function,
        argumentDimension=3, resultDimension=1):
    return __gridFunctionFromFunctor(
        "SurfaceNormalIndependentFunctor",
        context, space, dualSpace, function,
        argumentDimension, resultDimension)

def createDefaultIterativeSolver(boundaryOperator, gridFunction):
    """Return the default iterative linear solver.

    This solver lets you solve the equation A f = g for the function
    f, with A being the boundary operator passed via the boundaryOperator
    argument and g the function represented by the gridFunction
    argument.
    """
    basisFunctionType = boundaryOperator.basisFunctionType()
    if (basisFunctionType != gridFunction.basisFunctionType()):
        raise TypeError("BasisFunctionType of boundaryOperator and "
                        "gridFunction must be the same")
    resultType = boundaryOperator.resultType()
    if (resultType != gridFunction.resultType()):
        raise TypeError("ResultType of boundaryOperator and "
                        "gridFunction must be the same")
    result = core.constructObjectTemplatedOnBasisAndResult(
        "DefaultIterativeSolver", basisFunctionType, resultType,
        boundaryOperator, gridFunction)
    result._space = boundaryOperator.domain()
    return result

from core import defaultGmresParameterList
from core import defaultCgParameterList



