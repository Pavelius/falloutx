#include "draw.h"
#include "main.h"

using namespace draw;

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

static void test_adventure() {
	game.clear();
	auto& player = game.getplayer();
	player.create(Chitsa);
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
	player.add(AdvancedPowerArmor, 1);
	player.add(AdvPowerArmorMKII, 1);
	player.setposition(loc.geth(23, 25));
	player.setdirection(2);
	player.setanimate(AnimateStand, true);
	game.setcamera({1000, 100});
	if(!loc.read("test")) {
		loc.clear();
		loc.set(loc.geti(0, 0), bsdata<tilei>::elements[2], 128, 128);
	}
	auto opponent = bsdata<creaturei>::add();
	opponent->create(SuperMutant);
	opponent->add(Knife, 1);
	opponent->add(Spear, 1);
	opponent->add(Spear, 1);
	opponent->add(X10mmPistol, 1);
	opponent->add(X10mmSMG, 1);
	opponent->setposition(loc.geth(25, 19));
	opponent->setdirection(2);
	opponent->setanimate(AnimateStand, true);
	//player.chat(*opponent, "manors");
	//player.trade(*opponent);
	//game.getplayer().charsheet();
	setstage(game.introducing);
}

static void test_game() {
	//test_tile();
	game.setcamera({1000, 100});
	if(!loc.read("test")) {
		loc.clear();
		loc.set(loc.geti(0, 0), bsdata<tilei>::elements[2], loc.width, loc.height);
	}
	loc.editor();
}

void main_util();
void test_tile();
void test_walls();
void test_animate();

void gamei::mainmenu() {
	openform();
	while(ismodal()) {
		background(140);
		auto x = 29, y = 19;
		auto old_font = font;
		setfont(FONT4);
		post(0, game.newgame, buttong(x, y, 100, "История", 'H')); y += 41;
		post(0, test_walls, buttong(x, y, 100, "Загрузить", 'L')); y += 41;
		post(0, test_tile, buttong(x, y, 100, "Тайлы", 'T')); y += 41;
		post(0, test_animate, buttong(x, y, 100, "Анимация", 'A')); y += 41;
		post(0, game.worldmap, buttong(x, y, 100, "Мир", 'W')); y += 41;
		post(0, test_game, buttong(x, y, 100, "Игра", 'G')); y += 41;
		font = old_font;
		domodal();
	}
	closeform();
}

static point h2s1(int x, int y) {
	return{
		(short)(16 * (y + x * 2 - x / 2)),
		(short)(12 * (y - x / 2))
	};
}

static point s2h1(point pt) {
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
}

static void test_hittest() {
	char temp[260]; stringbuilder sb(temp);
	static int walls_row[] = {775, 772, 773, 776, 777, 778};
	openform();
	bool show_hex_index = true;
	game.camera = {-320, -240};
	while(ismodal()) {
		rectf({0, 0, 640, 480}, colors::gray);
		for(auto y = 0; y < 3; y++) {
			for(auto x = 0; x < 3; x++) {
				auto pt = t2s({(short)x, (short)y}) - game.camera;
				image(pt.x, pt.y, TILES, 132, 0);
			}
		}
		if(true) {
			auto index = 172;
			for(auto x = 0; x < 6; x++) {
				auto pt = h2s1(x, 0) - game.camera;
				image(pt.x, pt.y, WALLS, walls_row[x], 0);
				marker(pt.x, pt.y);
			}
		}
		if(false) {
			for(auto y = 195; y < 220; y++) {
				auto push_fore = fore;
				for(auto x = 390; x < 480; x++) {
					point pz = {(short)x, (short)y};
					auto pt = s2t(pz + game.camera);
					if(pt.x == 1 && pt.y == 2)
						fore = colors::red;
					else
						fore = colors::green;
					pixel(x, y);
				}
			}
		}
		if(true) {
			for(auto y = 0; y < 2; y++) {
				for(auto x = 0; x < 3 * 2; x++) {
					auto pt = h2s1(x, y) - game.camera;
					image(pt.x - 15, pt.y - 11, INTRFACE, 1, 0);
					sb.clear(); sb.add("%1i, %2i", x, y);
					text(pt.x - textw(temp) / 2, pt.y - 4, temp);
				}
			}
		}
		auto r10 = s2h1(h2s1(1, 0));
		auto r11 = s2h1(h2s1(1, 1));
		auto r51 = s2h1(h2s1(5, 2));
		auto r312 = s2h1(h2s1(3, 12));
		if(true) {
			for(auto y = 120; y < 250; y++) {
				auto push_fore = fore;
				fore = colors::red;
				for(auto x = 220; x < 520; x++) {
					point pz;
					pz.x = x; pz.y = y;
					auto pt = s2h1(pz + game.camera);
					if(pt.x == 4 && pt.y == 0)
					//	fore = colors::red;
					//else
					//	fore = colors::green;
					pixel(x, y);
				}
				fore = push_fore;
			}
		}
		sb.clear();
		point pt;
		pt = s2t(hot.mouse + game.camera);
		if(pt.x >= 0 && pt.y >= 0)
			sb.addn("Tile %1i, %2i", pt.x, pt.y);
		pt = s2h1(hot.mouse + game.camera);
		if(pt.x >= 0 && pt.y >= 0)
			sb.addn("Hex %1i, %2i", pt.x, pt.y);
		text(4, 4, temp);
		domodal();
		switch(hot.key) {
		case KeyEscape: breakmodal(0); break;
		case 'P': show_hex_index = !show_hex_index; break;
		}
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
	main_util();
	initialize();
	game.clear();
	game.add("Длинный текст выглядит круто, кроме того его можно форматировать и прокручивать с помощью колеса мышки.");
	//setstage(test_hittest);
	setstage(test_adventure);
	//setstage(game.mainmenu);
	//setstage(test_game);
	runstage();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}