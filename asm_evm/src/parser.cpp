#include "parser.h"

namespace evm { namespace assembler {
	parser::parser(lexer lex)
		:lexer_(std::move(lex))
	{}

	void parser::reset()
	{
		lexer_.reset();
	}

	lexer &parser::get_lexer()
	{
		return lexer_;
	}

}}