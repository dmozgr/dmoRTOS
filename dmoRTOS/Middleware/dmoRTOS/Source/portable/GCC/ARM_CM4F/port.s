			AREA |.text|,CODE,READONLY,ALIGN=2
			THUMB
			EXTERN currentPt

Systick_Handler		;save r0
	CPSID	I
	PUSH	{R4-R11}
	LDR		R0,=currentPt
	LDR		R1,[R0]

	STR		SP,[R1]
	LDR		R1,[R1,#4]
	STR		R1,[R0]

	LDR		SP,[R1]

	POP		{R4-R11}
	CPSIE	I
	BX		lr
