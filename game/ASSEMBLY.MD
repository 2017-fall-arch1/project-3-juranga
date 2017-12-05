#ASSEMBLY FOR movePlayerDown method

movPlayerDown: mov r12, r8
	       mov &Vec2, r12
	       mov &Vec2, r14
	       mov #0, r14
	       mov #12, 4(r14)
	       mov 14(r8), r13
	       mov 18(r8), 4(r13)
	       call #vec2Add
	       mov r12, 14(r8)
	       mov 4(r12), 18(r8)
	       mov r8, r12