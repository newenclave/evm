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
		c2: jmp Addr

		10: loda: load value. stack[ap-value]
		11: lodf: load value. stack[fp-value]
		12: lods: load value. stack[sp-value]

		FE: hlt: stops the machine
		FF: reserved
*/

namespace evm {

	class machine;

	using opcode_call = void(*)(machine *);
	inline void nop(machine *) {};

	/*
		Opcodes
	*/
	struct opcodes {
		static void hlt(machine *M);
		static void ipsh(machine *M);
		static void iadd(machine *M);
		static void fadd(machine *M);
		static void isub(machine *M);
		static void imul(machine *M);
		static void idiv(machine *M);
		static void imod(machine *M);

		static void loda(machine *M);
		static void lodf(machine *M);
		static void lods(machine *M);

		static void cal(machine *M);
		static void ret(machine *M);
		static void jmp(machine *M);

		static opcode_call get(machine *M);
		static const char *get_name(std::uint8_t opcode);
	};
}

#endif