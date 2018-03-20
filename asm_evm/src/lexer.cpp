#include "lexer.h"
#include "numeric.h"

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

		static unsigned base_for(std::uint32_t t)
		{
			switch (t) {
			case tokens::type::INT_BIN:
				return 2;
			case tokens::type::INT_OCT:
				return 8;
			case tokens::type::INT_DEC:
				return 10;
			case tokens::type::INT_HEX:
				return 16;
			default:
				break;
			}
			return 10;
		}

        template <typename ItrT>
        static
        std::string read_string( ItrT &begin, ItrT end, char c )
        {
            std::string res;
            for( ; (begin != end) && (*begin != c); ++begin ) {

                auto next = std::next(begin);

                if( *begin == '\\' && next != end ) {
                    if( c == *next ) {
                        res.push_back( c );
                    } else {
                        switch (*next) {
                        case 'n':
                            res.push_back('\n');
                            break;
                        case 'r':
                            res.push_back('\r');
                            break;
                        case 't':
                            res.push_back('\t');
                            break;
                        case '\\':
                            res.push_back('\\');
                            break;
                        case '"':
                            res.push_back('"');
                            break;
                        case '\'':
                            res.push_back('\'');
                            break;
                        case '0':
                            res.push_back('\0');
                            break;
                        default:
                            res.push_back('\\');
                            res.push_back(*next);
                            break;
                        }
                    }
                    begin = next;
                    if( begin == end ) {
                        break;
                    }
                } else {
                    res.push_back( *begin );
                }
            }

            if( begin != end ) {
                ++begin;
            }

            return res;
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

	lexer::lexem_info lexer::next()
	{
		lexer::lexem_info info;
		auto next_iter = skip_spaces(next_.end_);
		auto next = trie_.get(next_iter, content_.end(), true);
		info.begin_ = next.begin();
		info.end_ = next.end();
		info.value_.s_.assign(next.begin(), next.end());
		info.type_ = tokens::type::END_OF_FILE;
		int valid;
		if (next) {
			info.type_ = *next;
			switch (*next) {
			case tokens::type::INT_BIN:
			case tokens::type::INT_HEX: {
				info.type_ = tokens::type::INT_DEC;
				auto value = numeric::read_number(info.end_, content_.end(), base_for(*next));
				info.begin_ = next_iter;
				info.value_.s_ += value;
				info.value_.numeric_.u64_ = numeric::parse_int(value, base_for(*next), &valid);
			} break;
			default:
				info.value_.s_.assign(info.begin_, info.end_);
				break;
			}
		} else if(next_iter != content_.end()) {
			if (numeric::valid_for_dec(*next_iter) || *next_iter == '.') {
				info.begin_ = info.end_ = next_iter;
				int found = 0;
				auto value = numeric::read_float(info.end_, content_.end(), &found);
				info.value_.s_ += value;
				if (found) {
					info.type_ = tokens::type::FLOATING;
					info.value_.numeric_.f64_ = numeric::parse_float(value);
				} else {
					std::uint32_t base = (*next_iter == '0' ? 8 : 10);
					info.type_ = tokens::type::INTEGER;
					info.value_.numeric_.u64_ = numeric::parse_int(value, base, &valid);
				}
			} 
		}
		return info;
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
		current_ = next_;
		next_ = next();
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

