#include "draw.h"
#include "main.h"

using namespace draw;

const anminfo* anminfo::get(res_s rid) {
	return get(gres(rid));
}

const anminfo* anminfo::get(const sprite* ps) {
	if(!ps)
		return 0;
	return (anminfo*)ps->ptr(ps->cicles_offset + ps->cicles * sizeof(sprite::cicle));
}

bool drawable::update() {
	if(frame < frame_stop) {
		frame++;
		return false;
	}
	frame = frame_start;
	return true;
}

void drawable::set(draw::res_s v, int cicle) {
	rid = v;
	auto ps = gres(v);
	if(!ps)
		return;
	auto pc = ps->gcicle(cicle);
	frame_start = pc->start;
	frame_stop = frame_start + pc->count - 1;
	if(frame_stop < frame_start)
		frame_start = frame_stop;
	frame = frame_start;
}

void drawable::set(draw::res_s v, int cicle, point offset) {
	set(v, cicle);
	x = offset.x; y = offset.y;
}

void drawable::paint(int x, int y, unsigned flags) const {
	image(x, y, gres(rid), frame, flags);
}

res_s drawable::getrid(item_s armor, gender_s gender) {
	return (gender == Male) ? bsdata<itemi>::elements[armor].armor.male : bsdata<itemi>::elements[armor].armor.female;
}

void scenery::reanimate() {
	auto ps = gres(rid);
	if(!ps)
		return;
	auto pt = m2h(loc.gethx(index), loc.gethy(index));
	x = pt.x; y = pt.y;
	auto pc = ps->gcicle(type);
	frame = pc->start;
	frame_start = frame;
	frame_stop = frame;
}