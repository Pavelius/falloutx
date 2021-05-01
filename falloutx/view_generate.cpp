#include "draw.h"
#include "main.h"
#include "screenshoot.h"

using namespace draw;

struct focusi {
	variant				id;
	rect				rc;
};
BSDATAC(focusi, 128)

static variant			current_rollup;
static variant			current_info;
static variant			charsheet_tabs[] = {Bonuses, Karma, KilledCreatures};

void draw::setfocus(variant v) {
	current_info = v;
}

static void event_focus() {
	setfocus(hot.param);
}

variant draw::getfocus() {
	return current_info;
}

static void post(void* p, fnevent ev, variant id, int param) {
	execute(ev, id, param, p);
}

static const focusi* getby(variant id) {
	for(auto& e : bsdata<focusi>()) {
		if(e.id == id)
			return &e;
	}
	return 0;
}

static focusi* getfirst() {
	if(bsdata<focusi>::source.getcount() != 0)
		return bsdata<focusi>::elements;
	return 0;
}

static focusi* getlast() {
	if(bsdata<focusi>::source.getcount() != 0)
		return (focusi*)bsdata<focusi>::source.end();
	return 0;
}

static const focusi* getnext(const focusi* pc, int key) {
	if(!key)
		return pc;
	auto pf = getfirst();
	if(!pc)
		pc = getfirst();
	if(!pc)
		return 0;
	auto pe = pc;
	auto pl = getlast();
	int inc = 1;
	if(key == KeyLeft || key == KeyUp || key == (KeyTab | Shift))
		inc = -1;
	while(true) {
		pc += inc;
		if(pc > pl)
			pc = pf;
		else if(pc < pf)
			pc = pl;
		if(pe == pc)
			return pe;
		switch(key) {
		case KeyRight:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 < pc->rc.x1)
				return pc;
			break;
		case KeyLeft:
			if(pe->rc.y1 >= pc->rc.y1
				&& pe->rc.y1 <= pc->rc.y2
				&& pe->rc.x1 > pc->rc.x1)
				return pc;
			break;
		case KeyDown:
			if(pc->rc.y1 >= pe->rc.y2)
				return pc;
			break;
		case KeyUp:
			if(pc->rc.y2 <= pe->rc.y1)
				return pc;
			break;
		default:
			return pc;
		}
	}
}

variant draw::getnext(variant id, int key) {
	auto p = getby(id);
	if(!p)
		return id;
	p = getnext(p, key);
	if(!p)
		return id;
	return p->id;
}

static void modify_primary() {
	auto p = (creaturei*)hot.object;
	auto i = hot.param2;
	auto v = (stat_s)hot.param;
	setfocus(v);
	p->raise(v, i, true);
}

static void modify_skills() {
	auto p = (creaturei*)hot.object;
	auto i = hot.param2;
	auto v = (skill_s)hot.param;
	setfocus(v);
	p->raise(v, i, true);
}

static void modify_traits() {
	auto p = (creaturei*)hot.object;
	auto v = (trait_s)hot.param;
	setfocus(v);
	p->toggle(v, true);
}

static void modify_tags() {
	auto p = (creaturei*)hot.object;
	auto v = (skill_s)hot.param;
	setfocus(v);
	p->toggle(v, true);
}

bool draw::focusable(const rect& rc, variant id) {
	auto p = bsdata<focusi>::add();
	if(p) {
		p->id = id;
		p->rc = rc;
	}
	if(ishilite(rc)) {
		if(id != current_info && hot.key == MouseLeft && hot.pressed)
			execute(event_focus, id);
	}
	return id == current_info;
}

static color setcolor(bool checked, bool disabled, bool focused) {
	auto r = fore;
	if(disabled)
		setcolor(ColorDisable);
	else if(checked)
		setcolor(ColorCheck);
	else
		setcolor(ColorText);
	if(focused)
		fore = fore.hilight();
	return r;
}

static void label(int x, int y, int width, variant id, bool checked, const char* title) {
	auto old_fore = setcolor(checked, false, focusable({x, y, x + width, y + 12}, id));
	text(x, y, title);
	fore = old_fore;
}

static void labelr(int x, int y, int width, variant id, int value, int maximum) {
	if(!maximum)
		return;
	auto old_fore = fore;
	auto rang = value * 256 / maximum;
	auto focused = focusable({x, y, x + width, y + 12}, id);
	setcolor(ColorDisable);
	if(focused)
		fore = fore.hilight();
	text(x, y, id.getnameshort());
	fore = old_fore;
}

static void label(int x, int y, int width, variant id, bool checked) {
	label(x, y, width, id, checked, id.getnameshort());
}

static void label(int x, int y, int width, variant id, bool checked, int value) {
	auto old_fore = setcolor(checked, false, focusable({x, y, x + width, y + 11}, id));
	text(x, y, id.getnameshort());
	char temp[32]; stringbuilder sb(temp);
	sb.add(id.getformat(), value);
	text(x + width - 22, y, temp);
	fore = old_fore;
}

static void labelf(int x, int y, int width, variant id, bool checked, int value) {
	auto old_fore = setcolor(checked, false, focusable({x, y, x + width, y + 11}, id));
	text(x, y, id.getname());
	char temp[32]; stringbuilder sb(temp);
	sb.add(id.getformat(), value);
	text(x + width - 22, y, temp);
	fore = old_fore;
}

static void label(int x, int y, int width, variant id, bool checked, int value, int value2) {
	auto old_fore = setcolor(checked, false, focusable({x, y, x + width, y + 11}, id));
	text(x, y, id.getnameshort());
	char temp[32]; stringbuilder sb(temp);
	sb.add("%1i/%2i", value, value2);
	text(x + width - textw(temp), y, temp);
	fore = old_fore;
}

static void labeln(int x, int y, int width, variant id, bool checked, int value) {
	auto old_fore = setcolor(checked, false, focusable({x, y, x + width, y + 11}, id));
	char temp[260]; stringbuilder sb(temp);
	sb.add("%1: %2i", id.getnameshort(), value);
	text(x, y, temp);
	fore = old_fore;
}

static void button(int x, int y, int width, int normal, int pressed, fnevent proc, void* param, const char* title, unsigned key, bool disabled) {
	auto old_font = font;
	auto old_fore = fore;
	auto rp = false;
	auto r = buttonf(x, y, normal, pressed, key, false, false, &rp, disabled);
	setfont(FONT3);
	setcolor(ColorButton);
	text(x + (width - textw(title)) / 2 + (rp ? 1 : 0), y + 7, title);
	if(r)
		execute(proc, 0, 0, param);
	font = old_font;
	fore = old_fore;
}

static void button(int x, int y, int width, fnevent proc, void* param, const char* title, unsigned key = 0) {
	if(button(x, y, width, title, key))
		execute(proc, 0, 0, param);
}

static void event_name() {
	auto p = (creaturei*)hot.object;
	char value[16]; stringbuilder sb(value);
	sb.addv(p->getname(), 0);
	if(game.choosename(13, 1, value, sizeof(value))) {
		p->setname(value);
		p->update();
	}
}

static void event_age() {
	auto p = (creaturei*)hot.object;
	auto value = p->get(Age);
	if(game.chooseage(156, 1, value, 13, 90)) {
		p->set(Age, value);
		p->update();
	}
}

static void event_gender() {
	auto p = (creaturei*)hot.object;
	auto value = p->getgender();
	if(game.choosegender(236, 1, value)) {
		p->setgender(value);
		p->update();
	}
}

static void event_save() {
	auto p = (creaturei*)hot.object;
	char temp[32]; stringbuilder sb(temp);
	sb.addv(p->getname(), 0);
	answers an; an.select("characters", "*.gch");
	if(an.edit(variant(Save), temp, sizeof(temp)))
		p->write(temp);
}

static void event_export() {
	auto p = (creaturei*)hot.object;
	char temp[32]; stringbuilder sb(temp);
	sb.addv(p->getname(), 0);
	answers an; an.select("characters", "*.txt");
	if(an.edit(variant(Save), temp, sizeof(temp)))
		p->tofile(temp);
}

static void event_load() {
	auto p = (creaturei*)hot.object;
	answers an; an.select("characters", "*.gch");
	auto pn = (const char*)an.choose(variant(Load));
	if(pn)
		p->read(pn);
}

static void event_delete_file() {
	auto p = (creaturei*)hot.object;
	answers an; an.select("characters", "*.gch");
	auto pn = (const char*)an.choose(variant(Delete));
	if(pn)
		p->deletegch(pn);
}

static void event_error() {
	variant v1 = hot.param;
	message(v1.getname(), hot.param2);
}

static fnevent chooseopt() {
	answers an;
	an.add((int)event_save, variant(Save));
	an.add((int)event_load, variant(Load));
	an.add((int)event_delete_file, variant(Delete));
	an.add((int)event_export, variant(Print));
	return (fnevent)an.choose();
}

static void event_options() {
	auto old_hot = hot;
	auto pf = chooseopt();
	if(pf) {
		hot = old_hot;
		pf();
	}
}

static void event_next() {
	if(hot.param)
		message(variant(InvalidStartGame));
	else
		buttonok();
}

static void set_info_mode() {
	game.infopage = hot.param;
	setfocus(charsheet_tabs[game.infopage]);
}

static void label(int x, int y, int width, const creaturei* p, variant id, bool show_maximum_only) {
	switch(id.type) {
	case Damage:
		label(x, y, width, id, false, p->get(id));
		break;
	case Stat:
		if(id.value == PoisonPoints || id.value == RadiationPoints)
			labelr(x, y, width, id, p->get(id), 3);
		else if(id.value == HP) {
			if(show_maximum_only)
				label(x, y, width, id, false, p->get(id));
			else
				label(x, y, width, id, false, p->get(HPCur), p->get(id));
		} else
			label(x, y, width, id, false, p->get(id));
		break;
	case Wound:
		labelr(x, y, width, id, p->get(id), 1);
		break;
	}
}

static int label(int x, int y, int width, const creaturei* p, std::initializer_list<variant> elements, bool show_maximum_only) {
	auto y0 = y;
	for(auto id : elements) {
		label(x, y, width, p, id, show_maximum_only);
		y += 13;
	}
	return y - y0;
}

void creaturei::statinfo(int x, int y, variant focus) const {
	if(!focus)
		return;
	auto old_font = font;
	auto old_fore = fore;
	setfont(FONT2);
	setcolor(ColorInfo);
	line(x, y + 25, x + 270, y + 25);
	line(x, y + 26, x + 270, y + 26);
	image(x + 133, y + 33, SKILLDEX, focus.getavatar(), ImageNoOffset);
	auto p = focus.getname();
	auto w1 = textw(p);
	text(x, y, p);
	setfont(FONT1);
	const formula* f = 0;
	switch(focus.type) {
	case Skill:
		f = &bsdata<skilli>::elements[focus.value].expression;
		break;
	case Stat:
		f = basic.getformula((stat_s)focus.value);
		break;
	}
	if(f) {
		char temp[260]; stringbuilder sb(temp);
		temp[0] = 0; f->add(sb);
		text(x + 270 - textw(temp), y + 14, temp);
	}
	p = focus.getdescription();
	if(p && p[0])
		textf(x, y + 33, 135, p);
	font = old_font;
	fore = old_fore;
}

bool creaturei::choosestats(bool charsheet_mode, const statable* previous) {
	char temp[260]; stringbuilder sb(temp);
	answers an;
	int x, y, hkey;
	auto show_maximum_only = !charsheet_mode;
	openform();
	setfocus(Strenght);
	if(!current_rollup)
		current_rollup = SmallGuns;
	while(ismodal()) {
		bsdata<focusi>::source.clear();
		if(charsheet_mode)
			background(177);
		else
			background(169);
		if(charsheet_mode) {
			variant id = getfocus();
			if(id.type == Skill)
				current_rollup = id;
		}
		// Player background buttons
		sb.clear(); sb.add("%1i лет", get(Age));
		button(13, 0, 138, 185, 184, event_name, this, getname(), '1', charsheet_mode);
		button(154, 0, 80, 188, 189, event_age, this, temp, '2', charsheet_mode);
		button(235, 0, 80, 188, 189, event_gender, this, variant(getgender()).getnameshort(), '3', charsheet_mode);
		// Text labels
		auto old_font = font;
		auto old_fore = fore;
		setfont(FONT3);
		setcolor(ColorButton);
		if(!charsheet_mode) {
			text(50, 326, "Особенности");
			text(18, 286, "Очки хар.");
		} else {
			static int info_mode_frame[] = {180, 178, 179};
			auto x = 10, y = 326, w = 100;
			image(x, y, INTRFACE, maptbl(info_mode_frame, game.infopage), ImageNoOffset);
			for(auto i = 0; i < 3; i++) {
				auto x1 = x + w * i + 10;
				rect rc = {x1 + 4, y, x1 + w - 4, y + 28};
				if((ishilite(rc) && hot.key == MouseLeft && hot.pressed)
					|| (hot.key == ('1' + i)))
					execute(set_info_mode, i);
				auto pn = charsheet_tabs[i].getname();
				text(x1 + (w - textw(pn)) / 2, y + ((i == game.infopage) ? 4 : 6), pn);
			}
		}
		text(383, 5, "Навыки");
		if(charsheet_mode)
			text(390, 233, "Очки умений");
		else
			text(428, 233, "Метки");
		font = old_font;
		fore = old_fore;
		// Primary stats
		y = 37;
		for(variant i = FirstPrimary; i.value <= LastPrimary; i.value++) {
			rect rc = {102, y, 102 + 42, y + 9};
			auto value = get(i);
			if(!charsheet_mode) {
				auto isfocused = (variant(i) == current_info);
				hkey = isfocused ? '+' : 0;
				if(buttonf(149, y, 193, 194, hkey, false))
					post(this, modify_primary, i.value, 1);
				hkey = isfocused ? '-' : 0;
				if(buttonf(149, y + 12, 191, 192, hkey, false)) {
					if(previous && previous->get(i) >= basic.get(i))
						execute(event_error, variant(InvalidMinimal), get(i));
					else
						post(this, modify_primary, i.value, -1);
				}
			}
			number(59, y, 2, value);
			unsigned n = value;
			if(n > bsdata<gradei>::source.getcount())
				n = bsdata<gradei>::source.getcount();
			if(n < 1)
				n = 1;
			auto p = bsdata<gradei>::elements[n - 1].txt.name;
			if(charsheet_mode)
				label(102, y + 8, 84, i, false, p);
			else
				label(101, y + 8, 64, i, false, p);
			y += 33;
		}
		const int stw = 120;
		if(charsheet_mode) {
			auto x = 30, y = 278;
			labeln(x, y, stw, Level, false, get(Level)); y += 13;
			labeln(x, y, stw, Experience, false, get(Experience)); y += 13;
			labeln(x, y, stw, NextLevelExperience, false, get(NextLevelExperience)); y += 13;
			y = 366;
			switch(game.infopage) {
			case 0:
				for(auto i = FirstPerk; i <= LastPerk; i = (perk_s)(i + 1)) {
					if(y >= clipping.y2)
						break;
					//if(is(i))
					//	y += render_stat(x, y, 120, i);
				}
				break;
			}
		} else
			number(126, 282, 2, get(PrimaryPoints));
		// Secondanary stats
		static std::initializer_list<variant> secondanary_stat_health = {
			HP, PoisonPoints, RadiationPoints,
			WoundEye, WoundRightHand, WoundLeftHand, WoundRightLeg, WoundLeftLeg
		};
		label(194, 46, stw, this, secondanary_stat_health, show_maximum_only);
		static std::initializer_list<variant> secondanary_stat_other = {
			AC, AP, CarryWeight, DamageMelee, Sequence, Phisycal, PoisonResistance, RadiationResistance,
			HealingRate, CriticalHit
		};
		label(194, 182, stw, this, secondanary_stat_other, show_maximum_only);
		// Traits
		if(!charsheet_mode) {
			const auto w1 = 90;
			for(variant i = (trait_s)0; i.value < 16; i.value++) {
				hkey = (getfocus() == i) ? KeySpace : 0;
				auto x = (i.value > 7) ? 299 : 23;
				auto x1 = (i.value > 7) ? 189 : 48;
				auto y = 352 + 13 * (i.value % 8);;
				if(radio(x, y, 215, hkey))
					post(this, modify_traits, i.value);
				label(x1, y + 1, w1, i, basic.get(i) > 0);
			}
		}
		// Skills
		x = 375; y = 27; auto w1 = 230;
		for(auto i = FirstSkill; i <= LastSkill; i = skill_s(i + 1)) {
			labelf(x, y, w1, i, basic.tag.is(i), get(i));
			if(!charsheet_mode) {
				hkey = (getfocus() == variant(i)) ? KeySpace : 0;
				if(radio(347, y, 215, hkey)) {
					if(previous && previous->tag.is(i) && basic.tag.is(i))
						execute(event_error, variant(InvalidRemove));
					else
						execute(modify_tags, i, 0, this);
				}
			}
			y += 11;
		}
		if(current_rollup.type == Skill && charsheet_mode) {
			x = 634;
			y = 32 + 11 * (current_rollup.value - FirstSkill);
			image(x, y, INTRFACE, 190);
			auto isfocused = (current_rollup == current_info);
			hkey = isfocused ? '+' : 0;
			if(buttonf(x - 20, y - 12, 193, 194, hkey, false))
				post(this, modify_skills, current_rollup.value, 1);
			hkey = isfocused ? '-' : 0;
			if(buttonf(x - 20, y, 191, 192, hkey, false)) {
				if(previous && previous->get(current_rollup) >= basic.get(current_rollup))
					execute(event_error, variant(InvalidMinimal), get(current_rollup));
				else
					post(this, modify_skills, current_rollup.value, -1);
			}
		}
		if(charsheet_mode)
			number(524, 228, 2, get(SkillPoints));
		else
			number(524, 228, 2, get(SkillTagPoints));
		// Description
		statinfo(350, 275, getfocus());
		// Dialog buttons
		if(charsheet_mode) {
			button(345, 454, 90, event_export, this, variant(Print), 'P');
			if(button(456, 453, 80, variant(Next), KeyEnter))
				execute(event_next);
			button(553, 454, 80, buttoncancel, this, variant(Back), KeyEscape);
		} else {
			button(345, 454, 90, event_options, this, variant(Options), 'O');
			if(button(452, 454, 80, variant(Next), KeyEnter))
				execute(event_next);
			button(552, 454, 80, buttoncancel, this, variant(Back), KeyEscape);
		}
		domodal();
		switch(hot.key) {
		case KeyTab:
		case KeyUp:
		case KeyDown:
		case KeyRight:
		case KeyLeft:
			setfocus(getnext(getfocus(), hot.key));
			break;
		}
	}
	closeform();
	return getresult() != 0;
}

static void take_pregen() {
	game.getplayer().create((pregen_s)hot.param);
	setstage(game.introducing);
}

static void change_pregen() {
	game.getplayer().create((pregen_s)hot.param);
	auto result = game.getplayer().generate();
	if(result)
		setstage(game.introducing);
}

static void biography(int x, int y, int width, const char* name, const char* description) {
	char temp[260]; stringbuilder sb(temp);
	char temz[64]; stringbuilder s1(temz); s1.addof(name);
	sb.add("История %1:", temz);
	sb.upper();
	text(x, y, temp);
	if(description)
		textf(x, y + 30, 160, description);
}

void gamei::newgame() {
	char temp[32]; stringbuilder sb(temp);
	static pregen_s	pregens[] = {Narg, Mingun, Chitsa};
	const unsigned pregens_max = sizeof(pregens) / sizeof(pregens[0]);
	int index = 0, index_cash = -1;
	const int width = getwidth();
	const int height = getheight();
	creaturei player; player.clear();
	openform();
	while(ismodal()) {
		if(index != index_cash) {
			index_cash = index;
			player.create(pregens[index_cash]);
		}
		auto& ei = bsdata<pregeni>::elements[pregens[index_cash]];
		background(174);
		if(radio(81, 322, 8, KeyEnter))
			execute(take_pregen, pregens[index_cash]);
		if(radio(436, 319, 8, 'C'))
			execute(change_pregen, pregens[index_cash]);
		if(radio(81, 424, 8, 0))
			execute(change_pregen, NoPregen);
		if(radio(461, 424, 8, KeyEscape))
			execute(buttoncancel);
		if(buttonf(292, 320, 122, 123, KeyLeft, false)) {
			if(!index)
				execute(setint, pregens_max - 1, 0, &index);
			else
				execute(setint, index - 1, 0, &index);
		}
		if(buttonf(318, 320, 124, 125, KeyRight, false))
			execute(setint, (index + 1) % pregens_max, 0, &index);
		// premade info
		image(width / 2, 40 + height / 2, INTRFACE, 201 + index);
		text((width - textw(player.getname())) / 2, 40, player.getname());
		biography(width / 2 + 120, 40, 160, ei.txt.name, ei.txt.text);
		// stats
		auto x1 = 295, y1 = 70, w1 = 70, w2 = 20;
		for(variant v = Strenght; v.value <= Luck; v.value++) {
			auto p = v.getname(); auto value = player.get(v);
			sb.clear(); sb.add("%1i", value);
			text(x1 + w1 - 8 - textw(p), y1, p);
			text(x1 + w1, y1, temp);
			text(x1 + w1 + w2, y1, bsdata<gradei>::elements[value - 1].txt.name);
			y1 += texth();
		}
		y1 += 5;
		x1 += 20;
		static variant fast_info[] = {HP, AC, AP, DamageMelee};
		for(auto v : fast_info) {
			auto p = v.getnameshort(); auto value = player.get(v);
			sb.clear(); sb.add("%1i", value);
			text(x1 + w1 + w2 - 4 - textw(p), y1, p);
			text(x1 + w1 + w2, y1, temp);
			y1 += texth();
		}
		y1 += 5;
		// tags
		for(auto i = FirstSkill; i <= LastSkill; i = (skill_s)(i + 1)) {
			if(!ei.tag.is(i))
				continue;
			auto p = variant(i).getname();
			auto value = player.get(i);
			sb.clear(); sb.add("%1i%%", value);
			text(x1 + w1 + w2 - 4 - textw(p), y1, p);
			text(x1 + w1 + w2, y1, temp);
			y1 += texth();
		}
		for(auto i = FirstTrait; i <= LastTrait; i = (trait_s)(i + 1)) {
			if(!ei.trait.is(i))
				continue;
			auto p = variant(i).getname();
			text(x1 + w1 + w2 - 4 - textw(p), y1, p);
			y1 += texth();
		}
		domodal();
	}
	closeform();
}