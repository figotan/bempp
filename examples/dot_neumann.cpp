// Copyright (C) 2011 by the BEM++ Authors
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

#include "config_alugrid.hpp"
#include "config_trilinos.hpp"

#include "meshes.hpp"

#include "assembly/assembly_options.hpp"
#include "assembly/discrete_linear_operator.hpp"
#include "assembly/evaluation_options.hpp"
#include "assembly/grid_function.hpp"
#include "assembly/interpolated_function.hpp"
#include "assembly/linear_operator_superposition.hpp"
#include "assembly/ordinary_function.hpp"
#include "assembly/standard_local_assembler_factory_for_operators_on_surfaces.hpp"

#include "assembly/identity_operator.hpp"
#include "assembly/dot_single_layer_potential_3d.hpp"
#include "assembly/dot_double_layer_potential_3d.hpp"
#include "assembly/adjoint_double_layer_potential_3d.hpp"
#include "assembly/hypersingular_operator_3d.hpp"

#include "common/scalar_traits.hpp"

#include "grid/geometry.hpp"
#include "grid/geometry_factory.hpp"
#include "grid/grid_factory.hpp"

#include "linalg/aca_preconditioner_factory.hpp"
#include "linalg/default_iterative_solver.hpp"
#include "linalg/default_direct_solver.hpp"

#include "space/piecewise_linear_continuous_scalar_space.hpp"
#include "space/piecewise_constant_scalar_space.hpp"

#include <armadillo>
#include <cmath>
#include <iostream>
#include <memory>

using namespace Bempp;

typedef double BFT; // basis function type
typedef std::complex<double> RT; // result type (type used to represent discrete operators)

class MyFunctor
{
public:
    // Type of the function's values (e.g. float or std::complex<double>)
    typedef RT ValueType;
    // Type of coordinates (must be the "real part" of ValueType)
    typedef ScalarTraits<RT>::RealType CoordinateType;

    // Number of components of the function's argument
    static const int argumentDimension = 3;

    // Number of components of the function's result
    static const int resultDimension = 1;

    // Evaluate the function at the point "point" and store result in
    // the array "result"
    inline void evaluate(const arma::Col<CoordinateType>& point,
                         arma::Col<ValueType>& result) const {
        result(0) = 1.;
    }
};


int main(int argc, char* argv[])
{
    // Physical parameters
    BFT mua = 0.01; // absorption coefficient
    BFT mus = 1.0;  // diffusion coefficient
    BFT refind = 1.0; // refractive index
    BFT freq = 100*1e6; // modulation frequency [Hz]

    // Derived parameters
    const BFT c0 = 0.3;      // speed of light in vacuum [mm/ps]
    BFT c = c0/refind;       // speed of light in medium [mm/ps]
    BFT kappa = 1.0/(3.0*(mua+mus));   // diffusion coefficient
    BFT omega = 2.0*M_PI * freq*1e-12; // modulation frequency [cycles/ps]

    // Construct wave number
    RT waveNo = sqrt (RT(mua/kappa, omega/(c*kappa)));

    // Load mesh

    if (argc != 2) {
        std::cout << "Solve a Neumann problem for the Laplace equation.\n"
                     "Usage: " << argv[0] << " <mesh_file>" << std::endl;
        return 1;
    }
    std::auto_ptr<Grid> grid = loadTriangularMeshFromFile(argv[1]);

    // Initialize the spaces

    PiecewiseLinearContinuousScalarSpace<BFT> HplusHalfSpace(*grid);
    PiecewiseConstantScalarSpace<BFT> HminusHalfSpace(*grid);

    HplusHalfSpace.assignDofs();
    HminusHalfSpace.assignDofs();

    // Define some default options.

    AssemblyOptions assemblyOptions;

    // We want to use ACA

    AcaOptions acaOptions; // Default parameters for ACA
    //assemblyOptions.switchToAca(acaOptions);
    assemblyOptions.switchToDense();

    // Define the standard integration factory

    StandardLocalAssemblerFactoryForOperatorsOnSurfaces<BFT, RT> factory;

    // We need the single layer, double layer, and the identity operator

    DotSingleLayerPotential3D<BFT, RT> rhsOp(HplusHalfSpace, HminusHalfSpace, waveNo);
    DotDoubleLayerPotential3D<BFT, RT> dlp(HplusHalfSpace, HplusHalfSpace, waveNo);
    IdentityOperator<BFT, RT> id(HplusHalfSpace, HplusHalfSpace);

    // Form the left-hand side sum

    LinearOperatorSuperposition<BFT, RT> lhsOp = -0.5 * id + dlp;

    // Assemble the Operators

    rhsOp.assemble(factory, assemblyOptions);
    lhsOp.assemble(factory, assemblyOptions);

    // We also want a grid function

    MyFunctor functor;
    OrdinaryFunction<MyFunctor> function(functor);

    GridFunction<BFT, RT> u(HminusHalfSpace, function, factory, assemblyOptions);

    // Assemble the rhs

    std::cout << "Assemble rhs" << std::endl;

    GridFunction<BFT, RT> rhs = rhsOp * u;

    // Initialize the solver

    std::cout << "Initialize solver" << std::endl;

#ifdef WITH_TRILINOS
    DefaultIterativeSolver<BFT, RT> solver(lhsOp, rhs);
    solver.initializeSolver(defaultGmresParameterList(1e-5));
    solver.solve();
    std::cout << solver.getSolverMessage() << std::endl;
#else
    DefaultDirectSolver<BFT, RT> solver(lhsOp, rhs);
    solver.solve();
#endif

    // Extract the solution

    GridFunction<BFT, RT> solFun = solver.getResult();

    // Write out as VTK

    solFun.exportToVtk(VtkWriter::VERTEX_DATA, "Dirichlet_data",
                       "calculated_dirichlet_data");
}