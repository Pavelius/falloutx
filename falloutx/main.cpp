#include "draw.h"
#include "main.h"

using namespace draw;

void main_util();

static void load_game() {
	openform();
	while(ismodal()) {
		rectf({0, 0, getwidth(), getheight()}, colors::gray.darken());
		textf(100, 100, 400, "Будильник\n---\n* Что не так?.\n* Еще один пукт.\n\n[#Подчеркнутый текст]");
		domodal();
		if(hot.key == KeyEscape)
			breakmodal(0);
	}
	closeform();
}

static void show_invertory() {
	message("Пока данная фича не реализовано. Ожидайте дальнейших обновлений. Следите за релизами.");
}

extern char creature_animations[][3];

static void getpresent(stringbuilder& sb, animate_s v) {
	static const char* weapon_names[] = {"Knife", "Club", "Hammer", "Spear", "Pistol", "SMG", "Rifle", "Heavy Gun", "Machine Gun", "Rocket Launcher"};
	auto w = -1;
	auto b = actor::getbaseanimate(v, &w);
	sb.addn("animate #%1i %2", b, bsdata<animationi>::elements[b].txt.id);
	if(w != -1)
		sb.adds(weapon_names[w]);
	sb.adds(creature_animations[v]);
}

struct wallobj {
	texti							txt;
	short unsigned					start;
	std::initializer_list<point>	points;
	void paint(point base, point camera, bool show_center) const {
		auto ps = gres(WALLS);
		auto index = start;
		for(auto e : points) {
			auto pt = m2h(e.x + base.x, e.y + base.y) - camera;
			image(pt.x, pt.y, ps, ps->ganim(index, 0), 0);
			if(show_center) {
				line(pt.x - 4, pt.y, pt.x + 4, pt.y, colors::red);
				line(pt.x, pt.y - 4, pt.x, pt.y + 4, colors::red);
			}
			index++;
		}
	}
};

BSDATA(wallobj) = {
	{{"TableSE"}, 1, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 5}, {3, 4}, {4, 5}, {5, 4}}},
	{{"TableES"}, 11, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {5, 2}, {5, 3}, {5, 4}, {6, 4}}},
	{{"CaveWallW"}, 24, {{0, 0}, {1, -1}}},
	{{"CaveWallW1"}, 26, {{0, 0}, {-1, -1}, {-2, 0}, {-3, -1}}},
	{{"CaveWallW2"}, 30, {{0, 0}, {1, -1}, {2, 0}, {3, -1}}},
	{{"CaveWallNE"}, 34, {{0, 0}, {0, 1}, {1, 1}, {2, 2}, {3, 1}, {3, 3}}},
	{{"CaveWallE0"}, 40, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
	{{"CaveWallE1"}, 44, {{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
	{{"CaveWallС1"}, 48, {{0, 0}, {0, 1}}},
	{{"CaveWallС2"}, 50, {{0, 0}, {0, 1}}},
	{{"CaveWallCL1"}, 52, {{0, 0}, {2, 0}, {2, 1}}},
	{{"CaveWallCL2"}, 55, {{0, 0}, {0, 1}, {0, -2}}},
	{{"CaveWallCL3"}, 58, {{0, 0}, {1, 0}}},
	{{"CaveWallCL4"}, 60, {{0, 0}, {1, 0}, {2, 1}, {3, 1}}},
	{{"CaveWallCL4"}, 64, {{0, 0}, {1, 0}, {2, 1}}},
	{{"CaveWall5"}, 67, {{0, 0}, {1, -1}, {2, 0}}},
	{{"CaveWallC1"}, 70, {{0, 0}, {0, 1}, {2, 0}, {2, 1}}},
	{{"CaveWallNL"}, 74, {{0, 0}, {1, -1}, {2, 0}, {3, -1}}},
	{{"CaveWallNL"}, 78, {{0, 0}, {1, -1}}},
	{{"CaveWallNL"}, 80, {{0, 0}, {0, -1}}},
	{{"CaveWallNL"}, 82, {{0, 0}, {0, -1}}},
	{{"CaveWallNL"}, 84, {{0, 0}}},
	{{"CaveWallNL"}, 85, {{0, 0}}},
	{{"CaveWallNL"}, 86, {{0, 0}}},
	{{"CaveWallNL"}, 87, {{0, 0}, {1, 0}}},
	{{"CaveWallNL"}, 89, {{0, 0}, {1, 0}}},
	{{"CaveWallNL"}, 91, {{0, 0}}},
	{{"CaveColumn"}, 92, {{0, 0}}},
	{{"CaveColumn"}, 93, {{0, 0}}},
	{{"CaveBrick"}, 95, {{0, 0}, {1, 0}, {0, -1}}},
	{{"CaveCorner"}, 98, {{0, 0}}},
	{{"CaveCorner"}, 99, {{0, 0}}},
	{{"CaveCorner"}, 100, {{0, 0}}},
	{{"CaveCorner"}, 101, {{0, 0}}},
	{{"HanharWest"}, 102, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {1, 10}, {2, 10}, {2, 11}, {3, 10}, {3, 11}, {4, 11}}},
	{{"HangerEast"}, 119, {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}}},
	{{"Car"}, 132, {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}},
	{{"CarWall2"}, 139, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15}, {0, 16}, {0, 17}, {0, 18}, {1, 17}, {2, 17}, {2, 18}}},
	{{"CarWall3"}, 161, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {1, 11}, {1, 12}, {2, 12}}},
	{{"CarWall4"}, 177, {{0, 0}, {0, 1}, {1, 0}, {2, 1}, {3, 0}, {4, 1}, {5, 0}, {6, 1}, {7, 0}, {8, 1}, {9, 0}, {10, 1}, {11, 0}, {12, 1}, {13, 0}, {14, 1}, {15, 0}}},
	{{"TrackBarell"}, 194, {{0, 0}, {1, -1}, {2, 0}, {3, -1}, {4, 0}, {5, -1}, {6, 0}, {7, -1}, {8, 0}}},
	{{"CarWall5"}, 203, {{0, 0}, {0, 1}, {1, 0}, {2, 1}, {3, 0}, {4, 1}, {5, 0}, {6, 1}, {7, 0}, {8, 0}}},
	{{"CarWall6"}, 213, {{0, 0}, {1, -1}, {1, 0}, {2, 0}}},
	{{"Fence"}, 217, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}, {13, 0}}},
	{{"Wood"}, 231, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}}},
	{{"BrickCorner"}, 243, {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 2}}},
	{{"Wood"}, 248, {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 2}}},
	{{"WallVaultDoor"}, 253, {{0, 0}, {2, 0}}},
	{{"Door"}, 255, {{0, 0}, {2, 0}}},
	{{"Door"}, 257, {{0, 0}, {2, 0}}},
	{{"WallVault"}, 259, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}}},
	{{"WallVault"}, 272, {{0, 0}, {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, 0}}},
	{{"WallVault"}, 280, {{0, 0}, {0, 1}, {1, 0}, {2, 1}, {3, 0}, {3, 0}, {4, 1}, {4, 2}}},
	{{"WallVault"}, 288, {{0, 0}, {0, 1}, {1, 0}}},
	{{"WallDoor"}, 291, {{0, 0}, {1, -1}, {3, -1}}},
	{{"VaultDoorNS"}, 294, {{0, 0}, {0, 2}, {0, 3}}},
	{{"VaultDoorWE"}, 297, {{0, 0}, {1, -1}, {3, -1}}},
	{{"VaultDoorNSH"}, 300, {{0, 0}, {0, 2}}},
	{{"WallVault"}, 302, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}}},
	{{"VaultWall"}, 312, {{0, 0}, {1, -1}, {2, 0}, {3, -1}, {4, 0}}},
	{{"VaultWall"}, 317, {{0, 0}, {0, 1}}},
	{{"Fence"}, 319, {{0, 0}, {0, -1}, {0, -2}, {2, 0}, {1, -1}, {4, 0}, {4, -3}, {4, -2}, {0, -4}, {2, -4}, {3, -4}, {4, -4}}},
	{{"Fence"}, 331, {{0, 0}, {0, -1}}},
	{{"Grid"}, 333, {{0, 0}, {0, 1}}},
	{{"Grid"}, 335, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {7, 0}, {9, 0}, {11, 0}, {13, 0}}},
	{{"Grid"}, 345, {{0, 0}, {1, 0}}},
	{{"Grid"}, 347, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}},
	{{"Grid"}, 355, {{0, 0}, {0, -1}}},
	{{"Fence"}, 357, {{0, 0}, {1, -1}, {2, 0}, {4, 0}, {0, -2}, {4, -1}}},
	{{"MetalBuilding"}, 363, {{0, 0}, {0, 1}, {0, 2}, {0, 5}, {0, 6}, {0, 7}}},
	{{"StoneDoor"}, 369, {{0, 0}, {2, 0}}},
	{{"StoneDoor"}, 371, {{0, 0}, {0, 1}}},
	{{"WoodDoor"}, 373, {{0, 0}, {2, 0}}},
	{{"WoodWall"}, 375, {{0, 0}, {1, 0}}},
	{{"WoodWall"}, 377, {{0, 0}, {1, -1}, {2, 0}, {3, -1}, {4, 0}, {5, -1}, {6, 0}}},
	{{"WoodWall"}, 384, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}}},
	{{"WoodDuche"}, 393, {{0, 0}, {2, 0}}},
	{{"WoodWall"}, 395, {{0, 0}, {1, -1}, {2, 0}, {3, -1}, {4, 0}, {5, -1}, {6, 0}, {7, -1}}},
	{{"DirtWall"}, 403, {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
	{{"WoodDoor"}, 407, {{0, 0}, {0, 2}}},
	{{"WoodDoor"}, 409, {{0, 0}, {0, 2}}},
	{{"WoodDoor"}, 411, {{0, 0}, {0, 2}}},
	{{"WoodWall"}, 413, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}}},
	{{"StoneColumn"}, 421, {{0, 0}, {1, -1}}},
	{{"StoneWall"}, 423, {{0, 0}, {2, 0}, {4, 0}, {1, -1}, {6, 0}, {8, 0}, {3, -1}}},
	{{"StoneColumn"}, 430, {{0, 0}}},
	{{"StoneColumn"}, 431, {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}}},
	{{"StoneColumn"}, 438, {{0, 0}}},
	{{"StoneRound"}, 439, {{0, 0}, {0, 1}, {1, 0}, {2, 1}}},
	{{"StoneWall"}, 443, {{0, 0}, {2, 0}, {4, 0}, {6, 0}, {8, 0}}},
	{{"WoodWall"}, 448, {{0, 0}, {1, -1}, {-1, -1}}},
	{{"StoneRound"}, 451, {{0, 0}}},
	{{"StoneWall"}, 452, {{0, 0}, {0, -1}, {0, -2}, {0, -3}, {0, -4}}},
	{{"StoneColumn"}, 457, {{0, 0}}},
	{{"StoneCorner"}, 458, {{0, 0}}},
	{{"StoneWall"}, 459, {{0, 0}, {1, 0}}},
	{{"StoneCorner"}, 461, {{0, 0}}},
	{{"StoneWall"}, 462, {{0, 0}, {0, 1}, {1, 0}, {2, 1}}},
	{{"StoneCorner"}, 466, {{0, 0}}},
	{{"BrickCorner"}, 467, {{0, 0}}},
	{{"BrickWall"}, 468, {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {5, 0}, {6, 0}, {7, 0}, {9, 0}, {10, 0}}},
	{{"BrickWall"}, 477, {{0, 0}, {0, 1}, {0, 2}, {0, 5}, {0, 4}, {0, 3}, {0, 6}}},
	{{"BrickCorner"}, 484, {{0, 0}}},
	{{"BrickCorner"}, 485, {{0, 0}}},
	{{"BrickDoor"}, 486, {{0, 0}, {2, -1}}},
	{{"BrickWindow"}, 488, {{0, 0}, {1, -1}}},
	{{"BrickWall"}, 490, {{0, 0}, {2, 0}, {3, -1}, {4, 0}, {5, -1}, {7, -1}, {8, 0}}},
	{{"BrickCorner"}, 497, {{0, 0}}},
};

extern walli walls_data[];
const char* getlightflags(short unsigned v);
const char* getmaterial(int v);

static void test_walls() {
	//static unsigned short hilite;
	//static int show_index = 0;
	//static point camera = {250, -150};
	//auto show_center = false;
	//openform();
	//auto pd = bsdata<wallobj>::elements + 5;
	//int index = 0;
	//int index_wall = 200;
	//while(ismodal()) {
	//	if(index_wall >= (int)(sizeof(bsdata<wallobj>::elements) / sizeof(bsdata<wallobj>::elements[0])))
	//		index_wall = sizeof(bsdata<wallobj>::elements) / sizeof(bsdata<wallobj>::elements[0]) - 1;
	//	if(index_wall < 0)
	//		index_wall = 0;
	//	auto pd = bsdata<wallobj>::elements + index_wall;
	//	if(index >= (int)pd->points.size())
	//		index = pd->points.size() - 1;
	//	if(index < 0)
	//		index = 0;
	//	rectf({0, 0, 640, 480}, colors::gray);
	//	pd->paint({9, 13}, camera, show_center);
	//	auto pm = h2m(hot.mouse + camera);
	//	auto pt = m2h(pm.x, pm.y) - camera;
	//	image(pt.x, pt.y, INTRFACE, 1);
	//	char temp[1024]; stringbuilder sb(temp);
	//	sb.addn("%1 count %2i, start %3i %4", pd->txt.id, pd->points.size(), pd->start, walls_data[pd->start - 1].original);
	//	for(unsigned i = 0; i < pd->points.size(); i++) {
	//		sb.add("\n");
	//		if(i == index)
	//			sb.add("[~");
	//		auto pw = walls_data + (pd->start - 1 + i);
	//		sb.add("%1i, %2i %3 %4", pd->points.begin()[i].x, pd->points.begin()[i].y, getlightflags(pw->flags), getmaterial(pw->material));
	//		if(i == index)
	//			sb.add("]");
	//	}
	//	auto pci = (point*)pd->points.begin() + index;
	//	textf(4, 4, 640 / 2, temp);
	//	domodal();
	//	switch(hot.key) {
	//	case 'G': show_center = !show_center; break;
	//	case KeyUp: index--; break;
	//	case KeyDown: index++; break;
	//	case KeyLeft: index_wall--; break;
	//	case KeyRight: index_wall++; break;
	//	case 'W': pci->y--; break;
	//	case 'S': pci->y++; break;
	//	case 'Q': pci->x++; break;
	//	case 'A': pci->x--; break;
	//	}
	//}
	//closeform();
}

static void test_tiles() {
	//point camera = {0, 0};
	//static unsigned short hilite;
	//static int show_index = 0;
	//openform();
	//while(ismodal()) {
	//	char temp[128]; stringbuilder sb(temp);
	//	auto ps = gres(TILES);
	//	if(!ps)
	//		return;
	//	auto tm = getuitime();
	//	const auto count = ps->cicles;
	//	const auto max_width = 16;
	//	auto pt = s2m(hot.mouse + camera);
	//	if(pt.x < 0 || pt.y<0 || pt.x > max_width)
	//		hilite = 0;
	//	else
	//		hilite = pt.y * max_width + pt.x;
	//	auto need_break = false;
	//	rectf({0, 0, 640, 480}, colors::gray);
	//	for(auto y = 0; true; y++) {
	//		if(need_break)
	//			break;
	//		for(int x = 0; x < max_width; x++) {
	//			unsigned tv = y * max_width + x;
	//			if(!tv)
	//				continue;
	//			if(tv > count) {
	//				need_break = true;
	//				break;
	//			}
	//			point pt = m2s(x, y);
	//			point pz = pt - camera;
	//			image(pz.x - tile_width / 2, pz.y - tile_height / 2, ps, ps->ganim(tv, tm), 0);
	//			if(show_index) {
	//				sb.clear(); sb.add("%1i", tv);
	//				text(pz.x - textw(temp) / 2, pz.y - texth() / 2, temp);
	//			}
	//			if(hilite == tv)
	//				image(pz.x - tile_width / 2, pz.y - tile_height / 2, ps, ps->ganim(222, tm), 0);
	//		}
	//	}
	//	const int dx = 16;
	//	const int dy = 12;
	//	switch(hot.key) {
	//	case 'G': execute(setint, show_index ? 0 : 1, 0, &show_index); break;
	//	case KeyLeft: execute(setshort, camera.x - dx, 0, &camera.x); break;
	//	case KeyRight: execute(setshort, camera.x + dx, 0, &camera.x); break;
	//	case KeyUp: execute(setshort, camera.y - dy, 0, &camera.y); break;
	//	case KeyDown: execute(setshort, camera.y + dy, 0, &camera.y); break;
	//	case KeyEscape: execute(buttoncancel); break;
	//	}
	//	domodal();
	//}
	//closeform();
}

static void test_animate() {
	//char temp[260]; stringbuilder sb(temp);
	//openform();
	////int num;
	//auto animate = actor::getanimate(AnimateWeaponStand, 8);
	//const auto animate_last = LastAnimation;
	//creaturei player;
	//player.clear();
	//player.getarmor() = LeatherArmorMarkII;
	//player.getweapon(0) = X10mmPistol;
	//player.setgender(Female);
	//player.setdirection(2);
	//player.setanimate(animate, true);
	//while(ismodal()) {
	//	rectf({0, 0, 640, 480}, colors::gray);
	//	auto x = 320, y = 240;
	//	sb.clear();
	//	getpresent(sb, animate);
	//	sb.addn("frame %1i", player.frame - player.frame_start);
	//	auto pai = anminfo::get(player.rid);
	//	if(pai) {
	//		auto dir = player.getdirection();
	//		pai += player.getanimate();
	//		sb.addn("fps %1i", pai->fps);
	//		sb.addn("action frame %1i", pai->frame_act);
	//		sb.addn("frame count %1i", pai->frame_count);
	//		sb.addn("shift %1i, %2i", pai->offset[dir].x, pai->offset[dir].y);
	//		sb.addn("delta %1i, %2i", pai->delta[dir].x, pai->delta[dir].y);
	//	}
	//	auto old_fore = fore;
	//	setcolor(ColorInfo);
	//	textf(10, 10, 400, temp);
	//	fore = old_fore;
	//	player.x = x;
	//	player.y = y;
	//	player.updateanm();
	//	player.paint();
	//	line(x, y - 10, x, y + 10, colors::red);
	//	line(x - 10, y, x + 10, y, colors::red);
	//	domodal();
	//	switch(hot.key) {
	//	case KeyLeft: player.setdirection(player.getdirection() - 1); break;
	//	case KeyRight: player.setdirection(player.getdirection() + 1); break;
	//	case '+':
	//		if(animate == animate_last)
	//			animate = AnimateStand;
	//		else
	//			animate = animate_s(animate + 1);
	//		player.setanimate(animate);
	//		break;
	//	case '-':
	//		if(animate == AnimateStand)
	//			animate = animate_last;
	//		else
	//			animate = animate_s(animate - 1);
	//		player.setanimate(animate);
	//		break;
	//	case 'U':
	//		player.setanimate(animate);
	//		break;
	//	case 'K':
	//		player.setanimate(AnimateKnockOutBack);
	//		break;
	//	}
	//}
	//closeform();
}

static void test_game() {
	game.clear();
	auto& player = game.getplayer();
	player.create(Narg);
	player.add(SkillPoints, 5);
	player.add(X10mmAP, 24);
	player.add(X10mmPistol, 1);
	player.add(Knife, 1);
	player.add(Spear, 1);
	player.add(HuntingRifle, 1);
	player.add(X10mmSMG, 1);
	player.add(LeatherArmorMarkII, 1);
	player.add(MetalArmor, 1);
	player.add(Cookie, 4);
	player.add(X12GaShotgunShells, 10);
	player.setposition({1000, 100});
	player.setdirection(2);
	player.setanimate(AnimateStand, true);
	game.setcamera({1000, 100});
	loc.clear();
	loc.set(loc.geti(0, 0), bsdata<tilei>::elements[2], loc.width, loc.height);
	auto opponent = bsdata<creaturei>::add();
	opponent->create(Chitsa);
	opponent->add(Knife, 1);
	opponent->add(Spear, 1);
	opponent->add(Spear, 1);
	opponent->add(X10mmPistol, 1);
	opponent->add(X10mmSMG, 1);
	player.chat(*opponent, "manors");
	//player.trade(*opponent);
	//game.getplayer().charsheet();
	//setstage(game.introducing);
}

void gamei::mainmenu() {
	openform();
	while(ismodal()) {
		background(140);
		auto x = 29, y = 19;
		auto old_font = font;
		setfont(FONT4);
		post(0, game.newgame, buttong(x, y, 100, "История", 'H')); y += 41;
		post(0, test_walls, buttong(x, y, 100, "Загрузить", 'L')); y += 41;
		post(0, test_tiles, buttong(x, y, 100, "Тайлы", 'T')); y += 41;
		post(0, test_animate, buttong(x, y, 100, "Анимация", 'A')); y += 41;
		post(0, game.worldmap, buttong(x, y, 100, "Мир", 'W')); y += 41;
		post(0, test_game, buttong(x, y, 100, "Игра", 'G')); y += 41;
		font = old_font;
		domodal();
	}
	closeform();
}

int main(int argc, char* argv[]) {
	game.localization("ru", false);
	game.localization("debug", true);
	if(!game.checklocal())
		return -1;
	if(!game.readchats("ru"))
		return -1;
	//main_util();
	initialize();
	game.clear();
	game.add("Тестирование вывода информации в консоль для всех жителей городов %1.", "NRK");
	game.add("Оставайтесь хладнокровными и берегитесь врагов!");
	game.add("Длинный текст выглядит круто, кроме того его можно форматировать и прокручивать с помощью колеса мышки.");
	setstage(test_game);//game.mainmenu);
	runstage();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}