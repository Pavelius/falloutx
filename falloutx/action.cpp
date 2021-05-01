#include "main.h"

BSDATA(actioni) = {
	{{"NoAction"}, 252},
	{{"ThrowPunch"}, 42},
	{{"KickLeg"}, 41},
	{{"Swing"}, 44},
	{{"Thrust"}, 45},
	{{"Throw"}, 117},
	{{"FireSingle"}, 43},
	{{"FireBurst"}, 40},
	{{"Reload"}},
	{{"Drop"}, 254},
	{{"Look"}, 258},
	{{"Talk"}, 262},
	{{"Turn"}, 260},
	{{"Unload"}, 301},
	{{"Use"}, 264},
};
assert_enum(actioni, Use)

void actiona::add(action_s type, char ap, bool aimed) {
	auto p = adat::add();
	p->type = type;
	p->ap = ap;
	p->aimed = aimed;
	if(aimed)
		p->ap++;
}