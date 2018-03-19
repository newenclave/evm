#pragma once
#ifndef EVM_MACHINE_H
#define EVM_MACHINE_H

#include <cstdint>
#include <vector>
#include "opcodes.h"

namespace evm {

	struct opcodes;

	class machine {
		friend struct opcodes;
	public:

		enum data_type : uint8_t {
			NONE,
			UINTEGER,
			FLOATING,
			REGISTER,
		};

		using u8 = std::uint8_t;
		using i64 = std::int64_t;
		using u64 = std::uint64_t;
		using f64 = double;

		struct stack_value {
			union {
				u64 u64_value;
				i64 i64_value;
				f64 f64_value;
			} data;
			data_type type = NONE;
		};

		struct registers {
			std::size_t sp = 0;
			std::size_t fp = 0;
			std::size_t ap = 0;
			std::size_t ip = 0;

			u64 flags = 0;
		};

		machine(std::size_t stack_size);

		machine(machine &&) = default;
		machine & operator = (machine &&) = default;
		machine(const machine &) = default;
		machine & operator = (const machine &) = default;

		std::vector<u8> &memory()
		{
			return memory_;
		}

		const std::vector<u8> &memory() const
		{
			return memory_;
		}

		void reset()
		{
			regs_.ip = 0;
			regs_.ap = 0;
			regs_.fp = 0;
			regs_.sp = 0;

			regs_.flags = 0;
		}

		bool running() const
		{
			return running_;
		}

		const registers &get_registers() const
		{
			return regs_;
		}

		const std::vector<stack_value> &get_stack() const
		{
			return stack_;
		}

		opcode_call fetch()
		{
			return opcodes::get(this);
		}

		u64 mem_read_u64();
		f64 mem_read_f64();

		void push_u64(u64 value, data_type type = UINTEGER);
		void pop(std::size_t count = 1);
		u64 pop_u64();
		void pop_to(std::vector<stack_value> &output);
		void push_from(std::vector<stack_value> &output);

		u64 peek_u64(std::size_t pos = 0);

	public:
		bool running_ = true;
		registers regs_;
		std::vector<u8> memory_;
		std::vector<stack_value> stack_;
	};
}

#endif