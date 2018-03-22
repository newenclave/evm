#include <iostream>
#include "lexer.h"
#include "parser.h"

using namespace evm::assembler;
namespace {

	std::string operator ""_s(const char *data, std::size_t len)
	{
		return std::string(data, len);
	}
}

int main()
{
	evm::assembler::lexer lex{ 
		{ "0b"_s, tokens::type::INT_BIN },
		{ "0x"_s, tokens::type::INT_HEX },
		{ "hlt"_s, tokens::type::HLT },
		{ "ipush"_s, tokens::type::IPSH },
		{ "fpush"_s, tokens::type::FPSH },
		{ "iadd"_s, tokens::type::IADD },
		{ "jmp"_s, tokens::type::JMP },
		{ "$$"_s, tokens::type::CURRENT },
		{ ","_s, tokens::type::COMMA },
		{ "-"_s, tokens::type::DASH },
	};
	lex.reset(R"(
	ipush 10 
	ipush 20
	iadd
	)");
	
	parser P(std::move(lex));
	while (!P.eof()) {
		if (!P.next()) {
			break;
		}
	}

}