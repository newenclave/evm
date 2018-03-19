#include "lexer.h"

namespace evm { namespace assembler {

	namespace {
		
		bool is_space(char c) 
		{
			switch (c) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
			}
			return false;
		}
	}

	void lexer::reset() 
	{

	}
	
	void lexer::add(const std::string str, tokens::type)
	{
	}

	lexer::lexem_info lexer::current() const
	{
	}

	lexer::lexem_info lexer::next() const
	{
	}

	bool lexer::eof() const
	{
		return current_.type_ == tokens::type::END_OF_FILE;
	}

	lexer::const_iterator lexer::skip_spaces(const_iterator from) const
	{
		while (from != content_.end() && is_space(*from)) {
			++from;
		}
	}

	void lexer::advance()
	{
		auto next_iter = skip_spaces(next_.end_);
		auto next = trie_.get();

	}

}}

