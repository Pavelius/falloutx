#pragma once

template <class T>
class pushst {
	T*			ptr;
	T			value;
public:
	constexpr pushst(T& v) : ptr(&v), value(v) {}
	~pushst() { if(ptr) *ptr = value; }
	void		clear() { ptr = 0; }
};