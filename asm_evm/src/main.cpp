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
		{ "0"_s, tokens::type::INT_OCT },
		{ "0b"_s, tokens::type::INT_BIN },
		{ "0x"_s, tokens::type::INT_HEX },
	};
	lex.reset("0 0b 0x 0 0 0 ");

	while (!lex.eof()) {
		std::cout << (int)lex.current().type_ << "\n";
		lex.advance();
	}

}