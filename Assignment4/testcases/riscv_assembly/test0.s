	.text
	.file	"llvm-link"
	.globl	main                    # -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	sd	ra, 24(sp)
	.cfi_offset ra, -8
	sb	zero, 23(sp)
	addi	a0, zero, 33
	sb	a0, 22(sp)
	addi	a0, zero, 100
	sb	a0, 21(sp)
	addi	a0, zero, 108
	sb	a0, 20(sp)
	addi	a1, zero, 114
	sb	a1, 19(sp)
	addi	a1, zero, 111
	sb	a1, 18(sp)
	addi	a2, zero, 119
	sb	a2, 17(sp)
	addi	a2, zero, 32
	sb	a2, 16(sp)
	sb	a1, 15(sp)
	sb	a0, 14(sp)
	sb	a0, 13(sp)
	addi	a0, zero, 101
	sb	a0, 12(sp)
	addi	a0, zero, 104
	sb	a0, 11(sp)
	addi	a0, sp, 11
	call	print_string
	mv	a0, zero
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.globl	oat_malloc              # -- Begin function oat_malloc
	.p2align	1
	.type	oat_malloc,@function
oat_malloc:                             # @oat_malloc
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a0, -20(s0)
	addi	a1, zero, 1
	call	calloc
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end1:
	.size	oat_malloc, .Lfunc_end1-oat_malloc
                                        # -- End function
	.globl	oat_alloc_array         # -- Begin function oat_alloc_array
	.p2align	1
	.type	oat_alloc_array,@function
oat_alloc_array:                        # @oat_alloc_array
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a0, -20(s0)
	bltz	a0, .LBB2_2
	j	.LBB2_1
.LBB2_1:
	j	.LBB2_3
.LBB2_2:
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.oat_alloc_array)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.oat_alloc_array)
	addi	a2, zero, 24
	call	__assert_fail
.LBB2_3:
	lw	a0, -20(s0)
	addiw	a0, a0, 1
	slli	a0, a0, 2
	call	malloc
	sd	a0, -32(s0)
	lw	a0, -20(s0)
	ld	a1, -32(s0)
	sw	a0, 0(a1)
	ld	a0, -32(s0)
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end2:
	.size	oat_alloc_array, .Lfunc_end2-oat_alloc_array
                                        # -- End function
	.globl	array_of_string         # -- Begin function array_of_string
	.p2align	1
	.type	array_of_string,@function
array_of_string:                        # @array_of_string
# %bb.0:
	addi	sp, sp, -48
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	addi	s0, sp, 48
	sd	a0, -24(s0)
	ld	a0, -24(s0)
	beqz	a0, .LBB3_2
	j	.LBB3_1
.LBB3_1:
	j	.LBB3_3
.LBB3_2:
	lui	a0, %hi(.L.str.2)
	addi	a0, a0, %lo(.L.str.2)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.array_of_string)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.array_of_string)
	addi	a2, zero, 35
	call	__assert_fail
.LBB3_3:
	ld	a0, -24(s0)
	call	strlen
	sw	a0, -28(s0)
	lw	a0, -28(s0)
	bltz	a0, .LBB3_5
	j	.LBB3_4
.LBB3_4:
	j	.LBB3_6
.LBB3_5:
	lui	a0, %hi(.L.str.3)
	addi	a0, a0, %lo(.L.str.3)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.array_of_string)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.array_of_string)
	addi	a2, zero, 38
	call	__assert_fail
.LBB3_6:
	lw	a0, -28(s0)
	addiw	a0, a0, 1
	slli	a0, a0, 2
	call	malloc
	sd	a0, -40(s0)
	lw	a0, -28(s0)
	ld	a1, -40(s0)
	sw	a0, 0(a1)
	sw	zero, -32(s0)
	j	.LBB3_7
.LBB3_7:                                # =>This Inner Loop Header: Depth=1
	lw	a0, -32(s0)
	lw	a1, -28(s0)
	bge	a0, a1, .LBB3_10
	j	.LBB3_8
.LBB3_8:                                #   in Loop: Header=BB3_7 Depth=1
	ld	a0, -24(s0)
	lw	a1, -32(s0)
	add	a0, a0, a1
	lbu	a0, 0(a0)
	ld	a2, -40(s0)
	addiw	a1, a1, 1
	slli	a1, a1, 2
	add	a1, a2, a1
	sw	a0, 0(a1)
	j	.LBB3_9
.LBB3_9:                                #   in Loop: Header=BB3_7 Depth=1
	lw	a0, -32(s0)
	addi	a0, a0, 1
	sw	a0, -32(s0)
	j	.LBB3_7
.LBB3_10:
	ld	a0, -40(s0)
	ld	s0, 32(sp)
	ld	ra, 40(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end3:
	.size	array_of_string, .Lfunc_end3-array_of_string
                                        # -- End function
	.globl	string_of_array         # -- Begin function string_of_array
	.p2align	1
	.type	string_of_array,@function
string_of_array:                        # @string_of_array
# %bb.0:
	addi	sp, sp, -48
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	addi	s0, sp, 48
	sd	a0, -24(s0)
	ld	a0, -24(s0)
	beqz	a0, .LBB4_2
	j	.LBB4_1
.LBB4_1:
	j	.LBB4_3
.LBB4_2:
	lui	a0, %hi(.L.str.4)
	addi	a0, a0, %lo(.L.str.4)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a2, zero, 53
	call	__assert_fail
.LBB4_3:
	ld	a0, -24(s0)
	lw	a0, 0(a0)
	sw	a0, -28(s0)
	lw	a0, -28(s0)
	bltz	a0, .LBB4_5
	j	.LBB4_4
.LBB4_4:
	j	.LBB4_6
.LBB4_5:
	lui	a0, %hi(.L.str.3)
	addi	a0, a0, %lo(.L.str.3)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a2, zero, 56
	call	__assert_fail
.LBB4_6:
	lw	a0, -28(s0)
	addiw	a0, a0, 1
	call	malloc
	sd	a0, -40(s0)
	sw	zero, -32(s0)
	j	.LBB4_7
.LBB4_7:                                # =>This Inner Loop Header: Depth=1
	lw	a0, -32(s0)
	lw	a1, -28(s0)
	bge	a0, a1, .LBB4_13
	j	.LBB4_8
.LBB4_8:                                #   in Loop: Header=BB4_7 Depth=1
	ld	a0, -24(s0)
	lw	a1, -32(s0)
	addiw	a2, a1, 1
	slli	a2, a2, 2
	add	a0, a0, a2
	lb	a0, 0(a0)
	ld	a2, -40(s0)
	add	a1, a2, a1
	sb	a0, 0(a1)
	ld	a0, -40(s0)
	lw	a1, -32(s0)
	add	a0, a0, a1
	lbu	a0, 0(a0)
	beqz	a0, .LBB4_10
	j	.LBB4_9
.LBB4_9:                                #   in Loop: Header=BB4_7 Depth=1
	j	.LBB4_11
.LBB4_10:
	lui	a0, %hi(.L.str.5)
	addi	a0, a0, %lo(.L.str.5)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.string_of_array)
	addi	a2, zero, 62
	call	__assert_fail
.LBB4_11:                               #   in Loop: Header=BB4_7 Depth=1
	j	.LBB4_12
.LBB4_12:                               #   in Loop: Header=BB4_7 Depth=1
	lw	a0, -32(s0)
	addi	a0, a0, 1
	sw	a0, -32(s0)
	j	.LBB4_7
.LBB4_13:
	ld	a0, -40(s0)
	lw	a1, -28(s0)
	add	a0, a0, a1
	sb	zero, 0(a0)
	ld	a0, -40(s0)
	ld	s0, 32(sp)
	ld	ra, 40(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end4:
	.size	string_of_array, .Lfunc_end4-string_of_array
                                        # -- End function
	.globl	length_of_string        # -- Begin function length_of_string
	.p2align	1
	.type	length_of_string,@function
length_of_string:                       # @length_of_string
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sd	a0, -24(s0)
	ld	a0, -24(s0)
	beqz	a0, .LBB5_2
	j	.LBB5_1
.LBB5_1:
	j	.LBB5_3
.LBB5_2:
	lui	a0, %hi(.L.str.2)
	addi	a0, a0, %lo(.L.str.2)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.length_of_string)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.length_of_string)
	addi	a2, zero, 70
	call	__assert_fail
.LBB5_3:
	ld	a0, -24(s0)
	call	strlen
	sext.w	a0, a0
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end5:
	.size	length_of_string, .Lfunc_end5-length_of_string
                                        # -- End function
	.globl	string_of_int           # -- Begin function string_of_int
	.p2align	1
	.type	string_of_int,@function
string_of_int:                          # @string_of_int
# %bb.0:
	addi	sp, sp, -48
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	sd	s1, 24(sp)
	sd	s2, 16(sp)
	addi	s0, sp, 48
	sw	a0, -36(s0)
	lw	a2, -36(s0)
	lui	a0, %hi(string_of_int.buf)
	addi	s1, a0, %lo(string_of_int.buf)
	lui	a0, %hi(.L.str.6)
	addi	a1, a0, %lo(.L.str.6)
	mv	a0, s1
	call	sprintf
	lui	s2, %hi(string_of_int.len)
	sw	a0, %lo(string_of_int.len)(s2)
	lw	a0, %lo(string_of_int.len)(s2)
	addiw	a0, a0, 1
	call	malloc
	sd	a0, -48(s0)
	ld	a0, -48(s0)
	lw	a2, %lo(string_of_int.len)(s2)
	mv	a1, s1
	call	memcpy
	ld	a0, -48(s0)
	lw	a1, %lo(string_of_int.len)(s2)
	add	a0, a0, a1
	sb	zero, 0(a0)
	ld	a0, -48(s0)
	ld	s2, 16(sp)
	ld	s1, 24(sp)
	ld	s0, 32(sp)
	ld	ra, 40(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end6:
	.size	string_of_int, .Lfunc_end6-string_of_int
                                        # -- End function
	.globl	string_cat              # -- Begin function string_cat
	.p2align	1
	.type	string_cat,@function
string_cat:                             # @string_cat
# %bb.0:
	addi	sp, sp, -64
	sd	ra, 56(sp)
	sd	s0, 48(sp)
	addi	s0, sp, 64
	sd	a0, -24(s0)
	sd	a1, -32(s0)
	ld	a0, -24(s0)
	call	strlen
	sd	a0, -40(s0)
	ld	a0, -32(s0)
	call	strlen
	sd	a0, -48(s0)
	ld	a0, -40(s0)
	ld	a1, -48(s0)
	add	a0, a0, a1
	addi	a0, a0, 1
	call	malloc
	sd	a0, -56(s0)
	ld	a0, -56(s0)
	ld	a1, -24(s0)
	ld	a2, -40(s0)
	call	memcpy
	ld	a0, -56(s0)
	ld	a1, -40(s0)
	add	a0, a0, a1
	ld	a1, -32(s0)
	ld	a2, -48(s0)
	call	memcpy
	ld	a0, -56(s0)
	ld	a1, -40(s0)
	ld	a2, -48(s0)
	add	a1, a1, a2
	add	a0, a0, a1
	sb	zero, 0(a0)
	ld	a0, -56(s0)
	ld	s0, 48(sp)
	ld	ra, 56(sp)
	addi	sp, sp, 64
	ret
.Lfunc_end7:
	.size	string_cat, .Lfunc_end7-string_cat
                                        # -- End function
	.globl	print_string            # -- Begin function print_string
	.p2align	1
	.type	print_string,@function
print_string:                           # @print_string
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sd	a0, -24(s0)
	ld	a0, -24(s0)
	beqz	a0, .LBB8_2
	j	.LBB8_1
.LBB8_1:
	j	.LBB8_3
.LBB8_2:
	lui	a0, %hi(.L.str.2)
	addi	a0, a0, %lo(.L.str.2)
	lui	a1, %hi(.L.str.1)
	addi	a1, a1, %lo(.L.str.1)
	lui	a2, %hi(.L__PRETTY_FUNCTION__.print_string)
	addi	a3, a2, %lo(.L__PRETTY_FUNCTION__.print_string)
	addi	a2, zero, 95
	call	__assert_fail
.LBB8_3:
	ld	a1, -24(s0)
	lui	a0, %hi(.L.str.7)
	addi	a0, a0, %lo(.L.str.7)
	call	printf
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end8:
	.size	print_string, .Lfunc_end8-print_string
                                        # -- End function
	.globl	print_int               # -- Begin function print_int
	.p2align	1
	.type	print_int,@function
print_int:                              # @print_int
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a1, -20(s0)
	lui	a0, %hi(.L.str.6)
	addi	a0, a0, %lo(.L.str.6)
	call	printf
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end9:
	.size	print_int, .Lfunc_end9-print_int
                                        # -- End function
	.globl	print_bool              # -- Begin function print_bool
	.p2align	1
	.type	print_bool,@function
print_bool:                             # @print_bool
# %bb.0:
	addi	sp, sp, -32
	sd	ra, 24(sp)
	sd	s0, 16(sp)
	addi	s0, sp, 32
	sw	a0, -20(s0)
	lw	a0, -20(s0)
	bnez	a0, .LBB10_2
	j	.LBB10_1
.LBB10_1:
	lui	a0, %hi(.L.str.8)
	addi	a0, a0, %lo(.L.str.8)
	call	printf
	j	.LBB10_3
.LBB10_2:
	lui	a0, %hi(.L.str.9)
	addi	a0, a0, %lo(.L.str.9)
	call	printf
	j	.LBB10_3
.LBB10_3:
	ld	s0, 16(sp)
	ld	ra, 24(sp)
	addi	sp, sp, 32
	ret
.Lfunc_end10:
	.size	print_bool, .Lfunc_end10-print_bool
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"size >= 0"
	.size	.L.str, 10

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"runtime.c"
	.size	.L.str.1, 10

	.type	.L__PRETTY_FUNCTION__.oat_alloc_array,@object # @__PRETTY_FUNCTION__.oat_alloc_array
.L__PRETTY_FUNCTION__.oat_alloc_array:
	.asciz	"int32_t *oat_alloc_array(int32_t)"
	.size	.L__PRETTY_FUNCTION__.oat_alloc_array, 34

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"NULL != str"
	.size	.L.str.2, 12

	.type	.L__PRETTY_FUNCTION__.array_of_string,@object # @__PRETTY_FUNCTION__.array_of_string
.L__PRETTY_FUNCTION__.array_of_string:
	.asciz	"int32_t *array_of_string(char *)"
	.size	.L__PRETTY_FUNCTION__.array_of_string, 33

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"len >= 0"
	.size	.L.str.3, 9

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"NULL != arr"
	.size	.L.str.4, 12

	.type	.L__PRETTY_FUNCTION__.string_of_array,@object # @__PRETTY_FUNCTION__.string_of_array
.L__PRETTY_FUNCTION__.string_of_array:
	.asciz	"char *string_of_array(int32_t *)"
	.size	.L__PRETTY_FUNCTION__.string_of_array, 33

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"0 != str[i]"
	.size	.L.str.5, 12

	.type	.L__PRETTY_FUNCTION__.length_of_string,@object # @__PRETTY_FUNCTION__.length_of_string
.L__PRETTY_FUNCTION__.length_of_string:
	.asciz	"int32_t length_of_string(char *)"
	.size	.L__PRETTY_FUNCTION__.length_of_string, 33

	.type	string_of_int.buf,@object # @string_of_int.buf
	.local	string_of_int.buf
	.comm	string_of_int.buf,128,1
	.type	.L.str.6,@object        # @.str.6
.L.str.6:
	.asciz	"%ld"
	.size	.L.str.6, 4

	.type	string_of_int.len,@object # @string_of_int.len
	.section	.sbss,"aw",@nobits
	.p2align	2
string_of_int.len:
	.word	0                       # 0x0
	.size	string_of_int.len, 4

	.type	.L__PRETTY_FUNCTION__.print_string,@object # @__PRETTY_FUNCTION__.print_string
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__PRETTY_FUNCTION__.print_string:
	.asciz	"void print_string(char *)"
	.size	.L__PRETTY_FUNCTION__.print_string, 26

	.type	.L.str.7,@object        # @.str.7
.L.str.7:
	.asciz	"%s"
	.size	.L.str.7, 3

	.type	.L.str.8,@object        # @.str.8
.L.str.8:
	.asciz	"false"
	.size	.L.str.8, 6

	.type	.L.str.9,@object        # @.str.9
.L.str.9:
	.asciz	"true"
	.size	.L.str.9, 5

	.ident	"clang version 10.0.0-4ubuntu1~18.04.2 "
	.section	".note.GNU-stack","",@progbits
