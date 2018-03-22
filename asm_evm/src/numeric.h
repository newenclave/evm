#pragma once 
#ifndef ASM_EVM_NUMERIC_H
#define ASM_EVM_NUMERIC_H

#include "evm_asm/tokens.h"
#include <cstdint>

namespace evm { namespace assembler {

    template <char GapChar>
    struct numeric_templ {

        static const char gap_character = GapChar;

        static
        std::uint8_t char2int( char cc )
        {
            std::uint8_t c = static_cast<std::uint8_t>(cc);
            switch (c) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return c - '0';
            case 'a': case 'b': case 'c':
            case 'd': case 'e': case 'f':
                return c - 'a' + 0xa;
            case 'A': case 'B': case 'C':
            case 'D': case 'E': case 'F':
                return c - 'A' + 0xA;
            }
            return 0xFF;
        }

        template <typename CharT>
        static
        bool is_gap( CharT c )
        {
            return c == gap_character;
        }

        template <typename CharT>
        static
        bool valid_for_bin( CharT c )
        {
            return ('0' == c) || (c == '1');
        }

        template <typename CharT>
        static
        bool valid_for_oct( CharT c )
        {
            return ('0' <= c) && (c <= '7' );
        }

        template <typename CharT>
        static
        bool valid_for_dec( CharT c )
        {
            return ('0' <= c) && (c <= '9' );
        }

        template <typename CharT>
        static
        bool valid_for_dec_( CharT c )
        {
            return valid_for_dec( c ) || is_gap( c );
        }

        template <typename CharT>
        static
        bool valid_for_hex( CharT c )
        {
            return (('0' <= c) && (c <= '9' ))
                || (('a' <= c) && (c <= 'f' ))
                || (('A' <= c) && (c <= 'F' ))
                 ;
        }

        template <typename CharT>
        static
        bool valid_for_hex_( CharT c )
        {
            return valid_for_hex( c ) || is_gap( c );
        }

        static
        bool valid_for(std::uint32_t t, char c )
        {
            switch( t ) {
            case 2: return valid_for_bin( c );
            case 8: return valid_for_oct( c );
            case 10: return valid_for_dec( c );
            case 16: return valid_for_hex( c );
            default:
                break;
            }
            return false;
        }

        static
        bool valid_for_(std::uint32_t base, char c )
        {
            return valid_for(base, c) || is_gap( c );
        }

        static
        std::uint64_t parse_int( const std::string &input, std::uint32_t base,
                                 int *first_inval )
        {
            std::uint64_t res = 0;
            *first_inval = -1;

            int pos = 0;

            for( auto c: input ) {
                if( !is_gap(c) ) {
                    if( valid_for(base, c) ) {
                        res *= base;
                        res += char2int( c );
                        ++pos;
                    } else {
                        *first_inval = pos;
                        return 0;
                    }
                }
            }
            return res;
        }

        template <typename ItrT>
        static
        bool check_if_float( ItrT begin, ItrT end )
        {
            bool float_found = false;
            while( begin != end && !float_found ) {
                char c = *begin++;
                if( (c == 'e') || (c == 'E') ) {
                    float_found = true;
                } else if( c == '.' ) {
                    if( (begin != end) && (*begin == '.') ) {
                        return false;
                    } else {
                        float_found = true;
                    }
                } else if( !valid_for_dec_( c ) ) {
                    return false;
                }
            }
            return float_found;
        }

        template <typename ContT>
        static
        double parse_float( const ContT &cont )
        {
            auto bb = std::begin(cont);
            return parse_float( bb, std::end(cont) );
        }

		template <typename ItrT>
		static
		std::string read_number(ItrT &begin, ItrT end, std::uint32_t tt)
		{
			std::string res;

			for (; begin != end; ++begin) {
				if(valid_for_(tt, *begin)) {
					res.push_back(*begin);
				} else {
					break;
				}
			}

			return res;
		}

		template <typename ItrT>
		static
		std::string read_float(ItrT &begin, ItrT end, int *found)
		{
			std::string res;
			*found = 0;

			for (; begin != end; ++begin) {
				if (valid_for_dec(*begin)) {
					res.push_back(*begin);
				} else if (!numeric::is_gap(*begin)) {
					break;
				}
			}

			if (begin != end && *begin == '.') {
				res.push_back(*begin++);
				*found = 1;
				for (; begin != end; ++begin) {
					if (valid_for_dec(*begin)) {
						res.push_back(*begin);
					} else if (!is_gap(*begin)) {
						break;
					}
				}
			}

			if (begin != end && (*begin == 'e' || *begin == 'E')) {
				res.push_back(*begin++);
				*found = 1;
				if (begin != end && (*begin == '+' || *begin == '-')) {
					res.push_back(*begin++);
				}
				for (; begin != end; ++begin) {
					if (valid_for_dec(*begin)) {
						res.push_back(*begin);
					} else if (!is_gap(*begin)) {
						break;
					}
				}
			}
			return res;
		}

        template <typename ItrT>
        static
        double parse_float( ItrT &s, ItrT end )
        {
            double a = 0.0;
            int    e = 0;
            int    c = 0;

            while( (s != end) && valid_for_dec_(c = *s++) ) {
                if( !is_gap(c) ) {
                    a = a * 10.0 + (c - '0');
                }
            }

            if( c == '.' ) {
                while( (s != end) && valid_for_dec_(c = *s++)) {
                    if( !is_gap(c) ) {
                        a = a*10.0 + (c - '0');
                        e = e-1;
                    }
                }
            }

            if( (c == 'e' || c == 'E') ) {
                int sign = 1;
                int i = 0;
                c = *s++;

                if (c == '+') {
                    c = *s++;
                } else if (c == '-') {
                    c = *s++;
                    sign = -1;
                }

                while( valid_for_dec_( c ) ) {
                    if( is_gap(c) ) {
                        c = *s++;
                    }
                    i = i * 10 + (c - '0');
                    if( s == end ) {
                        break;
                    } else {
                        c = *s++;
                    }
                }
                e += i*sign;
            }

            while( e > 0 ) {
                a *= 10.0;
                e--;
            }

            while( e < 0 ) {
                a *= 0.1;
                e++;
            }

            if( s != end ) {
                --s;
            }
            return a;
        }

    };
    using numeric = numeric_templ<'_'>;
}}

#endif
