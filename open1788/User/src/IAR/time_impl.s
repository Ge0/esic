	MODULE time_impl
	PUBLIC time_impl
	SECTION .text:CODE:REORDER(1)

time_impl:
	PUSH	{LR}
	MOVS	R2, R0
	MOVS	R1, #0
	MOVS	R0, #17
	CMP	R2, #0
	BEQ.N	epilogue
	STR	R0, [R2]
epilogue:
	POP	{PC}
	END
