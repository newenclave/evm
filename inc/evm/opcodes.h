#pragma once
#ifndef EVM_OPCODES_H
#define EVM_OPCODES_H

#include <cstdint>

/*
	OPCODE TABLE:
		00: reserved
		01: ipush: push an integer value
		02: fpush: push a float value
		03: pop: pop the stack
		05: iadd: stack[-1] + stack[-2]
		06: fadd: floating point stack[-1] + stack[-2]
		07: isub: stack[-1] - stack[-2]
		08: fsub: floating point stack[-1] - stack[-2]

		09: imul: stack[-1] * stack[-2]
		0a: fmul: floating point stack[-1] * stack[-2]

		0b: idiv: stack[-1] / stack[-2]
		0c: fdiv: floating point stack[-1] / stack[-2]

		0d: imod: stack[-1] % stack[-2]

		c0: call addr, Num:
		c1: ret Num:
		c2: jmp Addr: set ip to address
		c3: jz Addr: set ip to address if the top of the stack is zero
		c4: jnz Addr: set ip to address if the top of the stack is not zero

		10: loda: load value. stack[ap-value]
		11: lodf: load value. stack[fp-value]
		12: lods: load value. stack[sp-value]

		FE: hlt: stops the machine
		FF: reserved
*/

#define ADD_FIELD8(name, value) static const std::uint8_t name = value

namespace evm {

	struct op {
		ADD_FIELD8(IPSH, 0x01);
		ADD_FIELD8(FPSH, 0x02);
		ADD_FIELD8(POP,  0x03);
		ADD_FIELD8(IADD, 0x05);
		ADD_FIELD8(FADD, 0x06);
		ADD_FIELD8(ISUB, 0x07);
		ADD_FIELD8(FSUB, 0x08);
		ADD_FIELD8(IMUL, 0x09);
		ADD_FIELD8(FMUL, 0x0A);
		ADD_FIELD8(IDIV, 0x0B);
		ADD_FIELD8(FDIV, 0x0C);
		ADD_FIELD8(IMOV, 0x0D);
		ADD_FIELD8(CALL, 0xC0);
		ADD_FIELD8(RET,  0xC1);
		ADD_FIELD8(JMP,  0xC2);
		ADD_FIELD8(JZ,   0xC3);
		ADD_FIELD8(JNZ,  0xC4);
		ADD_FIELD8(LODA, 0x10);
		ADD_FIELD8(LODF, 0x11);
		ADD_FIELD8(LODS, 0x12);
		ADD_FIELD8(HLT,  0xFE);
	};

	class machine;

	using opcode_call = void(*)(machine *);
	inline void nop(machine *) {};

	/*
		Opcodes
	*/
	struct opcodes {
		static void hlt(machine *M);
		static void ipsh(machine *M);
		static void fpsh(machine *M);
		static void pop(machine *M);

		static void iadd(machine *M);
		static void isub(machine *M);
		static void imul(machine *M);
		static void idiv(machine *M);
		static void imod(machine *M);

		static void fadd(machine *M);
		static void fsub(machine *M);
		static void fmul(machine *M);
		static void fdiv(machine *M);

		static void loda(machine *M);
		static void lodf(machine *M);
		static void lods(machine *M);

		static void cal(machine *M);
		static void ret(machine *M);
		static void jmp(machine *M);
		static void jz(machine *M);
		static void jnz(machine *M);

		static opcode_call get(machine *M);
		static const char *get_name(std::uint8_t opcode);
	};
}

#endif