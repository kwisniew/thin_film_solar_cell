#include "../include/Assembly.hpp"

namespace Assembly
{
	
	using namespace dealii;

	// constructor for the Asssembly Scratch
	template<int dim>	
	AssemblyScratch<dim>::
	AssemblyScratch(const FiniteElement<dim> & Poisson_fe,
			const FiniteElement<dim> & carrier_fe,
			const Quadrature<dim>	& quadrature,
			const Quadrature<dim-1>	& face_quadrature)
	:
	Poisson_fe_values(Poisson_fe, quadrature,
			update_values 		 | 
			update_gradients 	 |
			update_quadrature_points |
			update_JxW_values	 ),
	Poisson_fe_face_values(Poisson_fe, face_quadrature,
				update_values 		 | 
				update_normal_vectors	 |
				update_quadrature_points |
				update_JxW_values	 ),
	carrier_fe_values(carrier_fe, quadrature,
			update_values 			 | 
			update_gradients 		 |
			update_quadrature_points 	 |
			update_JxW_values		 ),
	carrier_fe_face_values(carrier_fe, face_quadrature,
				update_values 		 | 
				update_normal_vectors	 |	
				update_quadrature_points |
				update_JxW_values	 ),
	carrier_fe_subface_values(carrier_fe, face_quadrature,
				update_values 		 | 
				update_normal_vectors 	 |
				update_quadrature_points |
				update_JxW_values	 ),
	carrier_fe_neighbor_face_values(carrier_fe, face_quadrature,
				update_values 		 | 
				update_normal_vectors	 |
				update_quadrature_points |
				update_JxW_values	 ),
	Poisson_rhs_values(quadrature.size()),
	Poisson_bc_values(face_quadrature.size()),
	Poisson_bi_values(face_quadrature.size()),
	Poisson_app_values(face_quadrature.size()),
	electric_field_values(quadrature.size()),
	old_carrier_1_density_values(quadrature.size()),
	old_carrier_2_density_values(quadrature.size()),
	carrier_1_current_values(quadrature.size()),
	carrier_2_current_values(quadrature.size()),
	donor_doping_values(quadrature.size()),
	acceptor_doping_values(quadrature.size()),
	generation_values(quadrature.size()),
	carrier_1_bc_values(face_quadrature.size()),
	carrier_2_bc_values(face_quadrature.size()),
	electron_interface_values(face_quadrature.size()),
	hole_interface_values(face_quadrature.size()),
	reductant_interface_values(face_quadrature.size()),
	oxidant_interface_values(face_quadrature.size()),
	psi_i_potential(Poisson_fe.dofs_per_cell),
	psi_i_field(Poisson_fe.dofs_per_cell),
	psi_i_density(carrier_fe.dofs_per_cell),
	psi_i_current(carrier_fe.dofs_per_cell),
	normals(face_quadrature.size())
	{}	

	
	// copy constructor for the Asssembly Scratch
	template<int dim>
	AssemblyScratch<dim>::
	AssemblyScratch(const AssemblyScratch & scratch)
	:
	Poisson_fe_values(scratch.Poisson_fe_values.get_fe(),
			scratch.Poisson_fe_values.get_quadrature(),
			scratch.Poisson_fe_values.get_update_flags() ),
	Poisson_fe_face_values(scratch.Poisson_fe_face_values.get_fe(),
			   scratch.Poisson_fe_face_values.get_quadrature(),
			   scratch.Poisson_fe_face_values.get_update_flags() ),
	carrier_fe_values(scratch.carrier_fe_values.get_fe(),
			  scratch.carrier_fe_values.get_quadrature(),
			  scratch.carrier_fe_values.get_update_flags() ),
	carrier_fe_face_values(scratch.carrier_fe_face_values.get_fe(),
			   scratch.carrier_fe_face_values.get_quadrature(),
			   scratch.carrier_fe_face_values.get_update_flags() ),
	carrier_fe_subface_values(scratch.carrier_fe_subface_values.get_fe(),
			  scratch.carrier_fe_subface_values.get_quadrature(),
			  scratch.carrier_fe_subface_values.get_update_flags() ),
	carrier_fe_neighbor_face_values(scratch.carrier_fe_face_values.get_fe(),
				scratch.carrier_fe_face_values.get_quadrature(),
				scratch.carrier_fe_face_values.get_update_flags() ),
	Poisson_rhs_values(scratch.Poisson_rhs_values),
	Poisson_bc_values(scratch.Poisson_bc_values),
	Poisson_bi_values(scratch.Poisson_bi_values),
	electric_field_values(scratch.electric_field_values),
	old_carrier_1_density_values(scratch.old_carrier_1_density_values),
	old_carrier_2_density_values(scratch.old_carrier_2_density_values),
	carrier_1_current_values(scratch.carrier_1_current_values),
	carrier_2_current_values(scratch.carrier_2_current_values),
	donor_doping_values(scratch.donor_doping_values),
	acceptor_doping_values(scratch.acceptor_doping_values),
	generation_values(scratch.generation_values),
	carrier_1_bc_values(scratch.carrier_1_bc_values),
	carrier_2_bc_values(scratch.carrier_2_bc_values),
	electron_interface_values(scratch.electron_interface_values),
	hole_interface_values(scratch.hole_interface_values),
	reductant_interface_values(scratch.reductant_interface_values),
	oxidant_interface_values(scratch.oxidant_interface_values),
	psi_i_potential(scratch.psi_i_potential),
	psi_i_field(scratch.psi_i_field),
	psi_i_density(scratch.psi_i_density),
	psi_i_current(scratch.psi_i_current),
	normals(scratch.normals)
	{}	





	/////////////////////////////////////////////////////////////////////////////
	// COPY DATA
	////////////////////////////////////////////////////////////////////////////
	
	// Poisson copy data
	namespace Poisson
	{
		// constructor
		template<int dim>
		CopyData<dim>::
		CopyData(const FiniteElement<dim> & Poisson_fe)
		:
		local_rhs(Poisson_fe.dofs_per_cell),
		local_matrix(Poisson_fe.dofs_per_cell,	
			     Poisson_fe.dofs_per_cell),
		local_dof_indices(Poisson_fe.dofs_per_cell)
		{ }

		// copy constructor
		template<int dim>
		CopyData<dim>::
		CopyData(const CopyData & data)
		:
		local_rhs(data.local_rhs),
		local_matrix(data.local_matrix),
		local_dof_indices(data.local_dof_indices)	
		{ }



	} // end namespace Poisson

	
	// semiconductor copydata	
	namespace DriftDiffusion
	{
			// constructor
		template<int dim>
		CopyData<dim>::
		CopyData(const FiniteElement<dim> & carrier_fe)
		:
		local_carrier_1_rhs(carrier_fe.dofs_per_cell),
		local_carrier_2_rhs(carrier_fe.dofs_per_cell),
		local_matrix_1(carrier_fe.dofs_per_cell,
				carrier_fe.dofs_per_cell),
		local_matrix_2(carrier_fe.dofs_per_cell,
				carrier_fe.dofs_per_cell),
		local_mass_matrix(carrier_fe.dofs_per_cell,
				  carrier_fe.dofs_per_cell),
		vi_ui_matrix(carrier_fe.dofs_per_cell,
			     carrier_fe.dofs_per_cell),
		vi_ue_matrix(carrier_fe.dofs_per_cell,
			     carrier_fe.dofs_per_cell),
		ve_ui_matrix(carrier_fe.dofs_per_cell,
			     carrier_fe.dofs_per_cell),
		ve_ue_matrix(carrier_fe.dofs_per_cell,
			     carrier_fe.dofs_per_cell),	
		local_dof_indices(carrier_fe.dofs_per_cell),
		local_neighbor_dof_indices(carrier_fe.dofs_per_cell)
		{ }
	

		// copy constructor
		template<int dim>
		CopyData<dim>::
		CopyData(const CopyData & data)
		:
		local_carrier_1_rhs(data.local_carrier_1_rhs),
		local_carrier_2_rhs(data.local_carrier_2_rhs),
		local_matrix_1(data.local_matrix_1),
		local_matrix_2(data.local_matrix_2),
		local_mass_matrix(data.local_mass_matrix),
		vi_ui_matrix(data.vi_ui_matrix),
		vi_ue_matrix(data.vi_ue_matrix),
		ve_ui_matrix(data.ve_ui_matrix),
		ve_ue_matrix(data.ve_ue_matrix),
		local_dof_indices(data.local_dof_indices),
		local_neighbor_dof_indices(data.local_dof_indices)	
		{ }

	


	} // end semiconductor


} // end namespace Assembly
