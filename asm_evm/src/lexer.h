#pragma once
#ifndef ASM_EVM_LEXER_H
#define ASM_EVM_LEXER_H

#include "trie.h"
#include "tokens.h"

namespace evm { namespace assembler {

	class lexer {
	public:

		using iterator = std::string::iterator;
		struct lexem_info {
			std::uint32_t type_ = tokens::type::NONE;
			iterator begin_;
			iterator end_;
		};

		lexer(std::initializer_list<std::pair<std::string, std::uint32_t> > init);

		void reset(std::string data);
		void add(const std::string &str, std::uint32_t type);
		lexem_info current() const;
		lexem_info next() const;
		bool eof() const;
		void advance();
		bool expect(std::uint32_t type);

	private:

		iterator skip_spaces(iterator from) const;

		using trie_type = trie<char, std::uint32_t>;
		trie_type trie_;
		std::string content_;
		lexem_info current_;
		lexem_info next_;
	};
}}

#endif

