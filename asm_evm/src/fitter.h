#pragma once 
#ifndef EVM_ASM_ASSEMBLER_H
#define EVM_ASM_ASSEMBLER_H
#include <cstdint>
#include <vector>

namespace evm { namespace assembler {
	class fitter {
	public:
		using memory_type = std::vector<std::uint8_t>;
		memory_type &get_mem();
		void clear();
		void push_code(std::uint8_t code);
		void push_int(std::uint64_t value);
		void push_float(double value);
		void fill_until(std::size_t target_size);
	private:
		memory_type mem_;
	};
}}

#endif