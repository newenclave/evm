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

	lexer::lexer(std::initializer_list<std::pair<std::string, std::uint32_t> > init)
	{
		for (auto &val : init) {
			add(val.first, val.second);
		}
	}

	void lexer::reset(std::string data)
	{
		content_ = std::move(data);
		next_.end_ = content_.begin();
		advance();
		advance();
	}
	
	void lexer::add(const std::string &str, std::uint32_t type)
	{
		trie_.set(str.begin(), str.end(), type);
	}

	lexer::lexem_info lexer::current() const
	{
		return current_;
	}

	lexer::lexem_info lexer::next() const
	{
		return next_;
	}

	bool lexer::eof() const
	{
		return current_.type_ == tokens::type::END_OF_FILE;
	}

	lexer::iterator lexer::skip_spaces(iterator from) const
	{
		while (from != content_.end() && is_space(*from)) {
			++from;
		}
		return from;
	}

	void lexer::advance()
	{
		auto next_iter = skip_spaces(next_.end_);
		auto next = trie_.get(next_iter, content_.end(), true);
		current_ = next_;
		if (next) {
			next_.begin_ = next.begin();
			next_.end_ = next.end();
			next_.type_ = *next;
		} else {
			next_.begin_ = next_.end_ = content_.end();
			next_.type_ = tokens::type::END_OF_FILE;
		}
	}

	bool lexer::expect(std::uint32_t type)
	{
		if (next_.type_ == type) {
			advance();
			return true;
		}
		return false;
	}

}}

