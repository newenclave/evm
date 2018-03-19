#include <iostream>
#include <sstream>
#include "evm/machine.h"
#include "evm/endian.h"

namespace {

	void print_stack(evm::machine *M)
	{
		if (M->get_registers().sp == 0) {
			std::cout << "[ ]\n";
			return;
		}
		std::ostringstream oss;
		oss << "[";
		std::size_t top = static_cast<std::size_t>(M->get_registers().sp);
		while (top > 0) {
			switch (M->get_stack()[top].type)
			{
			case evm::machine::FLOATING:
				oss << " " << M->get_stack()[top].data.f64_value;
				break;
			case evm::machine::UINTEGER:
				oss << " " << M->get_stack()[top].data.u64_value;
				break;
			case evm::machine::REGISTER:
				oss << " r" << std::hex << M->get_stack()[top].data.u64_value
					<< std::dec;
				break;
			default:
				oss << " " << std::hex << M->get_stack()[top].data.u64_value
					<< std::dec;
				break;
			}
			--top;
		}
		oss << " ]";
		std::cout << oss.str() << "\n";
	}

	using mem_type = std::vector<std::uint8_t>;

	class assembler {
	public:

		void rep(std::size_t count)
		{
			mem_.resize(mem_.size() + count);
		}

		template <typename ...Args>
		void push(std::uint8_t code, Args&& ...args)
		{
			push_code(code);
			push_impl(std::forward<Args>(args)...);
		}

		std::uint64_t current()
		{
			return mem_.size();
		}

		const mem_type &memory() const
		{
			return mem_;
		}

	private:

		void push_code(std::uint8_t code)
		{
			mem_.push_back(code);
		}

		void push_data(int value)
		{
			push_data((std::uint64_t)value);
		}

		void push_data(std::uint64_t value)
		{
			auto data = reinterpret_cast<const std::uint8_t *>(&value);
#ifndef EVM_LITTLE_ENDIAN
			mem_.push_back(data[0]);
			mem_.push_back(data[1]);
			mem_.push_back(data[2]);
			mem_.push_back(data[3]);
			mem_.push_back(data[4]);
			mem_.push_back(data[5]);
			mem_.push_back(data[6]);
			mem_.push_back(data[7]);
#else 
			mem_.push_back(data[7]);
			mem_.push_back(data[6]);
			mem_.push_back(data[5]);
			mem_.push_back(data[4]);
			mem_.push_back(data[3]);
			mem_.push_back(data[2]);
			mem_.push_back(data[1]);
			mem_.push_back(data[0]);
#endif
		}

		void push_data(double value)
		{
			auto data = reinterpret_cast<const std::uint8_t *>(&value);
#ifdef EVM_LITTLE_ENDIAN
			mem_.push_back(data[0]);
			mem_.push_back(data[1]);
			mem_.push_back(data[2]);
			mem_.push_back(data[3]);
			mem_.push_back(data[4]);
			mem_.push_back(data[5]);
			mem_.push_back(data[6]);
			mem_.push_back(data[7]);
#else 
			mem_.push_back(data[7]);
			mem_.push_back(data[6]);
			mem_.push_back(data[5]);
			mem_.push_back(data[4]);
			mem_.push_back(data[3]);
			mem_.push_back(data[2]);
			mem_.push_back(data[1]);
			mem_.push_back(data[0]);
#endif
		}

		void push_impl()
		{}

		template <typename Head, typename ...Tail>
		void push_impl(Head head, Tail&& ...tail)
		{
			push_data(head);
			push_impl(std::forward<Tail>(tail)...);
		}

		mem_type mem_;
	};

}

int main() 
{
	using evm::op;
#define _(...) ASM.push(__VA_ARGS__)

	assembler ASM;
	// header. jump main
	_(	op::JMP, 0x40	);

	// function sum(a, b)
	ASM.rep(0x10 - ASM.current());
	_(	op::LODA, 1	); // load a 
	_(	op::LODA, 2	); // load b 
	_(	op::FADD		); // a + b 
	_(	op::RET, 1	); // return (a+b)

	// call main()
	ASM.rep(0x40 - ASM.current());
	_(	op::FPSH, 10.5		); // set a = 10
	_(	op::FPSH, 50.66		); // set b = 20 
	_(	op::CALL, 0x10, 2	); // sum(a, b)
	_(	op::POP				);  // reduce 1 
	_(	op::HLT				);  // stack.top = a + b

	evm::machine M(4096);
	M.memory() = ASM.memory();
	while (M.running()) {
		auto call = M.fetch();
		call(&M);
		print_stack(&M);
	}

    return 0;
}
