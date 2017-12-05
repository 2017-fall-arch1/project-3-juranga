## Assembly for movePlayerDown method

mov r12, r8 <br />
mov &Vec2, r12 <br />
mov &Vec2, r14 <br />
mov #0, r14 <br />
mov #12, 4(r14) <br />
mov 14(r8), r13 <br />
mov 18(r8), 4(r13) <br />
call #vec2Add <br />
mov r12, 14(r8) <br />
mov 4(r12), 18(r8) <br />
mov r8, r12