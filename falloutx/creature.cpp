#include "main.h"

BSDATAC(creaturei, 240)

creaturei* creaturei::ptr(const void* v) {
	auto p = (creaturei*)bsdata<creaturei>::source.ptr(v);
	if(p)
		return p;
	p = (creaturei*)game.playersa.ptr(v);
	if(p)
		return p;
	return 0;
}

void creaturei::updateanm() {
	if(next_stamp >= game.getaitime())
		return;
	next_stamp += 2500 / 10;
	if(!drawable::update())
		return;
	auto pai = draw::anminfo::get(rid);
	if(!pai)
		return;
	if(pai[getanimate()].frame_count == 1)
		return;
	auto a = getanimate();
	auto w = -1;
	if(a >= FirstWeaponAnimate)
		a = getbaseanimate(a, &w);
	auto hp = get(HPCur);
	auto& ai = bsdata<animationi>::elements[a];
	if(!ai.next)
		setanimate(AnimateStand);
	else if(ai.next_dead && hp <= 0)
		setanimate(getanimate(ai.next_dead, w));
	else
		setanimate(getanimate(ai.next, w));
}

void creaturei::clear() {
	memset(this, 0, sizeof(*this));
}

void creaturei::update() {
	current = basic;
	current.update(getarmor());
	current.update(getweapon());
	current.update();
}

void creaturei::finish() {
	set(HPCur, get(HP));
	set(APCur, 0);
}

bool creaturei::generate() {
	statable prev; prev.clear();
	for(auto i = Strenght; i <= Luck; i = (stat_s)(i + 1))
		prev.set(i, 1);
	auto result = choosestats(false, &prev);
	finish();
	return result;
}

void creaturei::charsheet() {
	statable prev = basic;
	if(!choosestats(true, &prev)) {
		basic = prev;
		update();
	}
}

void creaturei::create(pregen_s v) {
	auto& ei = bsdata<pregeni>::elements[v];
	clear();
	basic.apply(ei);
	if(!v) {
		basic.set(PrimaryPoints, 5);
		basic.set(SkillTagPoints, 3);
	}
	update();
	setname(ei.txt.name);
	setgender(ei.gender);
	finish();
}

bool creaturei::raise(skill_s v, int i, bool interactive) {
	if(i > 0) {
		if(basic.get(SkillPoints) < i) {
			if(interactive)
				draw::message(variant(NotEnoughtScoresToRaise), variant(SkillPoints).getname());
			return false;
		}
	}
	auto new_value = basic.get(v) + i;
	if(basic.tag.is(v))
		new_value += i;
	if(new_value >= 100) {
		if(interactive)
			draw::message(variant(InvalidMaximum), get(v));
		return false;
	}
	if(new_value < 0) {
		if(interactive)
			draw::message(variant(InvalidMinimal), get(v));
		return false;
	}
	basic.add(SkillPoints, -i);
	basic.set(v, new_value);
	update();
	return true;
}

bool creaturei::raise(stat_s v, int i, bool interactive) {
	if(i > 0) {
		if(basic.get(PrimaryPoints) < i) {
			if(interactive)
				draw::message(variant(NotEnoughtScoresToRaise), variant(PrimaryPoints).getname());
			return false;
		}
		if(get(v) + i > 10) {
			if(interactive)
				draw::message(variant(InvalidMaximum), get(v));
			return false;
		}
	}
	auto new_value = basic.get(v) + i;
	if(new_value < 1) {
		if(interactive)
			draw::message(variant(InvalidMinimal), get(v));
		return false;
	}
	basic.add(PrimaryPoints, -i);
	basic.set(v, new_value);
	update();
	return true;
}

bool creaturei::toggle(trait_s v, bool interactive) {
	if(!basic.trait.is(v)) {
		if(basic.gettraitpoints() >= 2) {
			if(interactive)
				draw::message(variant(InvalidTrait), 2);
			return false;
		}
		basic.trait.add(v);
	} else
		basic.trait.remove(v);
	update();
	return true;
}

bool creaturei::toggle(skill_s v, bool interactive) {
	if(!basic.tag.is(v)) {
		if(!basic.get(SkillTagPoints)) {
			if(interactive)
				draw::message(variant(NotEnoughtScoresToRaise), variant(SkillTagPoints).getname());
			return false;
		}
		basic.add(SkillTagPoints, -1);
		basic.tag.add(v);
	} else {
		basic.add(SkillTagPoints, 1);
		basic.tag.remove(v);
	}
	update();
	return true;
}

void creaturei::add(variant v, int i) {
	if(v.type == Item) {
		item e((item_s)v.value);
		e.setcount(i);
		e.create();
		additem(e);
	} else
		basic.add(v, i);
	update();
}

void creaturei::getactions(actiona& source, const item& it) const {
	auto& ei = it.geti();
	auto fast_shoot = is(FastShoot);
	if(!it) {
		source.add(ThrowPunch, 4, false);
		source.add(ThrowPunch, 4, true);
		source.add(KickLeg, 5, false);
		source.add(KickLeg, 5, true);
	} else if(ei.isweapon()) {
		source.add(FireSingle, ei.weapon.ap, false);
		if(!fast_shoot)
			source.add(FireSingle, ei.weapon.ap, true);
		source.add(FireSingle, ei.weapon.ap, false);
	} else
		source.add(Throw, 4, false);
}

action creaturei::getaction() const {
	actiona actions;
	getactions(actions, getweapon());
	if(!actions)
		return {};
	return actions[action_index % actions.getcount()];
}

void creaturei::add(stringbuilder& sb, const item& weapon) const {
	sb.addn(weapon.getname());
	if(weapon.isweapon()) {
		auto& ei = weapon.geti().weapon;
		sb.addn("Повр: %1i-%2i Длн: %3i", ei.min, ei.max, ei.range);
		if(ei.ammo) {
			auto ammo = weapon.getclipammo();
			auto count = weapon.getclipcount();
			if(!ammo)
				sb.addn("Нет зарядов");
			else
				sb.addn("Заряд: %1i/%2i %3", weapon.getclipcount(), ei.ammo_count, variant(ammo).getname());
		}
	} else {
		sb.addn(" ");
		sb.addn(" ");
	}
}