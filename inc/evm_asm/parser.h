#pragma once 

#include <vector>
#include "lexer.h"
#include "fitter.h"

namespace evm { namespace assembler {
	class parser {
	public:

		using memory_type = std::vector<std::uint8_t>;

		parser(lexer lex);
		void reset(lexer lex);
		lexer &get_lexer();
		fitter &get_fitter();
		memory_type &get_mem();

		bool eof() const;
		bool next();

	private:
		lexer lexer_;
		fitter fitter_;
	};
}}
