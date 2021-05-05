#include "main.h"

areai loc;
BSDATAC(mapobject, 256 * 16)

indext areai::geth(int x, int y) {
	if((unsigned)x >= width * 2 || (unsigned)y >= height * 2)
		return Blocked;
	return y * width * 2 + x;
}

void areai::clear() {
	memset(this, 0, sizeof(0));
	bsdata<mapobject>::source.clear();
}

void areai::set(indext i, int v, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			setfloor(geti(x, y), v);
	}
}

void areai::set(indext i, const tilei& e, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			setfloor(geti(x, y), e.random());
	}
}

mapobject* areai::add(indext index, draw::res_s rid, short unsigned type) {
	auto p = find(index);
	if(!p) {
		p = bsdata<mapobject>::add();
		memset(p, 0, sizeof(*p));
		p->index = index;
	}
	p->rid = rid;
	p->type = type;
	p->reanimate();
	return p;
}

mapobject* areai::find(indext index) {
	for(auto& e : bsdata<mapobject>()) {
		if(e.index == index)
			return &e;
	}
	return 0;
}

void areai::correct(indext i) {
	static direction_s all[] = {Up, Right, Down, Left};
	unsigned char c = 0;
	auto t = getfloor(i);
	auto p = tilei::find(t);
	if(!p)
		return;
	for(auto d : all) {
		c <<= 1;
		auto i1 = tot(i, d);
		if(i1 == Blocked)
			continue;
		auto v = getfloor(i1);
		if(p->is(v))
			c |= 1;
	}
	setfloor(i, p->correct(t, c));
}

indext areai::tot(indext i, direction_s d) {
	auto x = getx(i);
	auto y = gety(i);
	switch(d) {
	case Up:
		if(y == 0)
			return Blocked;
		return geti(x, y - 1);
	case Down:
		if(y == height - 1)
			return Blocked;
		return geti(x, y + 1);
	case Right:
		if(x == width - 1)
			return Blocked;
		return geti(x + 1, y);
	case Left:
		if(x == 0)
			return Blocked;
		return geti(x - 1, y);
	default:
		return Blocked;
	}
}

void areai::correct() {
	for(auto y = 0; y < 100; y++)
		for(auto x = 0; x < 100; x++)
			correct(geti(x, y));
}