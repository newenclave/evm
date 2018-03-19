#pragma once 
#ifndef ASM_EVM_TOKENS_H
#define ASM_EVM_TOKENS_H

namespace evm { namespace assembler {
	namespace tokens {

		enum class type {
			NONE,
			INT_BIN,
			INT_OCT,
			INT_DEC,
			INT_HEX,

			END_OF_FILE,
		};

	}
}}

#endif