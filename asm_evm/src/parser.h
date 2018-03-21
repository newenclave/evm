#pragma once 

#include <vector>
#include "lexer.h"

namespace evm { namespace assembler {
	class parser {
	public:
		parser(lexer lex);
		void reset();
		lexer &get_lexer();

	private:
		lexer lexer_;
		std::vector<std::uint8_t> mem_;
	};
}}
