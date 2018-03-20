#include <iostream>
#include "lexer.h"

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
		{ "ipush"_s, tokens::type::IPSH },
		{ "fpush"_s, tokens::type::FPSH },
		{ "iadd"_s, tokens::type::IADD },
		{ ","_s, tokens::type::COMMA },
		{ "-"_s, tokens::type::DASH },
	};
	lex.reset("-0x56ffda, 066");

	while (!lex.eof()) {
		std::cout << (int)lex.current().type_ << "\n";
		std::cout << "'" << lex.current().value_.s_ << "'" << "\n";
		std::cout << lex.current().value_.numeric_.f64_ << "\n";
		std::cout << lex.current().value_.numeric_.u64_ << "\n";
		std::cout << "=====================\n";
		lex.advance();
	}

}