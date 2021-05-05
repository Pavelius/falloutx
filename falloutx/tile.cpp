#include "main.h"

// up, right, down, left, sides, corners,
// {CLeft|CDown, CLeft|CUp, CRight|CUp, CRight|CDown}
BSDATA(tilei) = {
	{{"TileDirt"}, {182, 183, 184, 185, 186, 187}},
	{{"TileStep"}, {192, 193, 194, 195, 196, 197}},
	{{"TileCave"}, {21, 22, 23, 24, 33, 35, /*32, 34, 36, 29, 30*/}, {}, {}},
	{{"TileRock"}, {57, 58, 63, 64}, {59, 60, 61, 62}, {55, 56}, {51, 52}, {45}, {68, 66, 67, 65}},
	{{"TileBorder"}, {2, 15, 16, 17}, {6}, {5}, {9}, {4}, {7, 10, 3, 8}, {13, 14, 11, 12}},
	{{"TilePlates"}, {2276, 2277, 2278, 2279, 2280, 2281, 2282, 2283, 2284, 2285, 2286}},
	{{"TileAcid"}, {1058, 1059, 1062, 1063}, {1054, 1055}, {1060, 1064, 1074}, {1066, 1067}, {1076, 1057, 1061}, {1056, 1068, 1065, 1053}},
};
assert_enum(tilei, TileAcid)

// LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Left,
// LeftUp, RightUp, RightDown, LeftDown
//land_info land_data[] = {
//	{"Тротуар", 15, {132, 133, 134, 135}, {131, 129, 130, 127, 126, 118, 117, 124}, {169, 170, 171, 168}},
//	{"Пол", 30, {470, 471, 472, 473}, {481, 474, 478, 475, 479, 476, 480, 477}, {403, 402, 404, 403}},
//	{"Кипичный пол", 15, {2, 16, 17, 15}, {8, 6, 7, 5, 10, 9, 3, 4}, {12, 13, 14, 11}},
//	{"Слизь", 40, {1058, 1059, 1062, 1063}, {1053, 1054, 1056, 1060, 1068, 1066, 1065, 1061}, {1080, 1077, 1069, 1072}},
//	{"Плитка", 15, {2261, 2274, 2274, 2274}, {2267, 2265, 2266, 2264, 2269, 2268, 2262, 2263}, {2271, 2272, 2273, 2270}},
//	{"Каменный пол", 80, {2237, 2238, 2239, 2240}, {}, {}},
//	{"Красный пол", 40, {1892, 1892, 1892, 1892}, {1899, 1893, 1897, 1895, 1900, 1894, 1898, 1896}, {1903, 1901, 1902, 1892}},
//	{"Пол пещеры", 50, {21, 22, 23, 24}, {}, {}},
//	{"Камни пещеры", 40, {57, 63, 57, 63}, {65, 61, 68, 55, 66, 51, 67, 45}, {}},
//	{"Огород", 15, {716, 717, 718}, {715, 719}, {}},
//};
struct tilegroupi {
	struct element : point {
		constexpr element(short x, short y, int shift = 0) : point{x, y}, shift(shift) {}
		int				shift;
	};
	const char*			name;
	point				size;
	short unsigned		start;
	short unsigned		count;
	std::initializer_list<element> additional;
};
static tilegroupi group_data[] = {{"Углубление в пещере", {2, 3}, 118},
	{"Ямка в пещере", {2, 2}, 124},
	{"Доски в пустоше", {3, 6}, 474},
	{"Крыша", {2, 5}, 492},
	{"Крыша с устройствами", {3, 2}, 500},
	{"Посейдон", {0, 0}, 508, 0, {{2, 1}, /**/{4, 1}, {6, 1}, {8, 1}, {10, 1}, {11, 1}, {11, 1}, /**/{10, 2}, {9, 1}, {8, 2}, {7, 1}, {6, 2}, {5, 1}, /**/{4, 2}, {3, 1}, {2, 2}}},
	{"Пришельцы", {6, 1}, 757, 0, {{7, 4, -1}, {6, 1}, {5, 1}}},
	{"Стена", {3, 1}, 812, 0, {{4, 1, -1}}},
	{"Крыша", {7, 7}, 826},
	{"Здание с вентилятором", {2, 1}, 914, 0, {{4, 1}, {6, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 2}, {4, 1}, {3, 2}, {2, 1}, {1, 1}}},
	{"Дырка в земле", {10, 1}, 1101, 0, {{9, 1}, {10, 1, -1}, {9, 1}, {9, 2, -1}, {8, 1}, {6, 1, 1}, {4, 1, 1}, {3, 1, 1}}},
	{"Лужа", {3, 3}, 1415},
	{"Каменная впадина", {8, 4}, 1425},
	{"Скалы", {9, 3}, 1527, 26},
	{"Скалы", {9, 2}, 1554},
	{"Мост", {3, 2}, 1749},
	{"Реактор", {8, 8}, 1827},
	{"Неработающий реактор", {8, 8}, 2028},
	{"Левая часть входа", {7, 1}, 2120, 0, {{8, 2, -1}, {9, 1, -1}, {10, 2, -1}, {8, 1, 2}, {7, 1, 1}}},
	{"Правая часть входа", {10, 1}, 2187, 0, {{9, 1}, {8, 1}, {5, 4}, {2, 1, 1}}},
};

static bool have(const tilea& source, unsigned short v) {
	for(auto e : source) {
		if(e == v)
			return true;
	}
	return false;
}

static short unsigned randomc(const tilea& source) {
	auto s = source.size();
	if(!s)
		return 0;
	return source.begin()[rand() % s];
}

static bool have(const short unsigned* source, unsigned short v) {
	return source[0] == v
		|| source[1] == v
		|| source[2] == v
		|| source[3] == v;
}

bool tilei::is(short unsigned v) const {
	return have(center, v)
		|| have(up, v)
		|| have(right, v)
		|| have(down, v)
		|| have(left, v)
		|| have(corners, v)
		|| have(sides, v);
}

int tilei::random() const {
	auto s = center.size();
	if(!s)
		return 0;
	return center.begin()[rand() % s];
}

tilei* tilei::find(short unsigned t) {
	for(auto& e : bsdata<tilei>()) {
		if(e.is(t))
			return &e;
	}
	return 0;
}

short unsigned getrandomie(tilea& source, short unsigned t) {
	if(source.size() == 0)
		return t;
	if(have(source, t))
		return t;
	return randomc(source);
}

short unsigned getrandomie(short unsigned t1, short unsigned t) {
	return t1 ? t1 : t;
}

short unsigned tilei::correct(short unsigned t, unsigned char c) {
	const unsigned char CUp = 8;
	const unsigned char CRight = 4;
	const unsigned char CDown = 2;
	const unsigned char CLeft = 1;
	switch(c) {
	case 0:
		return t;
	case CUp | CRight | CDown:
		return getrandomie(left, t);
	case CUp | CLeft| CDown:
		return getrandomie(right, t);
	case CLeft | CRight | CDown:
		return getrandomie(up, t);
	case CLeft | CRight | CUp:
		return getrandomie(down, t);
	case CUp | CLeft:
		return getrandomie(sides[1], t);
	case CUp | CRight:
		return getrandomie(sides[2], t);
	case CDown | CRight:
		return getrandomie(sides[3], t);
	case CDown | CLeft:
		return getrandomie(sides[0], t);
	default:
		if(have(center, t))
			return t;
		return randomc(center);
	}
}