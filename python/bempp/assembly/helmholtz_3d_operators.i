%{
#include "assembly/helmholtz_3d_single_layer_boundary_operator.hpp"
#include "assembly/helmholtz_3d_double_layer_boundary_operator.hpp"
#include "assembly/helmholtz_3d_adjoint_double_layer_boundary_operator.hpp"
#include "assembly/helmholtz_3d_hypersingular_boundary_operator.hpp"
%}

%include "assembly/helmholtz_3d_operators_common.hpp"

namespace Bempp
{
%feature("compactdefaultargs")
    helmholtz3dSingleLayerBoundaryOperator;
%feature("compactdefaultargs")
    helmholtz3dDoubleLayerBoundaryOperator;
%feature("compactdefaultargs")
    helmholtz3dAdjointDoubleLayerBoundaryOperator;
%feature("compactdefaultargs")
    helmholtz3dHypersingularBoundaryOperator;
} // namespace Bempp

#define shared_ptr boost::shared_ptr
%include "assembly/helmholtz_3d_single_layer_boundary_operator.hpp"
%include "assembly/helmholtz_3d_double_layer_boundary_operator.hpp"
%include "assembly/helmholtz_3d_adjoint_double_layer_boundary_operator.hpp"
%include "assembly/helmholtz_3d_hypersingular_boundary_operator.hpp"
#undef shared_ptr

namespace Bempp
{
BEMPP_INSTANTIATE_SYMBOL_TEMPLATED_ON_BASIS(
    helmholtz3dSingleLayerBoundaryOperator);
BEMPP_INSTANTIATE_SYMBOL_TEMPLATED_ON_BASIS(
    helmholtz3dDoubleLayerBoundaryOperator);
BEMPP_INSTANTIATE_SYMBOL_TEMPLATED_ON_BASIS(
    helmholtz3dAdjointDoubleLayerBoundaryOperator);
BEMPP_INSTANTIATE_SYMBOL_TEMPLATED_ON_BASIS(
    helmholtz3dHypersingularBoundaryOperator);
} // namespace Bempp
