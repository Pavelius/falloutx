#include "main.h"

areai loc;

void areai::clear() {
	memset(this, 0, sizeof(0));
}

void areai::set(indext i, int v) {
	tiles[i] = v;
}

void areai::set(indext i, int v, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			set(geti(x, y), v);
	}
}

void areai::set(indext i, const tilei& e, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			set(geti(x, y), e.random());
	}
}