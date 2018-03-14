#include "evm/opcodes.h"
#include "evm/machine.h"
#include <string>

namespace {
	
}

namespace evm {

	void opcodes::hlt(machine *M)
	{
		M->running_ = false;
	}

	void opcodes::ipsh(machine *M)
	{
		M->stack_[++M->regs_.sp].data.u64_value = M->mem_read_u64();
	}

	void opcodes::iadd(machine *M)
	{
		M->stack_[M->regs_.sp - 1].data.u64_value = 
			M->stack_[M->regs_.sp - 1].data.u64_value + 
			M->stack_[M->regs_.sp].data.u64_value;
		--M->regs_.sp;
	}
	
	void opcodes::fadd(machine *M)
	{

	}

	void opcodes::isub(machine *M)
	{
		M->stack_[M->regs_.sp - 1].data.u64_value =
			M->stack_[M->regs_.sp - 1].data.u64_value -
			M->stack_[M->regs_.sp].data.u64_value;
		--M->regs_.sp;
	}

	void opcodes::imul(machine *M)
	{
		M->stack_[M->regs_.sp - 1].data.u64_value =
			M->stack_[M->regs_.sp - 1].data.u64_value *
			M->stack_[M->regs_.sp].data.u64_value;
		--M->regs_.sp;
	}

	void opcodes::idiv(machine *M)
	{
		M->stack_[M->regs_.sp - 1].data.u64_value =
			M->stack_[M->regs_.sp - 1].data.u64_value /
			M->stack_[M->regs_.sp].data.u64_value;
		--M->regs_.sp;
	}

	void opcodes::imod(machine *M)
	{
		M->stack_[M->regs_.sp - 1].data.u64_value =
			M->stack_[M->regs_.sp - 1].data.u64_value /
			M->stack_[M->regs_.sp].data.u64_value;
		--M->regs_.sp;
	}

	void opcodes::loda(machine *M)
	{
		auto pos = M->regs_.ap - static_cast<std::size_t>(M->mem_read_u64());
		M->push_u64(M->stack_[pos].data.u64_value);
	}

	void opcodes::lodf(machine *M)
	{
		auto pos = M->regs_.fp - static_cast<std::size_t>(M->mem_read_u64());
		M->push_u64(M->stack_[pos].data.u64_value);
	}

	void opcodes::lods(machine *M)
	{
		auto pos = M->regs_.sp - static_cast<std::size_t>(M->mem_read_u64());
		M->push_u64(M->stack_[pos].data.u64_value);
	}

	void opcodes::cal(machine *M)
	{
		auto address = M->mem_read_u64();
		auto argc = M->mem_read_u64();
		M->push_u64(argc);
		M->regs_.ap = M->regs_.sp;
		M->push_u64(M->regs_.ap);
		M->push_u64(M->regs_.fp);
		M->push_u64(M->regs_.ip);
		M->regs_.fp = M->regs_.sp;
		M->regs_.ip = static_cast<std::size_t>(address);
	}

	void opcodes::ret(machine *M)
	{
		auto num_args = M->mem_read_u64();
		std::vector<machine::stack_value> popped(static_cast<std::size_t>(num_args));
		M->pop_to(popped);
		M->regs_.sp = M->regs_.fp;
		M->regs_.ip = static_cast<std::size_t>(M->pop_u64());
		M->regs_.fp = static_cast<std::size_t>(M->pop_u64());
		M->regs_.ap = static_cast<std::size_t>(M->pop_u64());
		
		auto old_args = M->pop_u64();
		M->pop(static_cast<std::size_t>(old_args));

		M->push_from(popped);
		M->push_u64(num_args);
	}

	void opcodes::jmp(machine *M)
	{
		M->regs_.ip = static_cast<std::size_t>(M->mem_read_u64());
	}

	opcode_call opcodes::get(machine *M)
	{
		static const opcode_call allcalls[] = {
			/*      00,    01,   02,  03,  04,  05,   06,  07,   08,  09,   0a,   0b,  0c,  0d,  0e,  0f */
			/*00*/  nop, ipsh,  nop, nop, nop, iadd, nop, isub, nop, imul, nop, idiv, nop, imod, nop, nop,
			/*10*/ loda, lodf, lods, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*20*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*30*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*40*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*50*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*60*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*70*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*80*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*90*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*a0*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*b0*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*c0*/  cal,  ret,  jmp, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*d0*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*e0*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop, nop,
			/*f0*/  nop,  nop,  nop, nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, nop,  nop, hlt, nop,
		};																 
		return allcalls[M->memory_[static_cast<std::size_t>(M->regs_.ip++)]];
	}

	const char *opcodes::get_name(std::uint8_t opcode)
	{
		switch (opcode) {
		case 0x01: "ipush";
		case 0x03: "pop";
		default:
			return "unk";
		}
	}

}
