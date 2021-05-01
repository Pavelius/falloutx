#include "main.h"

static int getexperience(int n) {
	return ((n * (n - 1)) / 2) * 1000;
}

int statable::get(const term& e) const {
	if(e.divider)
		return get(e.id) * e.multiplier / e.divider;
	return get(e.id) * e.multiplier;
}

int	statable::get(formula_s v) const {
	switch(v) {
	case Experience: return experience;
	case NextLevelExperience: return getexperience(stats[Level]+1);
	default: return 0;
	}
}

void statable::set(formula_s v, int i) {
	switch(v) {
	case Experience: experience = i; break;
	default: break;
	}
}

int	statable::get(variant v) const {
	switch(v.type) {
	case Damage: return resistance[v.value];
	case Formula: return get((formula_s)v.value);
	case Skill: return skills[v.value];
	case Stat: return stats[v.value];
	case Trait: return trait.is((trait_s)v.value) ? 1 : 0;
	default: return 0;
	}
}

void statable::set(variant v, int i) {
	switch(v.type) {
	case Stat: stats[v.value] = i; break;
	case Skill: skills[v.value] = i; break;
	case Trait:
		if(i > 0)
			trait.add((trait_s)v.value);
		else
			trait.remove((trait_s)v.value);
		break;
	}
}

const formula* statable::getformula(stat_s v) const {
	auto& ei = bsdata<stati>::elements[v];
	for(auto& e : ei.formulas) {
		if(!e.condition || get(e.condition) > 0)
			return &e;
	}
	return 0;
}

int statable::get(const formula* p) const {
	if(!p)
		return 0;
	return p->start + get(p->t1) + get(p->t2);
}

void statable::apply(varianta source) {
	auto mode = Plus;
	auto count = 1;
	for(auto v : source) {
		if(v.type == Modifier) {
			mode = (modifier_s)v.value;
			continue;
		} else if(v.type == Number) {
			count = bsdata<numberi>::elements[v.value].value;
			continue;
		}
		switch(mode) {
		case Minus: add(v, -count); break;
		case Plus: add(v, count); break;
		}
	}
}

int statable::getprimarypoints() const {
	auto r = 0;
	for(auto i = FirstPrimary; i <= LastPrimary; i = (stat_s)(i + 1))
		r += stats[i];
	return r;
}

int statable::getskillpoints() const {
	auto r = 0;
	for(auto e : skills)
		r += e;
	return r;
}

int	statable::gettraitpoints() const {
	auto r = 0;
	for(auto i = (trait_s)0; i <= LastTrait; i = (trait_s)(i + 1)) {
		if(trait.is(i))
			r++;
	}
	return r;
}

int	statable::gettagpoints() const {
	auto r = 0;
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		if(tag.is(i))
			r++;
	}
	return r;
}

void statable::apply_traits() {
	for(auto i = FirstTrait; i <= LastTrait; i = (trait_s)(i + 1)) {
		if(trait.is(i)) {
			auto& e = bsdata<traiti>::elements[i];
			apply(e.effect);
		}
	}
	// Automatic correction primary stats
	for(auto i = Strenght; i <= Luck; i = (stat_s)(i + 1)) {
		if(stats[i] > 10)
			stats[i] = 10;
	}
}

void statable::apply_derived() {
	for(auto i = FirstDerived; i <= LastDerived; i = (stat_s)(i + 1)) {
		auto f = getformula(i);
		if(f)
			stats[i] += evalute(*f);
	}
}

int	statable::evalute(const formula& e) const {
	return e.start + get(e.t1) + get(e.t2);
}

void statable::apply_skills() {
	for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
		auto& e = bsdata<skilli>::elements[i];
		skills[i] += evalute(e.expression);
		if(tag.is(i))
			skills[i] += 20;
	}
}

void statable::update() {
	apply_traits();
	apply_derived();
	apply_skills();
}

void statable::update(const item& it) {
	auto& ei = it.geti();
	for(auto i = Phisycal; i <= LastDamage; i = (damage_s)(i + 1)) {
		resistance[i] += ei.armor.resistance[i];
		threshold[i] += ei.armor.threshold[i];
	}
	stats[AC] += ei.armor.ac;
}

void statable::clear() {
	memset(this, 0, sizeof(*this));
}

void statable::apply(const pregeni& ei) {
	for(auto i = Strenght; i <= Luck; i = (stat_s)(i + 1))
		stats[i] += ei.stats[i - Strenght];
	tag |= ei.tag;
	trait |= ei.trait;
	set(Age, ei.age);
	set(Level, ei.level);
	set(Experience, getexperience(ei.level));
}