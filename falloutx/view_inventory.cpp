#include "draw.h"
#include "main.h"
#include "screenshoot.h"

using namespace draw;

static item*		hilite_item;
static item*		item_info;
static char			chat_information[1024];

static void look_item() {
	item_info = (item*)hot.object;
}

static void drop_item() {
}

static void use_item() {
}

static void unload_item() {
}

static void itemtext(int x, int y, int width, int height, const item& it) {
	char temp[260]; stringbuilder sb(temp);
	rect rc = {x, y, x + width, y + height};
	sb.addn(it.getname());
	sb.addn("---");
	it.addtext(sb);
	textf(x, y, width, temp);
}

static void chartext(int x, int y, int width, int height, creaturei& player) {
	char temp[260]; stringbuilder sb(temp);
	static variant values[] = {HP, AC, Phisycal, Laser, Fire, Plasma, Explosive};
	rect rc = {x, y, x + width, y + height};
	sb.addn(player.getname());
	sb.addn("---");
	for(auto id = Strenght; id <= Luck; id = (stat_s)(id + 1))
		sb.addn("%1\t%2i", variant(id).getnameabr(), player.get(id));
	sb.addn("---");
	player.add(sb, player.getweapon(0));
	sb.addn("---");
	player.add(sb, player.getweapon(1));
	textf(x, y, width, temp, 0, 0, 0, 0, 22);
	sb.clear();
	for(auto id : values) {
		if(id.type == Damage)
			sb.addn("%1\t%2i/%3i%%", id.getnameabr(), player.getthreshold((damage_s)id.value), player.getresist((damage_s)id.value));
		else if(id.type == Stat && id.value == HP)
			sb.addn("%1\t%2i/%3i", id.getnameabr(), player.get(id), player.get(HP));
		else
			sb.addn("%1\t%2i", id.getnameabr(), player.get(id));
	}
	textf(x + 48, y + texth() * 2, width - 40, temp, 0, 0, 0, 0, 52);
	sb.clear(); sb.add("Общий вес %1i/%2i", 0, player.get(CarryWeight));
	text(rc.x1 + (rc.width() - draw::textw(temp)) / 2, rc.y2 - texth(), temp);
}

static int gear(int x, int y, int sx, int sy, item& it) {
	const int dw = 2;
	rect rc = {x, y, x + sx, y + sy};
	auto a = ishilite(rc);
	if(a)
		hilite_item = &it;
	if(!it)
		return 0;
	gear(rc.x1 + dw, rc.y1 + dw - 1, rc.width() - dw * 2, rc.height() - dw * 2, it, true);
	auto c1 = it.getcount();
	if(c1 > 1) {
		char temp[16]; stringbuilder sb(temp);
		state push;
		setcolor(ColorCheck);
		sb.add("x%1i", c1);
		draw::text(rc.x1 + 4, rc.y1 + 4, temp);
	}
	return rc.height();
}

void itema::paint(const rect& rc, int& origin, int mode, point pts) {
	// Correction
	const int element_height = 50 * rc.width() / 70;
	auto count_per_page = rc.height() / element_height;
	if(origin + count_per_page >= (int)count)
		origin = count - count_per_page;
	if(origin < 0)
		origin = 0;
	// Set cliping and colors
	state push;
	setcolor(ColorText);
	//draw::rectb(rc);
	// Mouse input
	auto a = ishilite(rc);
	if(a) {
		switch(hot.key) {
		case MouseWheelDown: execute(setint, origin + 1, 0, &origin); break;
		case MouseWheelUp: execute(setint, origin - 1, 0, &origin); break;
		default: break;
		}
		hilite_item = data[0];
	}
	if(mode == 1) {
		if(buttonf(pts.x, pts.y, 49, 50, KeyUp, false))
			execute(setint, origin - 1, 0, &origin);
		if(buttonf(pts.x, pts.y + 20, 51, 52, KeyDown, false))
			execute(setint, origin + 1, 0, &origin);
	} else if(mode == 2) {
		if(buttonf(pts.x, pts.y, 100, 101, 0, false))
			execute(setint, origin - 1, 0, &origin);
		if(buttonf(pts.x, pts.y + 25, 93, 94, 0, false))
			execute(setint, origin + 1, 0, &origin);
	}
	// Show elements
	setclip({rc.x1, rc.y1, rc.x2, rc.y2});
	auto x = rc.x1, y = rc.y1, width = rc.width();
	for(auto i = origin; i < (int)count; i++) {
		if(y >= rc.y2)
			break;
		if(gear(x, y, width, element_height - 1, *data[i]))
			y += element_height;
		//line(x, y, x + width, y, colors::red);
	}
}

static void render_inventory(creaturei& player) {
	hilite_item = 0;
	int x, y, sx, sy;
	static int origin;
	window(x, y, sx, sy, 48, -48);
	player.preview(x + 206, y + 116, player.getgender(), player.getarmor(), player.getweapon(), getuitime() / 300);
	if(isactionmode() && item_info)
		itemtext(x + 295, y + 48, 148, 180, *item_info);
	else
		chartext(x + 295, y + 48, 148, 180, player);
	gear(x + 152, y + 184, 92, 60, player.getarmor());
	gear(x + 152, y + 287, 92, 60, player.getweapon(0));
	gear(x + 245, y + 287, 92, 60, player.getweapon(1));
	itema source;
	player.getitems(source);
	source.sort();
	source.paint({x + 42, y + 40, x + 42 + 70, y + 40 + 300}, origin, 1, {(short)(x + 42 + 80), (short)(y + 40 - 4)});
	if(buttonf(x + 432, y + 322, 299, 300, KeyEscape, false))
		buttoncancel();
	if(isactionmode() && hilite_item) {
		addaction(Look, look_item, hilite_item, 0, item::getobjectname);
		addaction(Drop, drop_item, hilite_item, 0);
		if(hilite_item->isweapon() && hilite_item->getclipcount())
			addaction(Unload, unload_item, hilite_item, 0);
		addaction(NoAction, 0, 0, 0);
	}
	consoleview();
}

static item* choose_drag_target(creaturei& player, screenshoot& screen, item& source) {
	openform();
	while(ismodal()) {
		screen.restore();
		render_inventory(player);
		cursor.set(INVEN, source.geti().avatar.inventory, {0, 32});
		domodal();
		if(!hot.pressed)
			breakmodal((int)hilite_item);
	}
	closeform();
	return (item*)getresult();
}

void creaturei::inventory() {
	screenshoot screen;
	setactionmode(false);
	openform();
	while(ismodal()) {
		screen.restore();
		cursor.set(INTRFACE, 286);
		render_inventory(*this);
		domodal();
		switch(hot.key) {
		case MouseRight:
			if(hot.pressed && !isactionmode())
				setactionmode(true);
			break;
		case MouseLeft:
			if(!isactionmode() && hot.pressed && hilite_item) {
				auto source = hilite_item;
				auto element = *hilite_item;
				if(!element)
					break;
				source->addcount(-1);
				element.setcount(1);
				update();
				reanimate();
				auto target = choose_drag_target(*this, screen, element);
				if(target) {
					bool run = true;
					if(target == &getarmor() && !element.isarmor())
						run = false;
					if(run && !target->join(element))
						additem(element);
				}
				if(element)
					source->join(element);
				update();
				reanimate();
			}
			break;
		}
	}
	closeform();
}

static void inform(const char* format, ...) {
	if(!format)
		return;
	stringbuilder sb(chat_information);
	sb.addv(format, xva_start(format));
}

static void look_trade_item() {
	char temp[1024]; stringbuilder sb(temp);
	auto p = (item*)hot.object;
	sb.add("Это %1.", p->getname());
	p->addtext(sb);
	inform(temp);
}

void creaturei::trade(creaturei& opponent) {
	char temp[260];
	stringbuilder sb(temp);
	openform();
	screenshoot push;
	auto player_origin = 0, player_trade_origin = 0;
	auto opponent_origin = 0, opponent_trade_origin = 0;
	itema source;
	wearable player_trade;
	wearable opponent_trade;
	auto player_barter = get(Barter);
	auto opponent_barter = opponent.get(Barter);
	auto difference = opponent_barter - player_barter;
	if(difference < -75)
		difference = -75;
	else if(difference > 75)
		difference = 75;
	auto player_mult = 100 - difference;
	auto opponent_mult = 250 + difference;
	auto dlg = "barter";
	int opponent_act;
	int text_origin = 0, text_maximum = 0;
	inform(opponent.getspeech(dlg, 1));
	setactionmode(false);
	while(ismodal()) {
		opponent_act = 0;
		hilite_item = 0;
		push.restore();
		auto x = 0, y = 290;
		background(103);
		image(x, y, INTRFACE, 111, ImageNoOffset);
		preview(47, 404, getgender(), getarmor(), getweapon(), 2);
		if(chat_information[0]) {
			rect rc = {140, 232, 140 + 370, 232 + 45};
			state push; setclip(rc);
			scrollup(rc, text_origin, text_maximum, chat_information);
		}
		source.clear(); getitems(source); source.sort();
		source.paint({108, 328, 108 + 64, 328 + 136}, player_origin, 2, {190, 346});
		source.clear(); player_trade.getitems(source); source.sort();
		source.paint({244, 318, 244 + 64, 318 + 136}, player_trade_origin, 1, {202, 406});
		auto player_cost = source.getcost();
		player_cost = player_cost * player_mult / 100;
		preview(590, 404, opponent.getgender(), opponent.getarmor(), opponent.getweapon(), 3);
		source.clear(); opponent.getitems(source); source.sort();
		source.paint({460, 328, 460 + 64, 328 + 136}, opponent_origin, 2, {421, 346});
		source.clear(); opponent_trade.getitems(source); source.sort();
		source.paint({332, 318, 332 + 64, 318 + 136}, opponent_trade_origin, 1, {416, 406});
		auto opponent_cost = source.getcost();
		opponent_cost = opponent_cost * opponent_mult / 100;
		auto old_fore = fore;
		setcolor(ColorCheck);
		sb.clear(); sb.add("$%1i", player_cost);
		text(272 - textw(temp) / 2, 462, temp);
		sb.clear(); sb.add("$%1i", opponent_cost);
		text(360 - textw(temp) / 2, 462, temp);
		fore = old_fore;
		if(buttonf(40, 452, 96, 95, 'T', false)) {
			if(player_cost == 0 && opponent_cost == 0)
				execute(buttoncancel);
			else if(player_cost < opponent_cost)
				execute(setint, 20, 0, &opponent_act);
			else
				execute(buttonok);
		}
		if(buttonf(583, 452, 96, 95, KeyEscape, false))
			execute(buttoncancel);
		if(isactionmode() && hilite_item) {
			addaction(Look, look_trade_item, hilite_item, 0);
			addaction(NoAction, 0, 0, 0);
		}
		domodal();
		if(opponent_act)
			inform(opponent.getspeech(dlg, opponent_act));
		switch(hot.key) {
		case MouseLeft:
			if(!isactionmode() && hilite_item) {
				if(hot.pressed) {
					inform(hilite_item->geti().txt.text);
					if(isgear(hilite_item))
						player_trade.additem(*hilite_item);
					else if(opponent.isgear(hilite_item))
						opponent_trade.additem(*hilite_item);
					else if(player_trade.isgear(hilite_item))
						additem(*hilite_item);
					else if(opponent_trade.isgear(hilite_item))
						opponent.additem(*hilite_item);
				}
			}
			break;
		case MouseRight:
			if(hot.pressed && !isactionmode())
				setactionmode(true);
			break;
		}
	}
	push.restore();
	if(getresult()) {
		additems(opponent_trade);
		opponent.additems(player_trade);
	} else {
		additems(player_trade);
		opponent.additems(opponent_trade);
	}
	closeform();
}