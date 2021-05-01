#include "crt.h"
#include "io.h"

#pragma once

struct archive {
	io::stream&			source;
	bool				writemode;
	constexpr archive(io::stream& source, bool writemode) : source(source), writemode(writemode) {}
	void				set(void* value, unsigned size);
	bool				signature(const char* id);
	bool				version(short major, short minor);
	// Fixed data collection
	template<typename T, unsigned N> void set(adat<T, N>& value) {
		set(value.count);
		set(value.data, sizeof(value.data[0]) * value.count);
	}
	// Fixed array collection
	template<typename T> void set(array& value) {
		set(value.count);
		set(value.size);
		if(!writemode)
			value.reserve(value.count);
		set(value.ptr(0), value.getsize() * value.count);
	}
	// All simple types and requisites
	template<class T> void set(T& value) {
		set(&value, sizeof(value));
	}
};