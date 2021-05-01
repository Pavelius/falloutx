#include "main.h"

void nameable::setname(const char* v) {
	stringbuilder sb(name);
	sb.addv(v, 0);
}