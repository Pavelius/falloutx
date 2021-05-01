#include "main.h"

void drawablea::select() {
	auto ps = data + count;
	auto pe = endof();
	for(auto& e : bsdata<creaturei>()) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	for(auto& e : game.players) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

void drawablea::sortz() {
}