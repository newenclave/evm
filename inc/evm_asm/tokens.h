#pragma once 
#ifndef ASM_EVM_TOKENS_H
#define ASM_EVM_TOKENS_H

#include "evm/opcodes.h"

#define ADD_FIELD32(name, value) static const std::uint32_t name = value

namespace evm { namespace assembler {
	namespace tokens {
		
		constexpr std::uint32_t ID(std::uint32_t val)
		{
			return 0xF000 | val;
		}

		struct type: public evm::op {
			ADD_FIELD32(NONE, ID(0));
			ADD_FIELD32(INT_BIN, ID(1));
			ADD_FIELD32(INT_OCT, ID(2));
			ADD_FIELD32(INT_DEC, ID(3));
			ADD_FIELD32(INTEGER, ID(3));
			ADD_FIELD32(INT_HEX, ID(4)); //-V112
			ADD_FIELD32(FLOATING, ID(5)); 
			ADD_FIELD32(COMMA, ID(6));
			ADD_FIELD32(DASH, ID(7));
			ADD_FIELD32(CURRENT, ID(8));
			ADD_FIELD32(IDENT, ID(8));

			ADD_FIELD32(END_OF_FILE, ID(0xFFF));
		};

	}
}}

#endif