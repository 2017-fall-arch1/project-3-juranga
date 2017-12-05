## Assembly for movePlayerDown method

mov r12, r8 \n
mov &Vec2, r12 \n
mov &Vec2, r14 \n
mov #0, r14 \n
mov #12, 4(r14) \n
mov 14(r8), r13 \n
mov 18(r8), 4(r13) \n
call #vec2Add \n
mov r12, 14(r8) \n
mov 4(r12), 18(r8) \n
mov r8, r12