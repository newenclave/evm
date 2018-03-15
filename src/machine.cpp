#include "evm/machine.h"
#include "evm/endian.h"

namespace evm {

	machine::machine(std::size_t stack_size)
		:stack_(stack_size)
	{}

	machine::u64 machine::mem_read_u64()
	{
		std::uint64_t data = 0;
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		data = (data << 8) | memory_[regs_.ip++];
		return data;
	}
	
	machine::f64 machine::mem_read_f64()
	{
		machine::f64 result = 0;
		auto out = reinterpret_cast<std::uint8_t *>(&result);
		auto data = &memory_[regs_.ip];
#ifdef EVM_LITTLE_ENDIAN
		out[0] = data[0];
		out[1] = data[1];
		out[2] = data[2];
		out[3] = data[3];
		out[4] = data[4];
		out[5] = data[5];
		out[6] = data[6];
		out[7] = data[7];
#else 
		out[0] = data[7];
		out[1] = data[6];
		out[2] = data[5];
		out[3] = data[4];
		out[4] = data[3];
		out[5] = data[2];
		out[6] = data[1];
		out[7] = data[0];
#endif
		regs_.ip += sizeof(machine::f64);
		return result;
	}

	void machine::push_u64(machine::u64 value)
	{
		stack_[++regs_.sp].data.u64_value = value;
	}

	void machine::pop(std::size_t count)
	{
		regs_.sp -= count;
	}

	machine::u64 machine::pop_u64()
	{
		return stack_[regs_.sp--].data.u64_value;
	}

	void machine::pop_to(std::vector<machine::stack_value> &output)
	{
		for (auto &next : output) {
			next = std::move(stack_.back());
			stack_.pop_back();
		}
	}

	void machine::push_from(std::vector<machine::stack_value> &output)
	{
		for (auto &value : output) {
			stack_[++regs_.sp] = std::move(value);
		}
	}

	machine::u64 machine::peek_u64(std::size_t pos) 
	{
		return stack_[regs_.sp - pos].data.u64_value;
	}

}
