#include "draw.h"
#include "main.h"
#include "screenshoot.h"

using namespace draw;

const int			stx = tile_width / 5; // 16 - Ширина юнита тайла. Каждый тайл имеет размер в 5х3 юнита.
const int			sty = tile_height / 3; // 12 - Высота юнита тайла. Каждый тайл имеет размер в 5х3 юнита.

static drawable*	hilite_object;
static drawablea	objects;
static adat<drawable, 1024> drawables;
static indext		current_hexagon;
static indext		current_tile;
static bool			show_roof = true;
static bool			show_center = false;

// Получение координаты тайла(x,y) на экране
point draw::t2s(point v) {
	return {
		(short)(32 * v.y + 48 * v.x),
		(short)(24 * v.y - 12 * v.x)
	};
}

// Определение координаты тайла по координатам(x,y) на экране
point draw::s2t(point pt) {
	int x = pt.x + 40;
	int y = pt.y + 26;
	return{(short)((x - 4 * y / 3) / 64), (short)((x + 4 * y) / 128)};
}

// Получение координаты хексагона(x,y) на экране
point draw::h2s(point v) {
	return{
		(short)(16 * (v.y + v.x * 2 - v.x / 2)),
		(short)(12 * (v.y - v.x / 2))
	};
	//return{
	//	(short)(stx * (y + x + x / 2) - stx / 2),
	//	(short)(sty * (y - x + x / 2) - sty / 2)
	//};
}

point draw::h2s(indext v) {
	return h2s({(short)loc.gethx(v), (short)loc.gethy(v)});
}

// Определение координаты хексагона
point draw::s2h(point pt) {
	int x1 = pt.x + 15;
	int y1 = pt.y + 7;
	int nx;
	if(x1 < 0)
		nx = (x1 + 1) / 16 - 1;
	else
		nx = x1 / 16;
	int ny;
	if(y1 < 0)
		ny = (y1 + 1) / 12 - 1;
	else
		ny = y1 / 12;
	if(iabs(nx) % 2 != iabs(ny) % 2)
		nx--;
	long xhBase = 16 * nx;
	long yhBase = 12 * ny;
	int x = (4 * yhBase - 3 * xhBase) / 96;
	int y = yhBase / 12 - x / 2;
	return{(short)-x, (short)y};
	//int x1 = pt.x + stx / 2;
	//int y1 = pt.y;
	//int x0 = 4800;
	//int nx;
	//if(x1 - x0 < 0)
	//	nx = (x1 - x0 + 1) / stx - 1;
	//else
	//	nx = (x1 - x0) / stx;
	//int ny;
	//if(y1 < 0)
	//	ny = (y1 + 1) / sty - 1;
	//else
	//	ny = y1 / sty;
	//if(iabs(nx) % 2 != iabs(ny) % 2)
	//	nx--;
	//long xhBase = x0 + stx * nx;
	//long yhBase = sty * ny;
	//int x = (4 * yhBase - 3 * (xhBase - x0)) / 96;
	//int y = yhBase / 12 - x / 2;
	//x = 150 - x;
	//y = y + 76;
	//return{(short)x, (short)y};
}

static void addpoint() {
	auto p = (point*)hot.object;
	p->x += hot.param;
	p->y += hot.param2;
}

static void redraw_floor() {
	auto ps = gres(TILES);
	if(!ps)
		return;
	auto tm = getuitime();
	rect rc = {-tile_width, -tile_height, 640 + tile_width, 480 + tile_height};
	int xx = 8 - game.camera.x;
	int yy = 26 - game.camera.y;
	for(auto y = 0; y < 100; y++) {
		auto prev_xx = xx;
		auto prev_yy = yy;
		for(auto x = 0; x < 100; x++) {
			point pt = {(short)xx, (short)yy};
			if(pt.in(rc)) {
				auto tv = loc.getfloor(loc.geti(x, y));
				if(tv > 1)
					draw::image(xx, yy, ps, ps->ganim(tv, tm), 0);
			}
			xx += 48;
			yy -= 12;
		}
		xx = prev_xx + 32;
		yy = prev_yy + 24;
	}
}

static void redraw_roof() {
	auto ps = gres(TILES);
	if(!ps)
		return;
	auto tm = getuitime();
	rect rc = {-tile_width, -tile_height, 640 + tile_width, 480 + tile_height};
	int xx = 8 - game.camera.x;
	int yy = 26 - game.camera.y - 96;
	for(auto y = 0; y < 100; y++) {
		auto prev_xx = xx;
		auto prev_yy = yy;
		for(auto x = 0; x < 100; x++) {
			point pt = {(short)xx, (short)yy};
			if(pt.in(rc)) {
				auto tv = loc.getroof(loc.geti(x, y));
				if(tv > 1)
					draw::image(xx, yy, ps, ps->ganim(tv, tm), 0);
			}
			xx += 48;
			yy -= 12;
		}
		xx = prev_xx + 32;
		yy = prev_yy + 24;
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

static void add_drawable(const rect& rc, indext position, res_s rid, int cicle) {
	if(position == Blocked)
		return;
	auto pt = h2s({(short)loc.gethx(position), (short)loc.gethy(position)});
	if(!pt.in(rc))
		return;
	auto p = drawables.add();
	p->x = pt.x;
	p->y = pt.y;
	p->set(rid, cicle);
	objects.add(p);
}

static void prepare_objects() {
	auto size = 160;
	rect rc = {-size, -size, 640 + size, 480 + size};
	rc.move(game.camera.x, game.camera.y);
	drawables.clear();
	objects.clear();
	for(auto& e : bsdata<creaturei>()) {
		if(e && e.in(rc))
			objects.add(&e);
	}
	for(auto& e : game.players) {
		if(e && e.in(rc))
			objects.add(&e);
	}
	for(auto& e : bsdata<mapobject>()) {
		if(e && e.in(rc))
			objects.add(&e);
	}
	for(auto& e : bsdata<itemground>()) {
		if(e)
			add_drawable(rc, e.position, ITEMS, e.geti().avatar.ground);
	}
	objects.sortz();
}

static void redraw_objects() {
	hilite_object = 0;
	auto mouse = hot.mouse + game.camera;
	for(auto p : objects) {
		auto x = p->x - game.camera.x;
		auto y = p->y - game.camera.y;
		p->paint(x, y, 0);
		if(show_center)
			marker(x, y);
		if(p->hittest(mouse))
			hilite_object = p;
	}
}

static void redraw_hexagon() {
	if(current_hexagon == Blocked)
		return;
	if(isactionmode())
		return;
	auto x = loc.gethx(current_hexagon);
	auto y = loc.gethy(current_hexagon);
	current_tile = loc.geti(x / 2, y / 2);
	auto pt = h2s({(short)x, (short)y}) - game.camera;
	image(pt.x - 15, pt.y - 10, INTRFACE, 1);
}

static void chat_creature() {

}

static void control_map() {
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
}

static void set_hexagon_position() {
	auto pt = s2h(hot.mouse + game.camera);
	current_hexagon = loc.geth(pt.x, pt.y);
}

static void redraw_map() {
	redraw_floor();
	redraw_hexagon();
	prepare_objects();
	redraw_objects();
	if(show_roof)
		redraw_roof();
}

static void control_hilite() {
	if(setactionmode()) {
		if(hilite_object) {
			addaction(drawable::getnamefn);
			addaction(Talk, chat_creature, hilite_object, 0);
			addaction(Look, chat_creature, hilite_object, 0);
			addaction(UseItem, chat_creature, hilite_object, 0);
			addaction(UseSkill, chat_creature, hilite_object, 0);
			addaction(NoAction, chat_creature, 0, 0);
		}
	} else
		cursor.set(None, 0);
}

static drawable* choose_target() {
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
				breakmodal((int)hilite_object);
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
	return (drawable*)getresult();
}

static void use_item() {
	//choose_target();
	setstage(game.combat);
}

static void endcombat() {
	setstage(game.play);
}

static void change_item() {
	auto& player = game.getplayer();
	if(player.getweapon()) {
		player.setanimate(AnimateWeaponTakeOff);
		player.wait();
	}
	player.changeitems();
	if(player.getweapon()) {
		player.setanimate(AnimateWeaponTakeOn);
		player.wait();
	}
}

static void change_item_action() {
	game.getplayer().setactionindex(game.getplayer().getactionindex() + 1);
}

static void open_inventory() {
	game.getplayer().inventory();
}

static void open_map() {
	game.automap();
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

static void redraw_ap(int x, int y, int green, int red, int yellow) {
	auto ps = gres(INTRFACE);
	if(!ps)
		return;
	const int w = 9;
	horizontal(x, y, w, ps, ps->ganim(83, 0), green);
	horizontal(x, y, w, ps, ps->ganim(85, 0), yellow);
	horizontal(x, y, w, ps, ps->ganim(84, 0), red);
}

static void redraw_item(int x, int y, bool combat_mode) {
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

static void redraw_actions(bool combat_mode = false) {
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
	redraw_item(x + 265, y + 29, combat_mode);
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
		redraw_ap(x + 317, y + 19, 8, 0, 1);
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
	static rect play_area = {0, 0, 640, 480 - 99};
	openform();
	while(ismodal()) {
		current_hexagon = Blocked;
		if(hot.mouse.in(play_area))
			set_hexagon_position();
		redraw_map();
		redraw_actions();
		if(hot.mouse.in(play_area))
			control_hilite();
		control_map();
		domodal();
		game.update();
		switch(hot.key) {
		case MouseLeft:
			if(!isactionmode() && current_hexagon != Blocked && hot.pressed)
				game.getplayer().moveto(current_hexagon);
			break;
		case 'G': show_center = !show_center; break;
		case 'U':
			game.getplayer().doanimate(AnimateUse);
			break;
		case 'D':
			game.getplayer().doanimate(AnimateDodge);
			break;
		case 'K':
			game.getplayer().doanimate(AnimateKnockOutForward);
			break;
		}
	}
	closeform();
}

void gamei::combat() {
	animate(608, 477, gres(INTRFACE), 104, 10);
	openform();
	while(ismodal()) {
		rect rc = {0, 0, 640, 480 - 99};
		if(ishilite(rc))
			cursor.set(INTRFACE, 250);
		redraw_map();
		redraw_actions(true);
		domodal();
	}
	closeform();
	animate(608, 477, gres(INTRFACE), 104, 10, -2, -1);
}

void actor::wait() {
	auto current_action = animate;
	auto stop = frame_stop;
	while(ismodal()) {
		cursor.set(INTRFACE, 295);
		if(stop == frame && next_stamp >= game.getaitime())
			break;
		current_hexagon = Blocked;
		redraw_map();
		redraw_actions();
		game.update();
		if(current_action != animate)
			break;
		domodal();
	}
}

void add_util_info(stringbuilder& sb, int mode, int value);

static void redraw_actions_editor(int tile_frame, int wall_frame, int scenery_frame, int mode) {
	char temp[1024]; stringbuilder sb(temp);
	auto pt = hot.mouse + game.camera;
	if(current_hexagon != Blocked) {
		auto x = loc.gethx(current_hexagon);
		auto y = loc.gethy(current_hexagon);
		sb.addn("Hex %1i, %2i (%3i)", x, y, current_hexagon);
		sb.addn("Tile %1i, %2i (%3i)", loc.getx(current_tile), loc.gety(current_tile), current_tile);
	}
	sb.addn("Objects %1i", objects.getcount());
	switch(mode) {
	case 0: add_util_info(sb, mode, tile_frame); break;
	case 1: add_util_info(sb, mode, scenery_frame); break;
	case 2: add_util_info(sb, mode, wall_frame); break;
	}
	textf(4, 4, 300, temp);
}

short unsigned choose_wall(short unsigned start, int& mode);
short unsigned choose_tile(short unsigned start, int& mode);

static void apply_frame(int mode, int tile_frame, int scenery_frame, int wall_frame) {
	switch(mode) {
	case 0:
		if(current_tile != Blocked)
			loc.setfloor(current_tile, tile_frame);
		break;
	case 1:
		if(current_hexagon != Blocked)
			loc.add(current_hexagon, SCENERY, scenery_frame);
		break;
	case 2:
		if(current_hexagon != Blocked)
			loc.add(current_hexagon, WALLS, wall_frame);
		break;
	}
}

static short unsigned choose_scenery(short unsigned start, int& mode) {
	openform();
	auto ps = gres(SCENERY);
	auto origin = 0;
	const int dx = 80;
	const int mx = 8;
	const int dy = 80;
	const int my = 5;
	int current = start;
	while(ismodal()) {
		rectf({0, 0, 640, 480}, colors::gray);
		if(current < 0)
			current = 0;
		if(current > (int)ps->cicles - 1)
			current = ps->cicles - 1;
		if(current < origin)
			origin = (current / mx) * mx;
		if(current >= origin + my * mx)
			origin = (current / mx) * mx - (my - 1) * mx;
		if(origin < 0)
			origin = 0;
		auto index = origin;
		for(auto y = dy; y <= dy * my; y += dy) {
			for(auto x = dx / 2; x < dx * mx; x += dx) {
				image(x, y - 32, ps, ps->ganim(index, getuitime() / 100), 0);
				if(current == index)
					rectb({x - dx / 2, y - dy, x + dx / 2, y}, colors::red);
				index++;
			}
		}
		domodal();
		switch(hot.key) {
		case KeyEnter: mode = 1; breakmodal(current); break;
		case KeyEscape: breakmodal(start); break;
		case KeyLeft: current--; break;
		case KeyRight: current++; break;
		case KeyUp: current -= mx; break;
		case KeyDown: current += mx; break;
		}
	}
	closeform();
	return getresult();
}

void areai::editor() {
	openform();
	int wall_frame = 1, scenery_frame = 2, tile_frame = 3, mode = 0;
	while(ismodal()) {
		rectf({0, 0, 640, 480}, colors::gray);
		set_hexagon_position();
		redraw_map();
		control_hilite();
		control_map();
		redraw_actions_editor(tile_frame, wall_frame, scenery_frame, mode);
		domodal();
		switch(hot.key) {
		case KeyDelete:
			apply_frame(mode, 0, 0, 0);
			break;
		case MouseLeft:
			if(hot.pressed && current_hexagon!=Blocked)
				apply_frame(mode, tile_frame, scenery_frame, wall_frame);
			break;
		case KeySpace:
			apply_frame(mode, tile_frame, scenery_frame, wall_frame);
			break;
		case 'C':
			if(current_tile != Blocked)
				loc.correct(current_tile);
			//loc.correct();
			break;
		case 'A':
			loc.correct();
			break;
		case 'G': show_center = !show_center; break;
		case 'R': show_roof = !show_roof; break;
		case 'T': tile_frame = choose_tile(tile_frame, mode); break;
		case 'S': scenery_frame = choose_scenery(scenery_frame, mode); break;
		case 'W': wall_frame = choose_wall(wall_frame, mode); break;
		case F5: loc.write("test"); break;
		case F6: loc.read("test"); break;
		}
	}
	closeform();
}