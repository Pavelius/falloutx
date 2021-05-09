#include "main.h"

animate_s actor::getbaseanimate(animate_s v, int* w) {
	if(isweaponanimate(v)) {
		if(w)
			*w = (v - FirstWeaponAnimate) / 13;
		return animate_s(FirstWeaponAnimate + (v - FirstWeaponAnimate) % 13);
	}
	return v;
}

animate_s actor::getbaseanimate() const {
	return getbaseanimate(getanimate(), 0);
}

bool actor::isweaponanimate(animate_s v) {
	return v >= FirstWeaponAnimate;
}

animate_s actor::getanimate(animate_s v, int w) {
	if(w != -1 && isweaponanimate(v))
		return animate_s(v + w * 13);
	return v;
}

int	actor::getcicle(draw::res_s& rid, animate_s v, gender_s gender, item_s armor, item_s weapon, int dir) {
	rid = (gender == Male) ? bsdata<itemi>::elements[armor].armor.male : bsdata<itemi>::elements[armor].armor.female;
	if(!rid)
		rid = (gender == Male) ? draw::HMJMPS : draw::HFJMPS;
	auto wp = bsdata<itemi>::elements[weapon].avatar.animation;
	if(wp > 0) {
		switch(v) {
		case AnimateStand: v = AnimateWeaponStand; break;
		case AnimateWalk: v = AnimateWeaponWalk; break;
		case AnimateDodge: v = AnimateWeaponDodge; break;
		case AnimateThrown: v = AnimateWeaponThrow; break;
		case AnimateUnarmed1: v = AnimateWeaponSwing; break;
		case AnimateUnarmed2: v = AnimateWeaponThrust; break;
		}
		return (v + (wp - 1) * 13) * 6 + dir;
	}
	return v * 6 + dir;
}

void actor::reanimate() {
	draw::res_s rid;
	auto cicle = getcicle(rid, animate, getgender(), getarmor(), getweapon(), direction);
	set(rid, cicle);
	next_stamp = game.getaitime();
}

void actor::setdirection(unsigned char v) {
	direction = v % 6;
	set(rid, animate * 6 + direction);
}

void actor::moveto(indext v) {
	setdirection(getdirection(*this, draw::h2s(v)));
	setposition(v);
}

void actor::setposition(indext v) {
	index_position = v;
	drawable::setposition(draw::h2s({(short)loc.gethx(v), (short)loc.gethy(v)}));
}

void actor::setanimate(animate_s v, bool force) {
	if(!force && animate == v)
		return;
	animate = v;
	reanimate();
}

void actor::doanimate(animate_s v, bool stop_and_wait) {
	setanimate(v, true);
	if(stop_and_wait)
		wait();
}

bool wearable::additem(item& ei) {
	for(auto& e : gears) {
		if(!e)
			continue;
		if(e.join(ei))
			return true;
	}
	for(auto& e : gears) {
		if(e)
			continue;
		e = ei;
		ei.clear();
		return true;
	}
	return false;
}

unsigned char actor::getdirection(point from, point to) {
	static const unsigned char orientations[25] = {
		5, 5, 0, 0, 0,
		5, 5, 0, 0, 0,
		4, 4, 2, 1, 1,
		3, 3, 2, 2, 2,
		3, 3, 3, 2, 2
	};
	int dx = to.x - from.x;
	int dy = to.y - from.y;
	int div = imax(iabs(dx), iabs(dy));
	if(!div)
		return 2; //default
	if(div > 3)
		div /= 2;
	int ax = dx / div;
	int ay = dy / div;
	return orientations[(ay + 2) * 5 + ax + 2];
}

int actor::getlongest(const point from, const point to) {
	return imax(iabs(from.x - to.x), iabs(from.y - to.y));
}

int actor::getrange(const point p1, const point p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return isqrt(dx * dx + dy * dy);
}

bool wearable::additems(wearable& source) {
	for(auto& e : source.gears) {
		if(!e)
			continue;
		if(!additem(e))
			return false;
	}
	return true;
}