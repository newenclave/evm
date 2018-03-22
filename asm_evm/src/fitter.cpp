#include "fitter.h"
#include "evm/endian.h"

namespace evm { namespace assembler {
	fitter::memory_type &fitter::get_mem()
	{
		return mem_;
	}

	void fitter::clear()
	{
		mem_.clear();
	}

	void fitter::push_code(std::uint8_t code)
	{
		mem_.push_back(code);
	}

	void fitter::push_int(std::uint64_t value)
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

	void fitter::push_float(double value)
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

	void fitter::fill_until(std::size_t target_size)
	{
		if (target_size > mem_.size()) {
			mem_.resize(target_size);
		}
	}

}}