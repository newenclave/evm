#pragma once
#ifndef ASM_EVM_LEXER_H
#define ASM_EVM_LEXER_H

#include "trie.h"
#include "tokens.h"

namespace evm { namespace assembler {

	class lexer {
	public:

		using const_iterator = std::string::const_iterator;
		struct lexem_info {
			tokens::type type_;
			const_iterator begin_;
			const_iterator end_;
		};

		void reset();
		void add(const std::string str, tokens::type);
		lexem_info current() const;
		lexem_info next() const;
		bool eof() const;
		void advance();
		const_iterator skip_spaces(const_iterator from) const;


	private:
		using trie_type = trie<char, tokens::type>;
		trie_type trie_;
		std::string content_;
		lexem_info current_;
		lexem_info next_;
	};
}}

#endif

