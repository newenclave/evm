#include "parser.h"

namespace {

	enum precedence {
		NONE = 0,
		SUMM,
		MULTIPLY,
	};

	using namespace evm::assembler;

	using opcode_parser = bool(*)(parser &);

	bool nop(parser &)
	{
		return false;
	}

	bool up0(parser &P)
	{
		auto opcode = static_cast<std::uint8_t>(P.get_lexer().current().type_ & 0xff);
		P.get_fitter().push_code(opcode);
		return true;
	}

	bool up1(parser &P)
	{
		auto &lexer(P.get_lexer());
		auto opcode = static_cast<std::uint8_t>(lexer.current().type_ & 0xff);
		if (P.get_lexer().expect(tokens::type::INTEGER)) {
			P.get_fitter().push_code(opcode);
			P.get_fitter().push_int(lexer.current().value_.numeric_.u64_);
			return true;
		}
		return false;
	}

	bool up2(parser &P)
	{
		auto &lexer(P.get_lexer());
		auto opcode = static_cast<std::uint8_t>(lexer.current().type_ & 0xff);
		std::uint64_t par1 = 0;
		std::uint64_t par2 = 0;

		if (!lexer.expect(tokens::type::INTEGER)) {
			return false;
		}
		par1 = lexer.current().value_.numeric_.u64_;
		lexer.expect(tokens::type::COMMA);

		if (!lexer.expect(tokens::type::INTEGER)) {
			return false;
		}
		par2 = lexer.current().value_.numeric_.u64_;
		P.get_fitter().push_code(opcode);
		P.get_fitter().push_int(par1);
		P.get_fitter().push_int(par2);

		return true;
	}

	bool fp1(parser &P)
	{
		auto opcode = static_cast<std::uint8_t>(P.get_lexer().current().type_ & 0xff);
		if (P.get_lexer().expect(tokens::type::FLOATING)) {
			P.get_fitter().push_code(opcode);
			P.get_fitter().push_int(P.get_lexer().current().value_.numeric_.u64_);
			return true;
		}
		return false;
	}

	opcode_parser get_opcode_call(std::uint8_t code)
	{
		static const opcode_parser allcalls[] = {
			/*      00,  01,  02,  03,  04, 05,  06,  07,   08,  09,  0a,  0b,  0c,  0d,  0e,  0f */
			/*00*/ nop, up1, fp1, up0, nop, up0, up0, up0, up0, up0, up0, up0, up0, up0, nop, nop,
			/*10*/ up1, up1, up1, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*20*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*30*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*40*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*50*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*60*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*70*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*80*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*90*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*a0*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*b0*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*c0*/ up2, up1, up1, up0, up0, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*d0*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*e0*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop,
			/*f0*/ nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, nop, up0, nop,
		};
		return allcalls[code];
	}
}

namespace evm { namespace assembler {
	parser::parser(lexer lex)
		:lexer_(std::move(lex))
	{}

	void parser::reset(lexer lex)
	{
		lexer_ = std::move(lex);
		fitter_.clear();
	}

	lexer &parser::get_lexer()
	{
		return lexer_;
	}

	fitter &parser::get_fitter()
	{
		return fitter_;
	}

	parser::memory_type &parser::get_mem()
	{
		return fitter_.get_mem();
	}

	bool parser::eof() const
	{
		return lexer_.eof();
	}
	
	bool parser::next()
	{
		auto code = lexer_.current().type_;
		if (code <= 0xFF) { 
			if (get_opcode_call(static_cast<std::uint8_t>(code))(*this)) {
				lexer_.advance();
				return true;
			}
		}
		return false;
	}
}}
