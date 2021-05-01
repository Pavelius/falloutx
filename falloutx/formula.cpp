#include "main.h"

BSDATA(formulai) = {
	{{"Experience"}, 8},
	{{"NextLevelExperience"}, 9},
};
assert_enum(formulai, NextLevelExperience)

static void addterm(stringbuilder& sb, const char* pb, const term& t1) {
	if(t1.multiplier == 0)
		return;
	if(t1.multiplier > 0 && sb.get() != pb)
		sb.add(" + ");
	if(t1.multiplier < 0)
		sb.add(" - ");
	sb.add(variant(t1.id).getnameabr());
	auto m = iabs(t1.multiplier);
	if(m > 1)
		sb.add("x%1i", m);
	else if(t1.divider > 1)
		sb.add("/%1i", t1.divider);
}

void formula::add(stringbuilder& sb) const {
	auto pb = sb.get();
	if(start < 0) {
		addterm(sb, pb, t1);
		addterm(sb, pb, t2);
		sb.add("%1i", start);
	} else {
		if(start)
			sb.add("%1i", start);
		addterm(sb, pb, t1);
		addterm(sb, pb, t2);
	}
}