#pragma once 
#ifndef ASM_EVM_TRIE_H
#define ASM_EVM_TRIE_H

#include <memory>
#include <vector>
#include <stack>

namespace evm { namespace assembler {

	namespace nodes {
		template <typename KeyT, typename ValueT, typename Comp = std::less<KeyT> >
		class array {
		public:

			using key_type   = KeyT;
			using value_type = ValueT;
			using value_ptr  = std::unique_ptr<value_type>;

		private:

			using this_type  = array<key_type, value_type>;
			struct key_data {

				key_type    key;
				this_type   value;

				key_data( const key_data &other ) = delete;
				key_data& operator = ( const key_data &other ) = delete;

				explicit
				key_data( const key_type &k )
					:key(k)
				{ }

				key_data( key_data &&other )
					:key(std::move(other.key))
					,value(std::move(other.value))
				{ }

				key_data& operator = ( key_data &&other )
				{
					key   = std::move(other.key);
					value = std::move(other.value);
					return *this;
				}

				bool equal_keys( const key_type &rh ) const
				{
					return (!(key < rh)) && (!(rh < key));
				}

			};

			struct cmp {
				bool operator ( )( const key_data &d, const key_type &k ) const
				{
					Comp compare;
					return compare( d.key, k );
				}
			};

			using container  = std::vector<key_data>;

		public:

			array *get( const key_type &k )
			{
				if( next_.empty( ) ) {
					return nullptr;
				}
				auto f = std::lower_bound( next_.begin( ), next_.end( ),
										   k, cmp( ) );
				if( f != next_.end( ) ) {
					return (f->equal_keys( k ) ) ? &f->value : nullptr;
				} else {
					return nullptr;
				}
			}

			const array *get(const key_type &k) const
			{
				if (next_.empty()) {
					return nullptr;
				}
				auto f = std::lower_bound(next_.begin(), next_.end(),
					k, cmp());
				if (f != next_.end()) {
					return (f->equal_keys(k)) ? &f->value : nullptr;
				}
				else {
					return nullptr;
				}
			}

			array *set( const key_type &k )
			{
				if( next_.empty( ) ) {
					next_.emplace_back( key_data(k) );
					return &next_.begin( )->value;
				} else {
					auto f = std::lower_bound(next_.begin( ), next_.end( ),
											  k, cmp( ) );
					if( f != next_.end( ) && f->equal_keys( k ) ) {
						return &f->value;
					} else {
						auto nf = next_.emplace( f, key_data(k) );
						return &nf->value;
					}
				}
			}

			std::size_t remove(const array *node) 
			{
				for (auto b = next_.begin(); b != next_.end(); ++b) {
					if(&b->value == node) {
						next_.erase(b);
						return 1;
					}
				}
				return 0;
			}

			void set_value(value_type val)
			{
				val_.reset(new value_type(std::move(val)));
			}

			void reset_value( )
			{
				val_.reset();
			}

			bool empty() const
			{
				return next_.empty();
			}

			value_type *value( )
			{
				return val_.get( );
			}

			const value_type *value( ) const
			{
				return val_.get( );
			}

		private:

			value_ptr  val_;
			container  next_;
		};
	}

    template <typename KeyT, typename ValueT, typename Comp = std::less<KeyT>,
              template <typename,
                        typename,
                        typename> class NodeT = nodes::array>
    class trie {
    public:
        using key_type   = KeyT;
        using value_type = ValueT;
        using node_type  = NodeT<KeyT, ValueT, Comp>;

    public:
        base( ) = default;
        base( const base & ) = default;
        base& operator = ( const base & ) = default;
        base( base && ) = default;
        base& operator = ( base && ) = default;

		template <typename Iter>
		class const_result_view {
		public:

			~const_result_view( ) = default;
			const_result_view(const const_result_view &) = default;
			const_result_view(const_result_view &&) = default;
			const_result_view &operator = (const const_result_view &) = default;
			const_result_view &operator = (const_result_view &&) = default;
			const_result_view( ) = default;

			const_result_view(const node_type *node, Iter itrbegin, Iter itrend)
				:node_(node)
				,begin_(itrbegin)
				,end_(itrend)
			{ }

			operator bool( ) const
			{
				return node_ != nullptr;
			}

			const value_type &operator *( ) const
			{
				return *node_->value();
			}

			const value_type *operator ->( ) const
			{
				return node_->value( );
			}

			Iter begin() const
			{
				return begin_;
			}

			Iter end() const
			{
				return end_;
			}

		private:
			const node_type *node_ = nullptr;
			Iter             begin_;
			Iter             end_;
		};

        template <typename Iter>
        class result_view {
        public:

            ~result_view( ) = default;
            result_view ( const result_view &  ) = default;
            result_view (       result_view && ) = default;
            result_view & operator = ( const result_view &  ) = default;
            result_view & operator = (       result_view && ) = default;

            result_view( )
                :node_(nullptr)
            { }

            result_view( node_type *node, Iter itrbegin, Iter itrend )
                :node_(node)
                ,begin_(itrbegin)
                ,end_(itrend)
            { }

            operator bool ( ) const
            {
                return node_ != nullptr;
            }

            value_type &operator *( )
            {
                return *node_->value( );
            }

            const value_type &operator *( ) const
            {
                return *node_->value( );
            }

            value_type *operator -> ( )
            {
                return node_->value( );
            }

            const value_type *operator -> ( ) const
            {
                return node_->value( );
            }

            Iter begin( ) const
            {
                return begin_;
            }

            Iter end( ) const
            {
                return end_;
            }

        private:
            node_type *node_ = nullptr;
            Iter       begin_;
            Iter       end_;
        };

        template <typename IterT>
        void set( IterT begin, const IterT &end, value_type value )
        {
            set_s( &root_, begin, end, std::move(value) );
        }

        template <typename ContainerT>
        void set( const ContainerT &cnt, value_type value )
        {
            set( std::begin(cnt), std::end(cnt), std::move(value) );
        }

        void set( const key_type *ptr, size_t len, value_type value )
        {
            set( ptr, ptr + len, std::move(value) );
        }

		template <typename IterT>
		result_view<IterT> get(IterT b, const IterT &e, bool greedy)
		{
			return get_s<result_view>(&root_, b, e, greedy);
		}

		template <typename IterT>
		const_result_view<IterT> get(IterT b, const IterT &e, bool greedy) const
		{
			return get_s<const_result_view>(&root_, b, e, greedy);
		}

		using node_stack = std::stack<node_type *>;

		template <typename IterT>
		node_stack get_node_stack(IterT begin, const IterT &end) 
		{
			return get_node_stack(&root_, begin, end);
		}

		template <typename IterT>
		static
		node_stack get_node_stack(node_type *node, IterT begin, const IterT &end)
		{
			node_stack result;
			if (begin == end) {
				return node_stack();
			}

			for (; begin != end; ++begin) {
				result.push(node);
				node = node->get(*begin);
				if (!node) {
					break;
				}
			}
			if (node && node->value()) {
				result.push(node);
				return result;
			}
			return node_stack();
		}

	private:

        template <typename IterT>
        static
        void set_s( node_type *last,
                    IterT begin, const IterT &end, value_type value )
        {
            for( ;begin != end; ++begin ) {
                last = last->set( *begin );
            }
            last->set_value(std::move(value));
        }

        template <template <typename> class ResultType, 
                  typename IterT, typename NodeType>
        static
        ResultType<IterT> get_s( NodeType next_table,
                                 IterT b, const IterT &e, bool greedy )
        {
            using result_type = ResultType<IterT>;

			NodeType last_final = nullptr;

            if( b == e ) {
                return result_type(nullptr, b, e);
            }

			IterT start = b;
            IterT bb = b;
            ++bb;

            for( ; b!=e; ++b ) {

                next_table = next_table->get( *b );

                if( !next_table ) {
                    break;
                }

                if( next_table->value( ) ) {

                    last_final = next_table;

                    bb = b;
                    ++bb;

                    if( !greedy ) {
                        break;
                    }
                }
            }

            return last_final ? result_type(last_final, start, bb)
                              : result_type(nullptr, e, e);
        }

        node_type root_;
    };
} }

#endif