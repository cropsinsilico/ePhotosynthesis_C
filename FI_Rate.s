	.file	"FI_Rate.cpp"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.ascii	"Reference boost::detail::multi_array::value_accessor_one<T>:"
	.ascii	":access(boost::type<Reference>, boost::detail::multi_array::"
	.ascii	"value_accessor_one<T>::index, TP"
	.string	"tr, const size_type*, const index*, const index*) const [with Reference = double&; TPtr = double*; T = double; boost::detail::multi_array::value_accessor_one<T>::index = long int; boost::detail::multi_array::multi_array_base::size_type = long unsigned int]"
	.align 8
.LC1:
	.string	"/usr/include/boost/multi_array/base.hpp"
	.align 8
.LC2:
	.string	"size_type(idx - index_bases[0]) < extents[0]"
	.text
	.align 2
	.p2align 4
	.type	_ZNK5boost6detail11multi_array18value_accessor_oneIdE6accessIRdPdEET_NS_4typeIS7_EElT0_PKmPKlSE_.isra.0.part.0, @function
_ZNK5boost6detail11multi_array18value_accessor_oneIdE6accessIRdPdEET_NS_4typeIS7_EElT0_PKmPKlSE_.isra.0.part.0:
.LFB6441:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$.LC0, %ecx
	movl	$178, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
	.cfi_endproc
.LFE6441:
	.size	_ZNK5boost6detail11multi_array18value_accessor_oneIdE6accessIRdPdEET_NS_4typeIS7_EElT0_PKmPKlSE_.isra.0.part.0, .-_ZNK5boost6detail11multi_array18value_accessor_oneIdE6accessIRdPdEET_NS_4typeIS7_EElT0_PKmPKlSE_.isra.0.part.0
	.section	.rodata._ZN5boost15multi_array_refIdLm2EEixEl.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.ascii	"Reference boost::detail::multi_array::value_accessor_n<T, Nu"
	.ascii	"mDims>::access(boost::type<Reference>, boost::detail::multi_"
	.ascii	"array::value_accessor_n<T, NumDims>::index, TPtr, const size"
	.ascii	"_type*, const index*, const index*) const [with Reference = "
	.ascii	"boost"
	.string	"::detail::multi_array::sub_array<double, 1>; TPtr = double*; T = double; long unsigned int NumDims = 2; boost::detail::multi_array::value_accessor_n<T, NumDims>::index = long int; boost::detail::multi_array::multi_array_base::size_type = long unsigned int]"
	.section	.rodata._ZN5boost15multi_array_refIdLm2EEixEl.str1.1,"aMS",@progbits,1
.LC4:
	.string	"idx - index_bases[0] >= 0"
	.section	.text._ZN5boost15multi_array_refIdLm2EEixEl,"axG",@progbits,_ZN5boost15multi_array_refIdLm2EEixEl,comdat
	.align 2
	.p2align 4
	.weak	_ZN5boost15multi_array_refIdLm2EEixEl
	.type	_ZN5boost15multi_array_refIdLm2EEixEl, @function
_ZN5boost15multi_array_refIdLm2EEixEl:
.LFB5398:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	%rdi, %rax
	movq	(%rsi), %rcx
	movq	80(%rsi), %rdi
	movq	%rdx, %r8
	subq	64(%rsi), %r8
	js	.L8
	cmpq	32(%rsi), %r8
	jnb	.L9
	imulq	48(%rsi), %rdx
	addq	%rdi, %rdx
	leaq	(%rcx,%rdx,8), %rdx
	movq	%rdx, (%rax)
	leaq	40(%rsi), %rdx
	movq	%rdx, 8(%rax)
	leaq	56(%rsi), %rdx
	addq	$72, %rsi
	movq	%rdx, 16(%rax)
	movq	%rsi, 24(%rax)
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	movl	$.LC3, %ecx
	movl	$135, %edx
	movl	$.LC1, %esi
	movl	$.LC4, %edi
	call	__assert_fail
.L9:
	movl	$.LC3, %ecx
	movl	$136, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
	.cfi_endproc
.LFE5398:
	.size	_ZN5boost15multi_array_refIdLm2EEixEl, .-_ZN5boost15multi_array_refIdLm2EEixEl
	.section	.rodata._ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.ascii	"Reference boost::detail::multi_array::value_accessor_n<T, Nu"
	.ascii	"mDims>::access(boost::type<Reference>, boost::detail::multi_"
	.ascii	"array::value_accessor_n<T, NumDims>::index, TPtr, const size"
	.ascii	"_type*, const index*, const index*) const [with Reference = "
	.ascii	"boost::detail::multi_array::cons"
	.string	"t_sub_array<double, 1, const double*>; TPtr = const double*; T = double; long unsigned int NumDims = 2; boost::detail::multi_array::value_accessor_n<T, NumDims>::index = long int; boost::detail::multi_array::multi_array_base::size_type = long unsigned int]"
	.align 8
.LC6:
	.ascii	"boost::detail::mu"
	.string	"lti_array::sub_array<T, NumDims>& boost::detail::multi_array::sub_array<T, NumDims>::operator=(const ConstMultiArray&) [with ConstMultiArray = boost::detail::multi_array::const_sub_array<double, 1, const double*>; T = double; long unsigned int NumDims = 1]"
	.align 8
.LC7:
	.string	"/usr/include/boost/multi_array/subarray.hpp"
	.align 8
.LC8:
	.string	"std::equal(other.shape(),other.shape()+this->num_dimensions(), this->shape())"
	.align 8
.LC9:
	.ascii	"Reference boost::detail::multi_array::value_accessor_one<T>:"
	.ascii	":access(boost::type<Reference>, boost::detail::multi_array::"
	.ascii	"value_accessor_one<T>::index, TPtr, const si"
	.string	"ze_type*, const index*, const index*) const [with Reference = const double&; TPtr = const double*; T = double; boost::detail::multi_array::value_accessor_one<T>::index = long int; boost::detail::multi_array::multi_array_base::size_type = long unsigned int]"
	.section	.text._ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_,"axG",@progbits,_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_,comdat
	.p2align 4
	.weak	_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_
	.type	_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_, @function
_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_:
.LFB6001:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	(%rdx), %rbx
	movq	32(%rsi), %r12
	movq	16(%rsi), %rbp
	movq	24(%rsi), %r15
	movq	%rdi, 72(%rsp)
	movq	%rbx, 48(%rsp)
	movq	8(%rsi), %rbx
	movq	%rsi, 56(%rsp)
	movq	(%rsi), %rdi
	movq	%rbx, 32(%rsp)
	movq	%r12, %rbx
	movq	%rdx, 64(%rsp)
	cmpq	%rdi, 48(%rsp)
	je	.L38
	.p2align 4,,10
	.p2align 3
.L11:
	movq	%rdi, %rax
	subq	(%rbx), %rax
	js	.L39
	cmpq	0(%rbp), %rax
	jnb	.L40
	movq	(%r15), %rax
	movq	(%rcx), %r8
	movq	24(%rcx), %r11
	movq	16(%rcx), %r10
	imulq	%rdi, %rax
	movq	8(%rcx), %r14
	movq	%r8, %rdx
	movq	%rax, 8(%rsp)
	movq	32(%rcx), %rax
	subq	(%rax), %rdx
	js	.L41
	cmpq	(%r10), %rdx
	jnb	.L42
	movq	(%r11), %rsi
	movq	8(%r10), %r9
	imulq	%r8, %rsi
	movq	%rsi, 16(%rsp)
	cmpq	8(%rbp), %r9
	jne	.L43
	movq	8(%rax), %r13
	leaq	-1(%r9), %rax
	movq	8(%rbx), %r12
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	testq	%r9, %r9
	jle	.L31
	movq	%rcx, 40(%rsp)
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L35:
	addq	$1, %rax
.L28:
	leaq	(%r12,%rax), %rsi
	leaq	0(%r13,%rax), %rdx
	cmpq	%rax, %r9
	je	.L44
	movq	32(%rsp), %rcx
	imulq	8(%r15), %rsi
	addq	8(%rsp), %rsi
	leaq	(%rcx,%rsi,8), %rsi
	cmpq	8(%r10), %rax
	jnb	.L45
	movsd	(%rsi), %xmm0
	imulq	8(%r11), %rdx
	addq	16(%rsp), %rdx
	movsd	%xmm0, (%r14,%rdx,8)
	cmpq	%rax, 24(%rsp)
	jne	.L35
	movq	40(%rsp), %rcx
.L31:
	movq	56(%rsp), %rax
	addq	$1, %r8
	addq	$1, %rdi
	movq	%r8, (%rcx)
	movq	%rdi, (%rax)
	cmpq	%rdi, 48(%rsp)
	jne	.L11
.L38:
	movq	64(%rsp), %rax
	movq	32(%rsp), %rsi
	cmpq	%rsi, 8(%rax)
	jne	.L11
	movq	16(%rax), %rax
	cmpq	%rbp, %rax
	je	.L12
	movq	0(%rbp), %rdx
	movq	8(%rbp), %rsi
	xorq	(%rax), %rdx
	xorq	8(%rax), %rsi
	orq	%rdx, %rsi
	jne	.L11
.L12:
	movq	64(%rsp), %rax
	movq	24(%rax), %rax
	cmpq	%r15, %rax
	je	.L15
	movq	(%r15), %rdx
	movq	8(%r15), %rsi
	xorq	(%rax), %rdx
	xorq	8(%rax), %rsi
	orq	%rdx, %rsi
	jne	.L11
.L15:
	movq	64(%rsp), %rax
	movq	32(%rax), %rax
	cmpq	%rbx, %rax
	je	.L18
	movq	(%rbx), %rdx
	movq	8(%rbx), %rsi
	xorq	(%rax), %rdx
	xorq	8(%rax), %rsi
	orq	%rdx, %rsi
	jne	.L11
.L18:
	movq	72(%rsp), %rbx
	movq	32(%rcx), %rax
	movdqu	(%rcx), %xmm1
	movdqu	16(%rcx), %xmm2
	movq	%rax, 32(%rbx)
	movq	%rbx, %rax
	movups	%xmm1, (%rbx)
	movups	%xmm2, 16(%rbx)
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L45:
	.cfi_restore_state
	movl	$.LC0, %ecx
	movl	$178, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L44:
	movl	$.LC9, %ecx
	movl	$178, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L43:
	movl	$.LC6, %ecx
	movl	$235, %edx
	movl	$.LC7, %esi
	movl	$.LC8, %edi
	call	__assert_fail
.L42:
	movl	$.LC3, %ecx
	movl	$136, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L41:
	movl	$.LC3, %ecx
	movl	$135, %edx
	movl	$.LC1, %esi
	movl	$.LC4, %edi
	call	__assert_fail
.L40:
	movl	$.LC5, %ecx
	movl	$136, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L39:
	movl	$.LC5, %ecx
	movl	$135, %edx
	movl	$.LC1, %esi
	movl	$.LC4, %edi
	call	__assert_fail
	.cfi_endproc
.LFE6001:
	.size	_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_, .-_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.8,"aMS",@progbits,1
	.align 8
.LC10:
	.string	"const T& boost::array<T, N>::operator[](boost::array<T, N>::size_type) const [with T = bool; long unsigned int N = 2; boost::array<T, N>::const_reference = const bool&; boost::array<T, N>::size_type = long unsigned int]"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.1,"aMS",@progbits,1
.LC11:
	.string	"/usr/include/boost/array.hpp"
.LC12:
	.string	"(i < N)&&(\"out of range\")"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.8
	.align 8
.LC14:
	.ascii	"ArrayRef boost::detail::multi_array::multi_array_impl_base<T"
	.ascii	", NumDims>::generate_array_view(boost::type<Reference>, cons"
	.ascii	"t boost::detail::multi_array::index_gen<NumDims, NDims>&, co"
	.ascii	"nst size_type*, const index*, const index*, TPtr) const [wit"
	.ascii	"h ArrayRef = boost::detail::multi_array::multi_array_view<do"
	.ascii	"ub"
	.string	"le, 2>; int NDims = 2; TPtr = double*; T = double; long unsigned int NumDims = 2; boost::detail::multi_array::multi_array_impl_base<T, NumDims>::size_type = long unsigned int; boost::detail::multi_array::multi_array_impl_base<T, NumDims>::index = long int]"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.1
.LC15:
	.string	"stride != 0"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.8
	.align 8
.LC16:
	.string	"index_bases[n] <= start && ((start <= index_bases[n]+index(extents[n])) || (start == index_bases[n] && extents[n] == 0))"
	.align 8
.LC17:
	.string	"((index_bases[n] - bound_adjustment) <= finish) && (finish <= (index_bases[n] + index(extents[n]) - bound_adjustment))"
	.align 8
.LC18:
	.string	"T& boost::array<T, N>::operator[](boost::array<T, N>::size_type) [with T = long int; long unsigned int N = 2; boost::array<T, N>::reference = long int&; boost::array<T, N>::size_type = long unsigned int]"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.1
.LC19:
	.string	"dim == NDims"
	.section	.rodata._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE.str1.8
	.align 8
.LC20:
	.string	"boost::detail::multi_array::multi_array_view<T, NumDims>& boost::detail::multi_array::multi_array_view<T, NumDims>::operator=(const boost::detail::multi_array::multi_array_view<T, NumDims>&) [with T = double; long unsigned int NumDims = 2]"
	.align 8
.LC21:
	.string	"/usr/include/boost/multi_array/view.hpp"
	.align 8
.LC22:
	.string	"std::equal(other.shape(), other.shape()+this->num_dimensions(), this->shape())"
	.section	.text._ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE,"axG",@progbits,_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE,comdat
	.align 2
	.p2align 4
	.weak	_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE
	.type	_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE, @function
_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE:
.LFB5390:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$728, %rsp
	.cfi_def_cfa_offset 784
	movq	(%rsi), %rcx
	movq	8(%rsi), %rbx
	movq	24(%rdi), %rdx
	movq	8(%rdi), %rax
	movq	$0, 592(%rsp)
	subq	%rcx, %rbx
	movq	%rcx, 656(%rsp)
	movq	24(%rsi), %rcx
	movq	%rdx, 616(%rsp)
	movq	16(%rsi), %rdx
	movdqu	8(%rdi), %xmm0
	movq	%rcx, %rdi
	movq	%rbx, 8(%rsp)
	subq	%rdx, %rdi
	movq	%rdx, 664(%rsp)
	movq	%rbx, %rdx
	imulq	%rdi, %rdx
	movq	%rdi, 32(%rsp)
	movq	%rbx, 624(%rsp)
	movq	%rdi, 632(%rsp)
	movq	%rdx, 688(%rsp)
	movups	%xmm0, 600(%rsp)
	cmpq	$1, %rax
	ja	.L47
	cmpb	$1, 616(%rsp,%rax)
	sbbq	%rdx, %rdx
	orq	$1, %rdx
	movq	%rdx, 640(%rsp,%rax,8)
	movq	624(%rsp,%rax,8), %rdx
	movq	608(%rsp), %rax
	cmpq	$1, %rax
	ja	.L47
	cmpb	$0, 616(%rsp,%rax)
	jne	.L50
	negq	%rdx
.L50:
	movq	%rdx, 640(%rsp,%rax,8)
	movq	640(%rsp), %rax
	movq	656(%rsp), %rsi
	movq	664(%rsp), %rbx
	movq	648(%rsp), %rdi
	movq	%rax, 80(%rsp)
	imulq	%rsi, %rax
	movq	%rbx, 48(%rsp)
	movzbl	617(%rsp), %ecx
	imulq	%rdi, %rbx
	movq	%rdi, 88(%rsp)
	movq	%rsi, 24(%rsp)
	negq	%rax
	subq	%rbx, %rax
	cmpb	$0, 616(%rsp)
	movq	%rax, 16(%rsp)
	je	.L51
	testb	%cl, %cl
	jne	.L52
	movq	32(%rsp), %rbx
	leaq	-1(%rbx), %rdx
	imulq	%rdi, %rdx
	subq	%rdx, %rax
	movq	%rax, 16(%rsp)
	movq	%rax, 672(%rsp)
	xorl	%eax, %eax
.L87:
	subq	%rdx, %rax
	movq	%rax, 56(%rsp)
.L85:
	movq	56(%rsp), %rax
	movq	%rax, 680(%rsp)
	movq	688(%rsp), %rax
	movq	%rax, 40(%rsp)
	movq	%rax, %rbx
	movabsq	$1152921504606846975, %rax
	cmpq	%rax, %rbx
	ja	.L118
	movq	40(%rsp), %r15
	leaq	0(,%r15,8), %rbp
	movq	%rbp, %rdi
	call	_Znwm
	movq	%r15, 704(%rsp)
	leaq	(%rax,%rbp), %rdx
	movq	%rax, 696(%rsp)
	movq	%rax, %rbx
	movq	%rax, 592(%rsp)
	cmpq	%rdx, %rax
	je	.L56
	.p2align 4,,10
	.p2align 3
.L54:
	movq	$0x000000000, (%rax)
	addq	$8, %rax
	cmpq	%rax, %rdx
	jne	.L54
.L56:
	movq	24(%rsp), %rdi
	movq	8(%rsp), %rax
	movq	$1, 384(%rsp)
	leaq	32(%r12), %r10
	movq	32(%rsp), %rsi
	cmpq	%rax, 32(%r12)
	movb	$0, 392(%rsp)
	leaq	304(%rsp), %r8
	cmovbe	32(%r12), %rax
	movq	%rdi, 368(%rsp)
	cmpq	%rsi, 40(%r12)
	movq	%rsi, %rcx
	cmovbe	40(%r12), %rcx
	movq	$1, 416(%rsp)
	leaq	(%rdi,%rax), %rdx
	movq	48(%rsp), %rdi
	xorl	%r13d, %r13d
	xorl	%r11d, %r11d
	movq	%rdx, 376(%rsp)
	leaq	(%rdi,%rcx), %rdx
	movq	%rax, 96(%rsp)
	movq	%rdx, 408(%rsp)
	movq	64(%r12), %rdx
	movq	%rcx, 104(%rsp)
	movq	%rdx, 304(%rsp)
	addq	%rax, %rdx
	movq	72(%r12), %rax
	movq	%rdi, 400(%rsp)
	movq	%rax, 336(%rsp)
	addq	%rcx, %rax
	movq	%rax, 344(%rsp)
	movq	(%r12), %rax
	movb	$0, 424(%rsp)
	movq	%rax, 64(%rsp)
	movq	80(%r12), %rax
	movq	%rdx, 312(%rsp)
	movq	$1, 320(%rsp)
	movb	$0, 328(%rsp)
	movq	$1, 352(%rsp)
	movb	$0, 360(%rsp)
	movq	%rax, 72(%rsp)
.L55:
	movabsq	$-9223372036854775808, %rax
	movq	(%r8), %rcx
	movq	(%r10), %r14
	movabsq	$9223372036854775807, %rsi
	movq	32(%r10), %rdi
	movq	8(%r8), %r9
	cmpq	%rax, %rcx
	cmove	%rdi, %rcx
	cmpq	%rsi, %r9
	movq	16(%r8), %rsi
	leaq	(%rdi,%r14), %rax
	cmove	%rax, %r9
	testq	%rsi, %rsi
	je	.L72
	movq	%r9, %r15
	xorl	%ebp, %ebp
	subq	%rcx, %r15
	movq	%r15, %rax
	cqto
	idivq	%rsi
	testq	%rax, %rax
	js	.L60
	xorl	%eax, %eax
	testq	%rsi, %rsi
	movq	%rsi, %rdx
	setg	%al
	leaq	-1(%rax,%rax), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	addq	%r15, %rax
	cqto
	idivq	%rsi
	movq	%rax, %rbp
.L60:
	cmpq	%rcx, %rdi
	jg	.L64
	leaq	(%r14,%rdi), %rax
	cmpq	%rax, %rcx
	jle	.L63
	testq	%r14, %r14
	jne	.L64
	cmpq	%rcx, %rdi
	jne	.L64
.L63:
	movq	%rsi, %rdx
	shrq	$63, %rdx
	subq	%rdx, %rdi
	cmpq	%rdi, %r9
	jl	.L66
	subq	%rdx, %rax
	cmpq	%rax, %r9
	jg	.L66
	movq	16(%r10), %rax
	imulq	%rax, %rcx
	addq	%rcx, %r11
	cmpb	$0, 24(%r8)
	jne	.L67
	cmpq	$1, %r13
	ja	.L77
	imulq	%rax, %rsi
	movq	%rbp, 512(%rsp,%r13,8)
	movq	%rsi, 160(%rsp,%r13,8)
	addq	$1, %r13
.L67:
	addq	$32, %r8
	leaq	368(%rsp), %rax
	addq	$8, %r10
	cmpq	%r8, %rax
	jne	.L55
	cmpq	$2, %r13
	jne	.L80
	movq	64(%rsp), %rax
	addq	72(%rsp), %r11
	xorl	%r14d, %r14d
	movq	$0, 440(%rsp)
	movq	512(%rsp), %rsi
	movq	160(%rsp), %rdx
	movq	$0, 488(%rsp)
	leaq	368(%rsp), %r8
	leaq	(%rax,%r11,8), %rax
	movq	8(%rsp), %r13
	movq	24(%rsp), %rdi
	xorl	%r11d, %r11d
	movq	%rax, 64(%rsp)
	leaq	592(%rsp), %r10
	movq	%rax, 432(%rsp)
	movq	520(%rsp), %rax
	movq	%rdx, 472(%rsp)
	movq	168(%rsp), %rdx
	movq	%rax, 464(%rsp)
	imulq	%rsi, %rax
	movq	$0, 496(%rsp)
	movq	%rsi, 72(%rsp)
	movq	%rsi, 456(%rsp)
	movq	%rdx, 480(%rsp)
	movq	%rax, 448(%rsp)
.L79:
	movq	(%r8), %rcx
	movq	8(%r8), %r9
	movabsq	$-9223372036854775808, %rax
	movabsq	$9223372036854775807, %rsi
	cmpq	%rax, %rcx
	leaq	(%rdi,%r13), %rax
	cmove	%rdi, %rcx
	cmpq	%rsi, %r9
	movq	16(%r8), %rsi
	cmove	%rax, %r9
	testq	%rsi, %rsi
	je	.L72
	movq	%r9, %rbp
	xorl	%r15d, %r15d
	subq	%rcx, %rbp
	movq	%rbp, %rax
	cqto
	idivq	%rsi
	testq	%rax, %rax
	js	.L73
	xorl	%eax, %eax
	testq	%rsi, %rsi
	movq	%rsi, %rdx
	setg	%al
	leaq	-1(%rax,%rax), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	addq	%rbp, %rax
	cqto
	idivq	%rsi
	movq	%rax, %r15
.L73:
	cmpq	%rdi, %rcx
	jl	.L64
	leaq	0(%r13,%rdi), %rbp
	cmpq	%rbp, %rcx
	jle	.L75
	testq	%r13, %r13
	jne	.L64
	cmpq	%rdi, %rcx
	jne	.L64
.L75:
	movq	%rsi, %rax
	shrq	$63, %rax
	subq	%rax, %rdi
	cmpq	%rdi, %r9
	jl	.L66
	subq	%rax, %rbp
	cmpq	%rbp, %r9
	jg	.L66
	movq	48(%r10), %rax
	imulq	%rax, %rcx
	addq	%rcx, %r11
	cmpb	$0, 24(%r8)
	jne	.L76
	cmpq	$1, %r14
	ja	.L77
	imulq	%rax, %rsi
	movq	%r15, 256(%rsp,%r14,8)
	movq	%rsi, 208(%rsp,%r14,8)
	addq	$1, %r14
.L76:
	addq	$32, %r8
	leaq	432(%rsp), %rax
	addq	$8, %r10
	cmpq	%rax, %r8
	je	.L78
	movq	64(%r10), %rdi
	movq	32(%r10), %r13
	jmp	.L79
	.p2align 4,,10
	.p2align 3
.L51:
	movq	8(%rsp), %rax
	subq	$1, %rax
	imulq	80(%rsp), %rax
	testb	%cl, %cl
	je	.L119
	subq	%rax, 16(%rsp)
	movq	16(%rsp), %rbx
	negq	%rax
	movq	%rax, 56(%rsp)
	movq	%rbx, 672(%rsp)
	jmp	.L85
	.p2align 4,,10
	.p2align 3
.L78:
	cmpq	$2, %r14
	jne	.L80
	movq	208(%rsp), %rsi
	movq	256(%rsp), %rax
	movq	$0, 520(%rsp)
	movq	264(%rsp), %rdx
	movq	16(%rsp), %r14
	movq	$0, 568(%rsp)
	movq	%rsi, 552(%rsp)
	movq	216(%rsp), %rsi
	addq	%r14, %r11
	movq	%rax, 536(%rsp)
	movq	%rsi, 560(%rsp)
	movq	%rax, %rsi
	xorq	456(%rsp), %rax
	leaq	(%rbx,%r11,8), %rcx
	imulq	%rdx, %rsi
	movq	%rdx, 544(%rsp)
	xorq	464(%rsp), %rdx
	orq	%rax, %rdx
	movq	%rcx, 512(%rsp)
	movq	$0, 576(%rsp)
	movq	%rsi, 528(%rsp)
	jne	.L81
	leaq	536(%rsp), %rax
	movq	64(%rsp), %rsi
	leaq	472(%rsp), %rdx
	movq	%rcx, 264(%rsp)
	movq	%rax, 272(%rsp)
	leaq	552(%rsp), %rax
	leaq	456(%rsp), %rcx
	movq	%rax, 280(%rsp)
	leaq	568(%rsp), %rax
	leaq	112(%rsp), %rdi
	movq	%rax, 288(%rsp)
	movq	72(%rsp), %rax
	movq	%rsi, 216(%rsp)
	movq	%rax, 208(%rsp)
	leaq	488(%rsp), %rax
	movq	%rcx, 224(%rsp)
	movq	%rdx, 232(%rsp)
	movq	%rsi, 168(%rsp)
	leaq	160(%rsp), %rsi
	movq	%rcx, 176(%rsp)
	leaq	256(%rsp), %rcx
	movq	%rdx, 184(%rsp)
	leaq	208(%rsp), %rdx
	movq	%rax, 240(%rsp)
	movq	%rax, 192(%rsp)
	movq	$0, 256(%rsp)
	movq	$0, 160(%rsp)
	call	_ZNSt11__copy_moveILb0ELb0EN5boost9iterators6detail32iterator_category_with_traversalISt18input_iterator_tagNS1_27random_access_traversal_tagEEEE8__copy_mINS0_6detail11multi_array14array_iteratorIdPKdN4mpl_6size_tILm2EEENSA_15const_sub_arrayIdLm1ESD_EES5_EENSB_IdPdSG_NSA_9sub_arrayIdLm1EEES5_EEEET0_T_SP_SO_
	movq	104(%r12), %rdi
	movq	%rbx, (%r12)
	movzwl	616(%rsp), %eax
	movq	%r14, 80(%r12)
	movdqu	600(%rsp), %xmm1
	movq	%rbx, 104(%r12)
	movw	%ax, 24(%r12)
	movq	8(%rsp), %rax
	movups	%xmm1, 8(%r12)
	movq	%rax, 32(%r12)
	movq	32(%rsp), %rax
	movq	%rax, 40(%r12)
	movq	80(%rsp), %rax
	movq	%rax, 48(%r12)
	movq	88(%rsp), %rax
	movq	%rax, 56(%r12)
	movq	24(%rsp), %rax
	movq	%rax, 64(%r12)
	movq	48(%rsp), %rax
	movq	%rax, 72(%r12)
	movq	56(%rsp), %rax
	movq	%rax, 88(%r12)
	movq	40(%rsp), %rax
	movq	%rax, 96(%r12)
	movq	%rax, 112(%r12)
	testq	%rdi, %rdi
	je	.L100
	call	_ZdlPv
.L100:
	addq	$728, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movq	%r12, %rax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L81:
	.cfi_restore_state
	movl	$.LC20, %ecx
	movl	$314, %edx
	movl	$.LC21, %esi
	movl	$.LC22, %edi
	call	__assert_fail
	.p2align 4,,10
	.p2align 3
.L52:
	movq	$0, 56(%rsp)
	movq	16(%rsp), %rax
	movq	%rax, 672(%rsp)
	jmp	.L85
.L119:
	movq	32(%rsp), %rbx
	negq	%rax
	movq	%rax, %rcx
	leaq	-1(%rbx), %rdx
	imulq	88(%rsp), %rdx
	subq	%rdx, %rcx
	addq	%rcx, 16(%rsp)
	movq	16(%rsp), %rbx
	movq	%rbx, 672(%rsp)
	jmp	.L87
.L66:
	movl	$.LC14, %ecx
	movl	$464, %edx
	movl	$.LC1, %esi
	movl	$.LC17, %edi
	call	__assert_fail
.L64:
	movl	$.LC14, %ecx
	movl	$455, %edx
	movl	$.LC1, %esi
	movl	$.LC16, %edi
	call	__assert_fail
.L72:
	movl	$.LC14, %ecx
	movl	$430, %edx
	movl	$.LC1, %esi
	movl	$.LC15, %edi
	call	__assert_fail
.L47:
	movl	$.LC10, %ecx
	movl	$123, %edx
	movl	$.LC11, %esi
	movl	$.LC12, %edi
	call	__assert_fail
.L77:
	movl	$.LC18, %ecx
	movl	$118, %edx
	movl	$.LC11, %esi
	movl	$.LC12, %edi
	call	__assert_fail
.L80:
	movl	$.LC14, %ecx
	movl	$485, %edx
	movl	$.LC1, %esi
	movl	$.LC19, %edi
	call	__assert_fail
.L118:
	call	_ZSt17__throw_bad_allocv
	.cfi_endproc
.LFE5390:
	.size	_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE, .-_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE
	.text
	.p2align 4
	.globl	_Z7FI_RatedP9Variables
	.type	_Z7FI_RatedP9Variables, @function
_Z7FI_RatedP9Variables:
.LFB5025:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$984, %rsp
	.cfi_def_cfa_offset 1008
	movq	3088(%rdi), %rax
	movsd	%xmm0, 128(%rsp)
	movsd	(%rax), %xmm7
	movsd	16(%rax), %xmm5
	movsd	56(%rax), %xmm3
	movsd	80(%rax), %xmm6
	movsd	%xmm7, 136(%rsp)
	movsd	8(%rax), %xmm7
	movsd	48(%rax), %xmm12
	movsd	%xmm5, 16(%rsp)
	movsd	32(%rax), %xmm5
	movsd	64(%rax), %xmm4
	movsd	%xmm7, 144(%rsp)
	movsd	24(%rax), %xmm7
	movsd	%xmm5, 24(%rsp)
	movsd	72(%rax), %xmm5
	movsd	%xmm7, 56(%rsp)
	movsd	40(%rax), %xmm7
	movsd	%xmm3, 72(%rsp)
	movsd	96(%rax), %xmm3
	movsd	%xmm7, 152(%rsp)
	movsd	88(%rax), %xmm7
	movsd	%xmm6, 88(%rsp)
	movsd	112(%rax), %xmm6
	movsd	%xmm5, 80(%rsp)
	movsd	104(%rax), %xmm5
	movsd	%xmm3, 104(%rsp)
	movsd	%xmm6, 328(%rsp)
	movsd	%xmm7, 96(%rsp)
	movsd	120(%rax), %xmm7
	movsd	128(%rax), %xmm3
	movsd	136(%rax), %xmm6
	movsd	%xmm7, 336(%rsp)
	movsd	144(%rax), %xmm7
	movsd	%xmm3, 344(%rsp)
	movsd	152(%rax), %xmm3
	movsd	%xmm6, 352(%rsp)
	movsd	160(%rax), %xmm6
	movq	3040(%rdi), %rax
	movsd	%xmm7, 360(%rsp)
	movsd	%xmm6, 376(%rsp)
	movsd	40(%rax), %xmm6
	movsd	(%rax), %xmm15
	movsd	8(%rax), %xmm10
	movsd	16(%rax), %xmm0
	movsd	%xmm3, 160(%rsp)
	movsd	%xmm6, 168(%rsp)
	movsd	56(%rax), %xmm6
	movsd	24(%rax), %xmm7
	movsd	32(%rax), %xmm3
	movsd	%xmm6, 264(%rsp)
	movsd	64(%rax), %xmm6
	movsd	%xmm6, 272(%rsp)
	movsd	72(%rax), %xmm6
	movsd	%xmm6, 280(%rsp)
	movsd	80(%rax), %xmm6
	movsd	%xmm6, 288(%rsp)
	movsd	88(%rax), %xmm6
	movsd	%xmm6, 232(%rsp)
	movsd	96(%rax), %xmm6
	movsd	%xmm6, 240(%rsp)
	movsd	104(%rax), %xmm6
	movsd	160(%rax), %xmm2
	cmpb	$0, (%rdi)
	movsd	%xmm6, 248(%rsp)
	movsd	112(%rax), %xmm6
	movsd	%xmm2, 40(%rsp)
	movsd	168(%rax), %xmm2
	movsd	%xmm6, 256(%rsp)
	movsd	120(%rax), %xmm6
	movsd	%xmm2, 384(%rsp)
	movsd	%xmm6, 176(%rsp)
	movsd	128(%rax), %xmm6
	movsd	%xmm6, 8(%rsp)
	movsd	136(%rax), %xmm6
	movsd	%xmm6, 184(%rsp)
	movsd	144(%rax), %xmm6
	movsd	%xmm6, 32(%rsp)
	movsd	152(%rax), %xmm6
	movq	3064(%rdi), %rax
	movsd	8(%rax), %xmm9
	movq	2944(%rdi), %rax
	movsd	8(%rax), %xmm1
	jne	.L121
	movapd	%xmm9, %xmm8
	subsd	%xmm2, %xmm8
	subsd	%xmm1, %xmm8
	movsd	%xmm8, 392(%rsp)
.L122:
	movsd	.LC23(%rip), %xmm1
	movq	3112(%rbx), %rax
	movsd	288(%rbx), %xmm13
	movsd	296(%rbx), %xmm11
	movapd	%xmm1, %xmm8
	movsd	(%rax), %xmm14
	subsd	%xmm3, %xmm8
	subsd	168(%rsp), %xmm8
	mulsd	%xmm14, %xmm11
	subsd	%xmm7, %xmm8
	subsd	%xmm0, %xmm8
	movsd	%xmm8, 448(%rsp)
	movsd	304(%rbx), %xmm8
	addsd	%xmm8, %xmm13
	subsd	296(%rbx), %xmm8
	mulsd	%xmm14, %xmm8
	movsd	144(%rsp), %xmm14
	divsd	%xmm13, %xmm11
	mulsd	%xmm15, %xmm14
	movsd	%xmm14, 200(%rsp)
	movsd	152(%rsp), %xmm14
	mulsd	%xmm10, %xmm14
	movsd	%xmm14, 64(%rsp)
	movsd	24(%rsp), %xmm14
	divsd	%xmm13, %xmm8
	movsd	%xmm11, 112(%rsp)
	movsd	8(%rsp), %xmm11
	movsd	%xmm8, 368(%rsp)
	movsd	176(%rsp), %xmm8
	addsd	184(%rsp), %xmm8
	addsd	%xmm6, %xmm8
	addsd	%xmm8, %xmm11
	addsd	32(%rsp), %xmm11
	movapd	%xmm8, %xmm13
	addsd	40(%rsp), %xmm11
	divsd	%xmm11, %xmm13
	movapd	%xmm13, %xmm11
	movsd	136(%rsp), %xmm13
	subsd	%xmm11, %xmm1
	mulsd	%xmm15, %xmm13
	movsd	%xmm13, 192(%rsp)
	movsd	16(%rsp), %xmm13
	mulsd	%xmm15, %xmm13
	movsd	%xmm13, 48(%rsp)
	movsd	56(%rsp), %xmm13
	mulsd	%xmm10, %xmm13
	movsd	%xmm13, 56(%rsp)
	movapd	%xmm14, %xmm13
	mulsd	%xmm10, %xmm13
	mulsd	%xmm1, %xmm13
	movsd	%xmm13, 208(%rsp)
	movapd	%xmm13, %xmm2
	movsd	112(%rsp), %xmm13
	addsd	48(%rsp), %xmm13
	subsd	56(%rsp), %xmm13
	subsd	64(%rsp), %xmm13
	subsd	%xmm2, %xmm13
	movsd	%xmm13, 120(%rsp)
	movapd	%xmm14, %xmm13
	mulsd	%xmm0, %xmm13
	mulsd	%xmm1, %xmm13
	movsd	%xmm13, 480(%rsp)
	movapd	%xmm13, %xmm2
	movapd	%xmm1, %xmm13
	mulsd	%xmm0, %xmm13
	movapd	%xmm13, %xmm14
	movapd	%xmm12, %xmm13
	mulsd	%xmm0, %xmm13
	mulsd	.LC13(%rip), %xmm0
	mulsd	%xmm12, %xmm14
	divsd	.LC24(%rip), %xmm14
	mulsd	%xmm11, %xmm13
	mulsd	%xmm1, %xmm0
	mulsd	%xmm7, %xmm1
	addsd	%xmm14, %xmm13
	mulsd	%xmm12, %xmm0
	movapd	%xmm13, %xmm12
	movsd	120(%rsp), %xmm13
	addsd	%xmm0, %xmm12
	movsd	72(%rsp), %xmm0
	mulsd	%xmm0, %xmm1
	movsd	%xmm12, 216(%rsp)
	mulsd	%xmm7, %xmm0
	mulsd	.LC25(%rip), %xmm1
	mulsd	%xmm11, %xmm0
	addsd	%xmm0, %xmm1
	movsd	%xmm1, 224(%rsp)
	subsd	%xmm1, %xmm12
	movsd	80(%rsp), %xmm1
	mulsd	232(%rsp), %xmm1
	subsd	%xmm12, %xmm13
	movsd	%xmm1, 232(%rsp)
	movsd	88(%rsp), %xmm1
	mulsd	240(%rsp), %xmm1
	movapd	%xmm13, %xmm12
	subsd	%xmm2, %xmm12
	movsd	%xmm12, 488(%rsp)
	movsd	272(%rsp), %xmm12
	movsd	%xmm1, 240(%rsp)
	movsd	96(%rsp), %xmm1
	mulsd	248(%rsp), %xmm1
	mulsd	%xmm4, %xmm12
	movsd	%xmm1, 248(%rsp)
	movsd	104(%rsp), %xmm1
	mulsd	256(%rsp), %xmm1
	movapd	%xmm12, %xmm0
	mulsd	%xmm7, %xmm12
	mulsd	%xmm3, %xmm0
	movsd	%xmm1, 256(%rsp)
	movsd	264(%rsp), %xmm1
	movsd	%xmm12, 16(%rsp)
	mulsd	%xmm4, %xmm1
	movsd	280(%rsp), %xmm12
	mulsd	%xmm4, %xmm12
	mulsd	288(%rsp), %xmm4
	movapd	%xmm1, %xmm14
	mulsd	%xmm7, %xmm14
	mulsd	%xmm3, %xmm1
	movapd	%xmm12, %xmm13
	mulsd	%xmm7, %xmm13
	mulsd	%xmm3, %xmm12
	movsd	%xmm14, 264(%rsp)
	movapd	%xmm13, %xmm2
	movapd	%xmm4, %xmm13
	mulsd	%xmm7, %xmm13
	movsd	%xmm2, 72(%rsp)
	mulsd	%xmm3, %xmm4
	movsd	%xmm13, 80(%rsp)
	movsd	16(%rsp), %xmm13
	addsd	%xmm14, %xmm13
	addsd	%xmm2, %xmm13
	movapd	%xmm13, %xmm2
	movsd	80(%rsp), %xmm13
	addsd	%xmm13, %xmm2
	movsd	%xmm2, 272(%rsp)
	movapd	%xmm12, %xmm2
	movapd	%xmm1, %xmm12
	movsd	%xmm1, 288(%rsp)
	movsd	%xmm0, 88(%rsp)
	movapd	%xmm1, %xmm0
	movsd	88(%rsp), %xmm1
	movsd	%xmm4, 104(%rsp)
	movapd	%xmm14, %xmm4
	movsd	384(%rsp), %xmm14
	addsd	%xmm1, %xmm0
	movsd	%xmm2, 96(%rsp)
	addsd	%xmm12, %xmm4
	movsd	184(%rsp), %xmm12
	movsd	%xmm4, 296(%rsp)
	addsd	%xmm2, %xmm0
	movsd	16(%rsp), %xmm4
	addsd	%xmm1, %xmm4
	movsd	176(%rsp), %xmm1
	movapd	%xmm0, %xmm2
	movsd	104(%rsp), %xmm0
	addsd	%xmm0, %xmm2
	addsd	%xmm0, %xmm13
	movsd	%xmm4, 304(%rsp)
	movsd	72(%rsp), %xmm4
	addsd	96(%rsp), %xmm4
	movsd	%xmm2, 280(%rsp)
	movsd	328(%rsp), %xmm2
	mulsd	8(%rsp), %xmm2
	movsd	%xmm13, 320(%rsp)
	movsd	392(%rsp), %xmm13
	movsd	%xmm4, 312(%rsp)
	movapd	%xmm13, %xmm4
	mulsd	%xmm6, %xmm4
	movsd	%xmm2, 328(%rsp)
	movsd	336(%rsp), %xmm2
	mulsd	%xmm12, %xmm2
	movapd	%xmm4, %xmm0
	movsd	%xmm2, 184(%rsp)
	movsd	344(%rsp), %xmm2
	mulsd	32(%rsp), %xmm2
	movsd	%xmm2, 336(%rsp)
	movsd	352(%rsp), %xmm2
	mulsd	%xmm6, %xmm2
	divsd	%xmm8, %xmm6
	movsd	%xmm2, 344(%rsp)
	movsd	360(%rsp), %xmm2
	mulsd	%xmm2, %xmm0
	movapd	%xmm0, %xmm4
	movapd	%xmm1, %xmm0
	mulsd	%xmm14, %xmm0
	mulsd	160(%rsp), %xmm0
	divsd	%xmm9, %xmm4
	divsd	%xmm8, %xmm1
	movsd	%xmm4, 352(%rsp)
	movapd	%xmm0, %xmm4
	divsd	%xmm9, %xmm4
	movsd	%xmm4, 176(%rsp)
	movsd	40(%rsp), %xmm4
	mulsd	%xmm13, %xmm4
	movapd	%xmm4, %xmm0
	mulsd	%xmm2, %xmm0
	movapd	%xmm0, %xmm2
	divsd	%xmm9, %xmm2
	movsd	%xmm2, 360(%rsp)
	movsd	8(%rsp), %xmm0
	mulsd	%xmm14, %xmm0
	mulsd	160(%rsp), %xmm0
	movapd	%xmm0, %xmm2
	movsd	168(%rsp), %xmm0
	divsd	%xmm9, %xmm2
	mulsd	%xmm5, %xmm0
	mulsd	%xmm11, %xmm0
	movsd	%xmm0, 168(%rsp)
	movsd	%xmm2, 160(%rsp)
	movsd	376(%rsp), %xmm2
	mulsd	%xmm14, %xmm2
	movsd	%xmm2, 376(%rsp)
	movapd	%xmm5, %xmm2
	mulsd	%xmm7, %xmm2
	movapd	%xmm2, %xmm4
	mulsd	%xmm11, %xmm4
	movapd	%xmm0, %xmm11
	movapd	%xmm1, %xmm0
	movapd	%xmm12, %xmm1
	movapd	%xmm0, %xmm2
	divsd	%xmm8, %xmm1
	movsd	.LC27(%rip), %xmm8
	mulsd	%xmm11, %xmm2
	movapd	%xmm4, %xmm14
	movapd	%xmm4, %xmm12
	movsd	%xmm4, 384(%rsp)
	mulsd	%xmm0, %xmm12
	movsd	.LC26(%rip), %xmm0
	mulsd	%xmm6, %xmm4
	movsd	%xmm2, 416(%rsp)
	movsd	%xmm12, 392(%rsp)
	movsd	%xmm4, 408(%rsp)
	mulsd	%xmm1, %xmm14
	movapd	%xmm1, %xmm2
	movsd	32(%rsp), %xmm1
	mulsd	%xmm11, %xmm2
	mulsd	%xmm5, %xmm1
	movsd	%xmm14, 400(%rsp)
	movsd	%xmm2, 424(%rsp)
	movapd	%xmm6, %xmm2
	movsd	448(%rsp), %xmm6
	mulsd	%xmm11, %xmm2
	divsd	%xmm0, %xmm1
	movsd	%xmm2, 432(%rsp)
	movsd	8(%rsp), %xmm2
	mulsd	%xmm5, %xmm2
	mulsd	40(%rsp), %xmm5
	divsd	%xmm0, %xmm2
	movapd	%xmm1, %xmm12
	mulsd	%xmm3, %xmm12
	movsd	%xmm12, 32(%rsp)
	divsd	%xmm0, %xmm5
	movsd	24(%rsp), %xmm0
	addsd	152(%rsp), %xmm0
	movapd	%xmm2, %xmm11
	mulsd	%xmm3, %xmm11
	mulsd	%xmm6, %xmm2
	movsd	%xmm11, 8(%rsp)
	addsd	%xmm12, %xmm11
	movsd	%xmm2, 448(%rsp)
	movapd	%xmm5, %xmm4
	mulsd	%xmm3, %xmm4
	addsd	%xmm7, %xmm3
	movsd	.LC28(%rip), %xmm7
	mulsd	%xmm6, %xmm5
	addsd	%xmm4, %xmm11
	movsd	%xmm4, 40(%rsp)
	movapd	%xmm1, %xmm4
	movapd	%xmm10, %xmm1
	movsd	%xmm5, 464(%rsp)
	mulsd	%xmm8, %xmm1
	mulsd	%xmm7, %xmm10
	movsd	%xmm11, 440(%rsp)
	mulsd	%xmm6, %xmm4
	mulsd	%xmm15, %xmm8
	mulsd	%xmm15, %xmm7
	mulsd	%xmm0, %xmm1
	movapd	%xmm10, %xmm0
	mulsd	%xmm3, %xmm0
	addsd	%xmm4, %xmm2
	movsd	%xmm4, 456(%rsp)
	mulsd	%xmm7, %xmm3
	mulsd	%xmm13, %xmm1
	movapd	%xmm2, %xmm6
	addsd	%xmm5, %xmm6
	movsd	%xmm6, 472(%rsp)
	divsd	%xmm9, %xmm1
	addsd	%xmm0, %xmm1
	movsd	%xmm1, 24(%rsp)
	movsd	136(%rsp), %xmm0
	movq	40(%rbx), %rdx
	addsd	144(%rsp), %xmm0
	mulsd	%xmm0, %xmm8
	mulsd	%xmm13, %xmm8
	divsd	%xmm9, %xmm8
	addsd	%xmm3, %xmm8
	movsd	%xmm8, 136(%rsp)
	testq	%rdx, %rdx
	jne	.L123
	movq	$1, 40(%rbx)
	movl	$1, %edx
.L123:
	movq	32(%rbx), %rax
	testq	%rax, %rax
	js	.L125
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
.L126:
	movsd	128(%rsp), %xmm3
	comisd	%xmm0, %xmm3
	jbe	.L124
	movsd	.LC29(%rip), %xmm0
	addq	$1, %rdx
	movq	%rdx, 40(%rbx)
	comisd	%xmm0, %xmm3
	jb	.L141
	movsd	128(%rsp), %xmm5
	subsd	%xmm0, %xmm5
	cvttsd2siq	%xmm5, %rax
	movq	%rax, 32(%rbx)
	btcq	$63, 32(%rbx)
.L124:
	leaq	4240(%rbx), %rbp
	cmpq	%rdx, 4272(%rbx)
	jb	.L142
.L130:
	subq	$1, %rdx
	movq	%rbp, %rsi
	leaq	496(%rsp), %rdi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	520(%rsp), %rax
	movq	504(%rsp), %rcx
	movq	496(%rsp), %rdx
	movq	(%rax), %rax
	movq	%rax, %rsi
	negq	%rsi
	testq	%rax, %rax
	jg	.L135
	cmpq	(%rcx), %rsi
	jnb	.L136
	movq	40(%rbx), %rax
	movq	4240(%rbx), %rsi
	movsd	128(%rsp), %xmm5
	movsd	%xmm5, (%rdx)
	leaq	-1(%rax), %rdx
	movq	4320(%rbx), %rax
	movq	%rdx, %rcx
	subq	4304(%rbx), %rcx
	js	.L137
	cmpq	4272(%rbx), %rcx
	jnb	.L138
	movq	4288(%rbx), %rdi
	movl	$1, %ecx
	imulq	%rdx, %rdi
	subq	4312(%rbx), %rcx
	js	.L135
	cmpq	4280(%rbx), %rcx
	jnb	.L136
	movsd	192(%rsp), %xmm3
	addq	4296(%rbx), %rax
	addq	%rdi, %rax
	leaq	528(%rsp), %rdi
	movsd	%xmm3, (%rsi,%rax,8)
	movq	%rbp, %rsi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	552(%rsp), %rdx
	movq	544(%rsp), %rsi
	movl	$2, %eax
	movq	536(%rsp), %rdi
	movq	528(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	movq	4320(%rbx), %r8
	movsd	200(%rsp), %xmm7
	movq	4240(%rbx), %rsi
	addq	%rax, %rax
	movsd	%xmm7, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	movq	%rdx, %rcx
	subq	4304(%rbx), %rcx
	js	.L137
	cmpq	4272(%rbx), %rcx
	jnb	.L138
	movq	4288(%rbx), %rdi
	movl	$3, %ecx
	imulq	%rdx, %rdi
	subq	4312(%rbx), %rcx
	js	.L135
	cmpq	4280(%rbx), %rcx
	jnb	.L136
	movq	4296(%rbx), %rcx
	movsd	48(%rsp), %xmm5
	leaq	(%rcx,%rcx,2), %rax
	addq	%r8, %rax
	addq	%rdi, %rax
	leaq	560(%rsp), %rdi
	movsd	%xmm5, (%rsi,%rax,8)
	movq	%rbp, %rsi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	584(%rsp), %rdx
	movq	576(%rsp), %rsi
	movl	$4, %eax
	movq	568(%rsp), %rdi
	movq	560(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	movsd	56(%rsp), %xmm6
	movq	4240(%rbx), %r9
	movq	4320(%rbx), %rsi
	salq	$2, %rax
	movsd	%xmm6, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %r8
	movq	%r8, %rax
	subq	4304(%rbx), %rax
	js	.L137
	cmpq	4272(%rbx), %rax
	jnb	.L138
	movq	4288(%rbx), %rax
	movq	4312(%rbx), %rdx
	imulq	%r8, %rax
	addq	%rax, %rsi
	movl	$5, %eax
	subq	%rdx, %rax
	js	.L135
	movq	4280(%rbx), %rcx
	cmpq	%rcx, %rax
	jnb	.L136
	movq	4296(%rbx), %rdi
	movsd	64(%rsp), %xmm2
	leaq	(%rdi,%rdi,4), %rax
	addq	%rsi, %rax
	movl	$6, %esi
	leaq	(%r9,%rax,8), %rax
	movsd	%xmm2, (%rax)
	subq	%rdx, %rsi
	js	.L135
	cmpq	%rcx, %rsi
	jnb	.L136
	movsd	208(%rsp), %xmm4
	leaq	0(,%rdi,8), %rsi
	movl	$7, %r9d
	addq	%rsi, %rax
	movsd	%xmm4, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	216(%rsp), %xmm1
	addq	%rsi, %rax
	movl	$8, %r9d
	movsd	%xmm1, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	224(%rsp), %xmm0
	addq	%rsi, %rax
	movl	$9, %r9d
	movsd	%xmm0, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	232(%rsp), %xmm15
	addq	%rsi, %rax
	movl	$10, %r9d
	movsd	%xmm15, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	240(%rsp), %xmm9
	addq	%rsi, %rax
	movl	$11, %r9d
	movsd	%xmm9, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	248(%rsp), %xmm10
	addq	%rsi, %rax
	movl	$12, %r9d
	movsd	%xmm10, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	256(%rsp), %xmm14
	addq	%rsi, %rax
	movl	$13, %r9d
	movsd	%xmm14, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	272(%rsp), %xmm8
	addq	%rsi, %rax
	movl	$14, %r9d
	movsd	%xmm8, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	264(%rsp), %xmm11
	addq	%rsi, %rax
	movl	$15, %r9d
	movsd	%xmm11, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	16(%rsp), %xmm12
	addq	%rsi, %rax
	movl	$16, %r9d
	movsd	%xmm12, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	72(%rsp), %xmm13
	addq	%rsi, %rax
	movl	$17, %r9d
	movsd	%xmm13, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	80(%rsp), %xmm12
	addq	%rsi, %rax
	movl	$18, %r9d
	movsd	%xmm12, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	280(%rsp), %xmm13
	addq	%rsi, %rax
	movl	$19, %r9d
	movsd	%xmm13, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	288(%rsp), %xmm8
	addq	%rsi, %rax
	movl	$20, %r9d
	movsd	%xmm8, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	88(%rsp), %xmm11
	addq	%rsi, %rax
	movl	$21, %r9d
	movsd	%xmm11, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	96(%rsp), %xmm10
	addq	%rsi, %rax
	movl	$22, %r9d
	movsd	%xmm10, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	104(%rsp), %xmm14
	addq	%rsi, %rax
	movl	$23, %r9d
	movsd	%xmm14, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	296(%rsp), %xmm15
	addq	%rsi, %rax
	movl	$24, %r9d
	movsd	%xmm15, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	304(%rsp), %xmm9
	addq	%rsi, %rax
	movl	$25, %r9d
	movsd	%xmm9, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	312(%rsp), %xmm1
	addq	%rsi, %rax
	movl	$26, %r9d
	movsd	%xmm1, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	320(%rsp), %xmm0
	addq	%rsi, %rax
	movl	$27, %r9d
	movsd	%xmm0, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	328(%rsp), %xmm2
	addq	%rsi, %rax
	movl	$28, %r9d
	movsd	%xmm2, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	184(%rsp), %xmm4
	addq	%rsi, %rax
	movl	$29, %r9d
	movsd	%xmm4, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	336(%rsp), %xmm5
	addq	%rsi, %rax
	movl	$30, %r9d
	movsd	%xmm5, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	344(%rsp), %xmm6
	addq	%rsi, %rax
	movl	$31, %r9d
	movsd	%xmm6, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	352(%rsp), %xmm3
	addq	%rsi, %rax
	movl	$32, %r9d
	movsd	%xmm3, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	176(%rsp), %xmm7
	addq	%rsi, %rax
	movl	$33, %r9d
	movsd	%xmm7, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	360(%rsp), %xmm7
	addq	%rsi, %rax
	movl	$34, %esi
	movsd	%xmm7, (%rax)
	subq	%rdx, %rsi
	js	.L135
	cmpq	%rcx, %rsi
	jnb	.L136
	movsd	160(%rsp), %xmm3
	movq	%r8, %rdx
	movq	%rbp, %rsi
	movsd	%xmm3, (%rax,%rdi,8)
	leaq	592(%rsp), %rdi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	616(%rsp), %rdx
	movq	608(%rsp), %rsi
	movl	$35, %eax
	movq	600(%rsp), %rdi
	movq	592(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rdx
	leaq	624(%rsp), %rdi
	movq	%rbp, %rsi
	movsd	376(%rsp), %xmm5
	leaq	(%rdx,%rdx,8), %rax
	salq	$2, %rax
	subq	%rdx, %rax
	movsd	%xmm5, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	648(%rsp), %rdx
	movq	640(%rsp), %rsi
	movl	$36, %eax
	movq	632(%rsp), %rdi
	movq	624(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	movsd	112(%rsp), %xmm6
	movq	%rbp, %rsi
	leaq	656(%rsp), %rdi
	leaq	(%rax,%rax,8), %rax
	salq	$2, %rax
	movsd	%xmm6, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	680(%rsp), %rdx
	movq	672(%rsp), %rsi
	movl	$37, %eax
	movq	664(%rsp), %rdi
	movq	656(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	leaq	688(%rsp), %rdi
	movq	%rbp, %rsi
	movsd	368(%rsp), %xmm2
	leaq	(%rax,%rax,8), %rdx
	leaq	(%rax,%rdx,4), %rax
	movsd	%xmm2, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	712(%rsp), %rdx
	movq	704(%rsp), %rdi
	movl	$38, %eax
	movq	696(%rsp), %rcx
	movq	688(%rsp), %rsi
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rcx), %rax
	jnb	.L136
	movsd	384(%rsp), %xmm4
	movq	(%rdi), %rax
	leaq	(%rax,%rax,8), %rdx
	leaq	(%rax,%rdx,2), %rax
	addq	%rax, %rax
	movsd	%xmm4, (%rsi,%rax,8)
	movq	40(%rbx), %rax
	movq	4240(%rbx), %rsi
	leaq	-1(%rax), %rdx
	movq	4320(%rbx), %rax
	movq	%rdx, %rcx
	subq	4304(%rbx), %rcx
	js	.L137
	cmpq	4272(%rbx), %rcx
	jnb	.L138
	movq	4288(%rbx), %rdi
	movl	$39, %ecx
	imulq	%rdx, %rdi
	subq	4312(%rbx), %rcx
	js	.L135
	cmpq	4280(%rbx), %rcx
	jnb	.L136
	movsd	168(%rsp), %xmm1
	movq	4296(%rbx), %rcx
	leaq	(%rcx,%rcx,4), %r8
	leaq	(%rax,%r8,8), %rax
	subq	%rcx, %rax
	addq	%rdi, %rax
	leaq	720(%rsp), %rdi
	movsd	%xmm1, (%rsi,%rax,8)
	movq	%rbp, %rsi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	744(%rsp), %rdx
	movq	736(%rsp), %rsi
	movl	$40, %eax
	movq	728(%rsp), %rdi
	movq	720(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	movq	4240(%rbx), %r9
	movsd	392(%rsp), %xmm0
	movq	4320(%rbx), %rdi
	leaq	(%rax,%rax,4), %rax
	salq	$3, %rax
	movsd	%xmm0, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %r8
	movq	%r8, %rdx
	subq	4304(%rbx), %rdx
	js	.L137
	cmpq	4272(%rbx), %rdx
	jnb	.L138
	movq	4288(%rbx), %rax
	movq	4312(%rbx), %rdx
	imulq	%r8, %rax
	addq	%rax, %rdi
	movl	$41, %eax
	subq	%rdx, %rax
	js	.L135
	movq	4280(%rbx), %rcx
	cmpq	%rcx, %rax
	jnb	.L136
	movq	4296(%rbx), %rsi
	movsd	400(%rsp), %xmm15
	leaq	(%rsi,%rsi,4), %rax
	leaq	(%rsi,%rax,8), %rax
	addq	%rdi, %rax
	movl	$42, %edi
	leaq	(%r9,%rax,8), %rax
	movsd	%xmm15, (%rax)
	subq	%rdx, %rdi
	js	.L135
	cmpq	%rcx, %rdi
	jnb	.L136
	movsd	408(%rsp), %xmm9
	leaq	0(,%rsi,8), %rdi
	movl	$43, %r9d
	addq	%rdi, %rax
	movsd	%xmm9, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	416(%rsp), %xmm10
	addq	%rdi, %rax
	movl	$44, %r9d
	movsd	%xmm10, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	424(%rsp), %xmm14
	addq	%rdi, %rax
	movl	$45, %r9d
	movsd	%xmm14, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	432(%rsp), %xmm8
	addq	%rdi, %rax
	movl	$46, %r9d
	movsd	%xmm8, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	8(%rsp), %xmm11
	addq	%rdi, %rax
	movl	$47, %r9d
	movsd	%xmm11, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	32(%rsp), %xmm12
	addq	%rdi, %rax
	movl	$48, %r9d
	movsd	%xmm12, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	40(%rsp), %xmm13
	addq	%rdi, %rax
	movl	$49, %r9d
	movsd	%xmm13, (%rax)
	subq	%rdx, %r9
	js	.L135
	cmpq	%rcx, %r9
	jnb	.L136
	movsd	440(%rsp), %xmm12
	addq	%rdi, %rax
	movl	$50, %edi
	movsd	%xmm12, (%rax)
	subq	%rdx, %rdi
	js	.L135
	cmpq	%rcx, %rdi
	jnb	.L136
	movsd	448(%rsp), %xmm13
	movq	%r8, %rdx
	leaq	752(%rsp), %rdi
	movsd	%xmm13, (%rax,%rsi,8)
	movq	%rbp, %rsi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	776(%rsp), %rdx
	movq	768(%rsp), %rdi
	movl	$51, %eax
	movq	760(%rsp), %rcx
	movq	752(%rsp), %rsi
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rcx), %rax
	jnb	.L136
	movsd	456(%rsp), %xmm8
	movq	(%rdi), %rax
	leaq	784(%rsp), %rdi
	leaq	(%rax,%rax,2), %rax
	movq	%rax, %rdx
	salq	$4, %rdx
	addq	%rdx, %rax
	movsd	%xmm8, (%rsi,%rax,8)
	movq	40(%rbx), %rax
	movq	%rbp, %rsi
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	808(%rsp), %rdx
	movq	800(%rsp), %rsi
	movl	$52, %eax
	movq	792(%rsp), %rdi
	movq	784(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	movq	4320(%rbx), %r8
	movsd	464(%rsp), %xmm11
	movq	4240(%rbx), %rsi
	leaq	(%rax,%rax,2), %rdx
	leaq	(%rax,%rdx,4), %rax
	salq	$2, %rax
	movsd	%xmm11, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	movq	%rdx, %rax
	subq	4304(%rbx), %rax
	js	.L137
	cmpq	4272(%rbx), %rax
	jnb	.L138
	movq	4288(%rbx), %rdi
	movl	$53, %ecx
	imulq	%rdx, %rdi
	subq	4312(%rbx), %rcx
	js	.L135
	cmpq	4280(%rbx), %rcx
	jnb	.L136
	movq	4296(%rbx), %rcx
	movsd	472(%rsp), %xmm10
	leaq	(%rcx,%rcx,2), %rax
	leaq	(%rcx,%rax,4), %rax
	leaq	(%rcx,%rax,4), %rax
	addq	%r8, %rax
	addq	%rdi, %rax
	leaq	816(%rsp), %rdi
	movsd	%xmm10, (%rsi,%rax,8)
	movq	%rbp, %rsi
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	840(%rsp), %rdx
	movq	832(%rsp), %rdi
	movl	$54, %eax
	movq	824(%rsp), %rcx
	movq	816(%rsp), %rsi
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rcx), %rax
	jnb	.L136
	movq	(%rdi), %rax
	movsd	24(%rsp), %xmm14
	leaq	848(%rsp), %rdi
	leaq	(%rax,%rax,2), %rax
	leaq	(%rax,%rax,8), %rax
	addq	%rax, %rax
	movsd	%xmm14, (%rsi,%rax,8)
	movq	40(%rbx), %rax
	movq	%rbp, %rsi
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	872(%rsp), %rdx
	movq	864(%rsp), %rsi
	movl	$55, %eax
	movq	856(%rsp), %rdi
	movq	848(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	leaq	880(%rsp), %rdi
	movq	%rbp, %rsi
	movsd	136(%rsp), %xmm15
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,2), %rax
	leaq	(%rax,%rax,4), %rax
	movsd	%xmm15, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	904(%rsp), %rdx
	movq	896(%rsp), %rsi
	movl	$56, %eax
	movq	888(%rsp), %rdi
	movq	880(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rdx
	movsd	120(%rsp), %xmm9
	movq	%rbp, %rsi
	leaq	912(%rsp), %rdi
	leaq	0(,%rdx,8), %rax
	subq	%rdx, %rax
	salq	$3, %rax
	movsd	%xmm9, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	936(%rsp), %rdx
	movq	928(%rsp), %rsi
	movl	$57, %eax
	movq	920(%rsp), %rdi
	movq	912(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	movq	(%rsi), %rax
	leaq	944(%rsp), %rdi
	movq	%rbp, %rsi
	movsd	480(%rsp), %xmm4
	leaq	0(,%rax,8), %rdx
	subq	%rax, %rdx
	leaq	(%rax,%rdx,8), %rax
	movsd	%xmm4, (%rcx,%rax,8)
	movq	40(%rbx), %rax
	leaq	-1(%rax), %rdx
	call	_ZN5boost15multi_array_refIdLm2EEixEl
	movq	968(%rsp), %rdx
	movq	960(%rsp), %rsi
	movl	$58, %eax
	movq	952(%rsp), %rdi
	movq	944(%rsp), %rcx
	subq	(%rdx), %rax
	js	.L135
	cmpq	(%rdi), %rax
	jnb	.L136
	imulq	$58, (%rsi), %rax
	movsd	48(%rsp), %xmm5
	movsd	488(%rsp), %xmm1
	movsd	56(%rsp), %xmm6
	movsd	192(%rsp), %xmm3
	movsd	64(%rsp), %xmm2
	movsd	200(%rsp), %xmm7
	movsd	%xmm1, (%rcx,%rax,8)
	movsd	208(%rsp), %xmm4
	movsd	216(%rsp), %xmm1
	movq	3136(%rbx), %rax
	movsd	224(%rsp), %xmm0
	movsd	232(%rsp), %xmm15
	movsd	240(%rsp), %xmm9
	movsd	%xmm3, (%rax)
	movsd	248(%rsp), %xmm10
	movsd	256(%rsp), %xmm14
	movsd	%xmm7, 8(%rax)
	movsd	272(%rsp), %xmm8
	movsd	264(%rsp), %xmm11
	movsd	%xmm5, 16(%rax)
	movsd	%xmm6, 24(%rax)
	movsd	%xmm2, 32(%rax)
	movsd	%xmm4, 40(%rax)
	movsd	%xmm1, 48(%rax)
	movsd	%xmm0, 56(%rax)
	movsd	%xmm15, 64(%rax)
	movsd	%xmm9, 72(%rax)
	movsd	%xmm10, 80(%rax)
	movsd	%xmm14, 88(%rax)
	movsd	%xmm8, 96(%rax)
	movsd	%xmm11, 104(%rax)
	movsd	16(%rsp), %xmm12
	movsd	72(%rsp), %xmm13
	movsd	88(%rsp), %xmm11
	movsd	96(%rsp), %xmm10
	movsd	288(%rsp), %xmm8
	movsd	%xmm12, 112(%rax)
	movsd	104(%rsp), %xmm14
	movsd	80(%rsp), %xmm12
	movsd	%xmm13, 120(%rax)
	movsd	296(%rsp), %xmm15
	movsd	280(%rsp), %xmm13
	movsd	304(%rsp), %xmm9
	movsd	%xmm8, 144(%rax)
	movsd	312(%rsp), %xmm1
	movsd	320(%rsp), %xmm0
	movsd	%xmm12, 128(%rax)
	movsd	328(%rsp), %xmm2
	movsd	184(%rsp), %xmm4
	movsd	%xmm13, 136(%rax)
	movsd	336(%rsp), %xmm5
	movsd	344(%rsp), %xmm6
	movsd	%xmm11, 152(%rax)
	movsd	%xmm10, 160(%rax)
	movsd	%xmm14, 168(%rax)
	movsd	%xmm15, 176(%rax)
	movsd	%xmm9, 184(%rax)
	movsd	%xmm1, 192(%rax)
	movsd	%xmm0, 200(%rax)
	movsd	%xmm2, 208(%rax)
	movsd	%xmm4, 216(%rax)
	movsd	%xmm5, 224(%rax)
	movsd	%xmm6, 232(%rax)
	movsd	352(%rsp), %xmm3
	movsd	8(%rsp), %xmm11
	movsd	176(%rsp), %xmm7
	movsd	112(%rsp), %xmm6
	movsd	384(%rsp), %xmm4
	movsd	168(%rsp), %xmm1
	movsd	%xmm3, 240(%rax)
	movsd	400(%rsp), %xmm15
	movsd	408(%rsp), %xmm9
	movsd	%xmm7, 248(%rax)
	movsd	416(%rsp), %xmm10
	movsd	424(%rsp), %xmm14
	movsd	%xmm4, 296(%rax)
	movsd	432(%rsp), %xmm8
	movsd	360(%rsp), %xmm7
	movsd	%xmm1, 304(%rax)
	movsd	160(%rsp), %xmm3
	movsd	376(%rsp), %xmm5
	movsd	%xmm15, 320(%rax)
	movsd	368(%rsp), %xmm2
	movsd	392(%rsp), %xmm0
	movsd	%xmm9, 328(%rax)
	movsd	%xmm10, 336(%rax)
	movsd	%xmm14, 344(%rax)
	movsd	%xmm8, 352(%rax)
	movsd	%xmm11, 360(%rax)
	movsd	%xmm7, 256(%rax)
	movsd	%xmm3, 264(%rax)
	movsd	%xmm5, 272(%rax)
	movsd	%xmm6, 280(%rax)
	movsd	%xmm2, 288(%rax)
	movsd	%xmm0, 312(%rax)
	movsd	32(%rsp), %xmm12
	movsd	40(%rsp), %xmm13
	movsd	24(%rsp), %xmm14
	movsd	120(%rsp), %xmm9
	movsd	%xmm12, 368(%rax)
	movsd	456(%rsp), %xmm8
	movsd	440(%rsp), %xmm12
	movsd	%xmm13, 376(%rax)
	movsd	464(%rsp), %xmm11
	movsd	448(%rsp), %xmm13
	movsd	472(%rsp), %xmm10
	movsd	136(%rsp), %xmm15
	movsd	%xmm12, 384(%rax)
	movsd	480(%rsp), %xmm4
	movsd	488(%rsp), %xmm1
	movsd	%xmm13, 392(%rax)
	movsd	%xmm8, 400(%rax)
	movsd	%xmm11, 408(%rax)
	movsd	%xmm10, 416(%rax)
	movsd	%xmm14, 424(%rax)
	movsd	%xmm15, 432(%rax)
	movsd	%xmm9, 440(%rax)
	movsd	%xmm4, 448(%rax)
	movsd	%xmm1, 456(%rax)
	addq	$984, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L121:
	.cfi_restore_state
	movsd	328(%rdi), %xmm2
	movsd	%xmm2, 392(%rsp)
	jmp	.L122
	.p2align 4,,10
	.p2align 3
.L141:
	cvttsd2siq	%xmm3, %rax
	leaq	4240(%rbx), %rbp
	movq	%rax, 32(%rbx)
	cmpq	%rdx, 4272(%rbx)
	jnb	.L130
.L142:
	leaq	944(%rsp), %rsi
	movq	%rbp, %rdi
	movq	%rdx, 952(%rsp)
	movq	$0, 944(%rsp)
	movq	$0, 960(%rsp)
	movq	$59, 968(%rsp)
	call	_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE
	movq	40(%rbx), %rax
	leaq	4120(%rbx), %rdi
	leaq	944(%rsp), %rsi
	movq	$0, 944(%rsp)
	movq	$0, 960(%rsp)
	movq	%rax, 952(%rsp)
	movq	$6, 968(%rsp)
	call	_ZN5boost11multi_arrayIdLm2ESaIdEE6resizeERKNS_6detail11multi_array10extent_genILm2EEE
	movq	40(%rbx), %rdx
	jmp	.L130
	.p2align 4,,10
	.p2align 3
.L125:
	movq	%rax, %rcx
	andl	$1, %eax
	pxor	%xmm0, %xmm0
	shrq	%rcx
	orq	%rax, %rcx
	cvtsi2sdq	%rcx, %xmm0
	addsd	%xmm0, %xmm0
	jmp	.L126
.L135:
	movl	$.LC0, %ecx
	movl	$177, %edx
	movl	$.LC1, %esi
	movl	$.LC4, %edi
	call	__assert_fail
.L136:
	call	_ZNK5boost6detail11multi_array18value_accessor_oneIdE6accessIRdPdEET_NS_4typeIS7_EElT0_PKmPKlSE_.isra.0.part.0
.L138:
	movl	$.LC3, %ecx
	movl	$136, %edx
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	__assert_fail
.L137:
	movl	$.LC3, %ecx
	movl	$135, %edx
	movl	$.LC1, %esi
	movl	$.LC4, %edi
	call	__assert_fail
	.cfi_endproc
.LFE5025:
	.size	_Z7FI_RatedP9Variables, .-_Z7FI_RatedP9Variables
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.type	_GLOBAL__sub_I__Z7FI_RatedP9Variables, @function
_GLOBAL__sub_I__Z7FI_RatedP9Variables:
.LFB6422:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	movq	$0, _ZN5boost12_GLOBAL__N_17extentsE(%rip)
	movq	$0, _ZN5boost12_GLOBAL__N_17extentsE+8(%rip)
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE6422:
	.size	_GLOBAL__sub_I__Z7FI_RatedP9Variables, .-_GLOBAL__sub_I__Z7FI_RatedP9Variables
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z7FI_RatedP9Variables
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.local	_ZN5boost12_GLOBAL__N_17extentsE
	.comm	_ZN5boost12_GLOBAL__N_17extentsE,16,16
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC13:
	.long	0
	.long	0
	.align 8
.LC23:
	.long	0
	.long	1072693248
	.align 8
.LC24:
	.long	3435973837
	.long	1075367116
	.align 8
.LC25:
	.long	0
	.long	1074266112
	.align 8
.LC26:
	.long	0
	.long	1093567616
	.align 8
.LC27:
	.long	858993459
	.long	1069757235
	.align 8
.LC28:
	.long	0
	.long	1104006501
	.align 8
.LC29:
	.long	0
	.long	1138753536
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 9.3.1 20200408 (Red Hat 9.3.1-2)"
	.section	.note.GNU-stack,"",@progbits
