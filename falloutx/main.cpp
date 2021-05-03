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
	player.setposition({1000, 100});
	player.setdirection(2);
	player.setanimate(AnimateStand, true);
	game.setcamera({1000, 100});
	loc.clear();
	loc.set(loc.geti(0, 0), bsdata<tilei>::elements[2], loc.width, loc.height);
	auto opponent = bsdata<creaturei>::add();
	opponent->create(Narg);
	opponent->add(Knife, 1);
	opponent->add(Spear, 1);
	opponent->add(Spear, 1);
	opponent->add(X10mmPistol, 1);
	opponent->add(X10mmSMG, 1);
	opponent->setposition({900, 100});
	opponent->setdirection(4);
	opponent->setanimate(AnimateStand, true);
	//player.chat(*opponent, "manors");
	//player.trade(*opponent);
	//game.getplayer().charsheet();
	setstage(game.introducing);
}

static void test_game() {
	game.setcamera({1000, 100});
	loc.clear();
	loc.set(loc.geti(0, 0), bsdata<tilei>::elements[2], loc.width, loc.height);
	loc.editor();
}

void test_walls();
void test_animate();
void test_tiles();

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
	main_util();
	initialize();
	game.clear();
	game.add("Тестирование вывода информации в консоль для всех жителей городов %1.", "NRK");
	game.add("Оставайтесь хладнокровными и берегитесь врагов!");
	game.add("Длинный текст выглядит круто, кроме того его можно форматировать и прокручивать с помощью колеса мышки.");
	setstage(test_game);
	//setstage(game.mainmenu);
	runstage();
	return 0;
}

int __stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}