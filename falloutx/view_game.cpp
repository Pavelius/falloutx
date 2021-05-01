#include "draw.h"
#include "main.h"
#include "screenshoot.h"

using namespace draw;

const int			stx = tile_width / 5; // 16 - Ширина юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
const int			sty = tile_height / 3; // 12 - Высота юнита тайла. Каждый тайл имеет размер в 5х3 юнита.

static variant		hilite_object;
static drawablea	objects;

// Получение координаты тайла(x,y) на экране
point draw::m2s(int x, int y) {
	return{(short)(2 * stx * y + 3 * stx * x), (short)(2 * sty * y - sty * x)};
}

// Определение индекса тайла по координатам(x,y) на экране
point draw::s2m(point pt) {
	int x = pt.x + tile_width / 2 - 2;
	int y = pt.y + tile_height / 4 - 2;
	return{(short)((x - 4 * y / 3) / 64), (short)((x + 4 * y) / 128)};
}

// Получение координаты хексагона(x,y) на экране
point draw::m2h(int x, int y) {
	int x1 = stx * (y + x + x / 2) - stx / 2;
	int y1 = sty * (y - x + x / 2) - sty / 2;
	return{(short)x1, (short)y1};
}

point draw::h2m(point pt) {
	int x1 = pt.x + stx / 2;
	int y1 = pt.y;
	int x0 = 4800;
	int nx;
	if(x1 - x0 < 0)
		nx = (x1 - x0 + 1) / stx - 1;
	else
		nx = (x1 - x0) / stx;
	int ny;
	if(y1 < 0)
		ny = (y1 + 1) / sty - 1;
	else
		ny = y1 / sty;
	if(iabs(nx) % 2 != iabs(ny) % 2)
		nx--;
	long xhBase = x0 + stx * nx;
	long yhBase = sty * ny;
	int x = (4 * yhBase - 3 * (xhBase - x0)) / 96;
	int y = yhBase / 12 - x / 2;
	x = 150 - x;
	y = y + 76;
	return{(short)x, (short)y};
}

static void addpoint() {
	auto p = (point*)hot.object;
	p->x += hot.param;
	p->y += hot.param2;
}

static void render_tiles() {
	auto ps = gres(TILES);
	if(!ps)
		return;
	auto tm = getuitime();
	auto pm = s2m(game.camera);
	int x1 = pm.x - 8; int x2 = x1 + 19;
	int y1 = pm.y - 1; int y2 = y1 + 20;
	for(auto y = y1; y < y2; y++) {
		if(y < 0 || y >= areai::height)
			continue;
		for(int x = x1; x < x2; x++) {
			if(x < 0 || x >= areai::width)
				continue;
			auto tv = loc.gettile(loc.geti(x, y));
			if(tv > 0) {
				point pt = m2s(x, y);
				point pz = pt - game.camera;
				draw::image(pz.x, pz.y + tile_height / 2, ps, ps->ganim(tv, tm), 0);
			}
		}
	}
}

static void scrollmap(int x, int y, int cicle) {
	const int w = 640;
	const int h = 480;
	const int s = 4;
	const int dx = 16;
	const int dy = 12;
	rect rc = {};
	if(x == 0) {
		rc.x1 = s + 1;
		rc.x2 = w - s - 1;
	} else {
		rc.x1 = (x < 0) ? 0 : w - s;
		rc.x2 = rc.x1 + s;
	}
	if(y == 0) {
		rc.y1 = s + 1;
		rc.y2 = h - s - 1;
	} else {
		rc.y1 = (y < 0) ? 0 : h - s;
		rc.y2 = rc.y1 + s;
	}
	if(hot.mouse.in(rc)) {
		cursor.set(INTRFACE, cicle);
		if(hot.key == InputTimer)
			execute(addpoint, x * dx, y * dy, &game.camera);
	}
}

static void prepare_objects() {
	objects.select();
	objects.sortz();
}

static void update_objects() {
	for(auto p : objects) {
		variant v = p;
		switch(v.type) {
		case Creature:
			((creaturei*)v.getpointer())->updateanm();
			break;
		}
	}
}

static void render_objects() {
	for(auto p : objects)
		p->paint(p->x - game.camera.x, p->y - game.camera.y, 0);
}

static void render_map(bool combat_mode) {
	rect rc = {0, 0, 640, 480 - 99};
	rectf(rc, colors::gray);
	if(ishilite(rc)) {
		if(combat_mode)
			cursor.set(INTRFACE, 251);
		else
			cursor.set(INTRFACE, 250);
	}
	const int dx = 16;
	const int dy = 12;
	switch(hot.key) {
	case KeyLeft: execute(addpoint, -dx, 0, &game.camera); break;
	case KeyRight: execute(addpoint, dx, 0, &game.camera); break;
	case KeyUp: execute(addpoint, 0, -dy, &game.camera); break;
	case KeyDown: execute(addpoint, 0, dy, &game.camera); break;
	}
	scrollmap(-1, -1, 270);
	scrollmap(0, -1, 271);
	scrollmap(1, -1, 272);
	scrollmap(1, 0, 273);
	scrollmap(1, 1, 274);
	scrollmap(0, 1, 275);
	scrollmap(-1, 1, 276);
	scrollmap(-1, 0, 277);
	render_tiles();
	prepare_objects();
	render_objects();
	if(ishilite(rc) && istips() && istipsonetime()) {
		auto pt = hot.mouse + game.camera;
		game.add("Area %1i, %2i", pt.x, pt.y);
	}
}

static variant choose_target() {
	openform();
	screenshoot screen;
	while(ismodal()) {
		screen.restore();
		cursor.set(INTRFACE, 251);
		domodal();
		switch(hot.key) {
		case MouseLeft:
		case MouseLeftDBL:
			if(hot.pressed)
				breakmodal(hilite_object);
			break;
		case MouseRight:
			breakmodal(0);
			break;
		case InputKeyUp:
			breakmodal(0);
			break;
		default:
			break;
		}
	}
	closeform();
	return getresult();
}

static void use_item() {
	//choose_target();
	setstage(game.combat);
}

static void endcombat() {
	setstage(game.play);
}

static void change_item() {
}

static void change_item_action() {
	game.getplayer().setactionindex(game.getplayer().getactionindex() + 1);
}

static void open_inventory() {
	game.getplayer().inventory();
}

static void open_map() {
}

static void quit_game() {
	setstage(game.mainmenu);
}

static fnevent choose_option() {
	answers an;
	an.add(0, variant(Save));
	an.add(0, variant(Load));
	an.add(0, variant(Options));
	an.add((int)quit_game, variant(Quit));
	return (fnevent)an.choose();
}

static void open_options() {
	auto p = choose_option();
	if(p)
		p();
}

static void open_skills() {
	auto& player = game.getplayer();
	auto skill = player.chooseskill();
	if(!skill)
		return;
	auto target = choose_target();
}

static void open_charsheet() {
	game.getplayer().charsheet();
}

static void horizontal(int& x, int y, int d, sprite* ps, int frame, int count) {
	for(auto i = 0; i < count; i++) {
		image(x, y, ps, frame, 0);
		x += d;
	}
}

static void render_ap(int x, int y, int green, int red, int yellow) {
	auto ps = gres(INTRFACE);
	if(!ps)
		return;
	const int w = 9;
	horizontal(x, y, w, ps, ps->ganim(83, 0), green);
	horizontal(x, y, w, ps, ps->ganim(85, 0), yellow);
	horizontal(x, y, w, ps, ps->ganim(84, 0), red);
}

static void render_item(int x, int y, bool combat_mode) {
	auto ps = gres(INTRFACE);
	if(!ps)
		return;
	auto& player = game.getplayer();
	auto fr = ps->ganim(32, 0);
	auto& pf = ps->get(fr);
	rect rc = {x, y, x + pf.sx - 4, y + pf.sy - 4};
	if(buttonf(rc.x1, rc.y1, 32, 31, 0, false)) {
		if(combat_mode) {
		} else
			execute(use_item);
	}
	auto weapon = player.getweapon();
	auto a = ishilite(rc);
	if(hot.mouse.in(rc) && hot.pressed) {
		rc.y1 -= 2;
		rc.y2 -= 2;
		rc.x1 += 1;
		rc.x2 += 1;
	}
	gear(rc.x1, rc.y1, rc.width(), rc.height(), weapon, false);
	auto action = player.getaction();
	if(action) {
		image(rc.x1 + rc.width() - 30, rc.y1 + 16,
			ps, ps->ganim(bsdata<actioni>::elements[action.type].avatar, getuitime()), 0);
		if(action.aimed)
			image(rc.x1 + rc.width() - 10, rc.y1 + 36,
				ps, ps->ganim(288, getuitime()), 0);
		if(a && hot.key == MouseRight && !hot.pressed)
			execute(change_item_action);
		image(rc.x1 + 16, rc.y1 + 56, ps, ps->ganim(289, 0), 0);
		numberap(rc.x1 + 30, rc.y1 + 45, 1, action.ap);
	}
}

static void render_actions(bool combat_mode = false) {
	auto ps = gres(INTRFACE);
	if(!ps)
		return;
	auto fr = ps->ganim(16, 0);
	auto dy = ps->get(fr).sy;
	auto x = 0, y = getheight() - dy;
	image(x, y, ps, fr, ImageNoOffset);
	if(buttonf(x + 210, y + 40, 47, 46, 'I', false))
		execute(open_inventory);
	if(buttonf(x + 210, y + 60, 18, 17, KeyEscape, false))
		execute(open_options);
	render_item(x + 265, y + 29, combat_mode);
	if(buttonf(x + 526, y + 38, 13, 10, 'M', false))
		execute(open_map);
	if(buttonf(x + 526, y + 58, 57, 56, 'C', false))
		execute(open_charsheet);
	if(buttonf(x + 526, y + 78, 59, 58, 'P', false))
		execute(game.pipboy);
	if(buttonf(x + 218, y + 6, 6, 7, 'W', false))
		execute(change_item);
	if(buttonf(x + 523, y + 6, 6, 7, 'S', false))
		execute(open_skills);
	numbersm(x + 472, y + 39, 4, game.getplayer().get(HP), 0);
	numbersm(x + 472, y + 76, 4, game.getplayer().get(AC), 1);
	if(combat_mode) {
		render_ap(x + 317, y + 19, 8, 0, 1);
		auto pc = ps->gcicle(104);
		image(608, 477, ps, ps->ganim(104, pc->count - 1), 0);
		image(608, 477, ps, ps->ganim(109, 0), 0);
		if(buttonf(590, 424, 105, 106, 'E', false)) {
		}
		if(buttonf(590, 446, 107, 108, KeyEnter, false))
			execute(endcombat);
	}
	consoleview(x + 26, y + 32);
}

void gamei::play() {
	openform();
	while(ismodal()) {
		render_map(false);
		render_actions();
		domodal();
		update_objects();
	}
	closeform();
}

void gamei::combat() {
	animate(608, 477, gres(INTRFACE), 104, 10);
	openform();
	while(ismodal()) {
		render_map(true);
		render_actions(true);
		domodal();
	}
	closeform();
	animate(608, 477, gres(INTRFACE), 104, 10, -2, -1);
}