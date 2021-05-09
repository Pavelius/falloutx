#include "main.h"

using namespace draw;

BSDATA(pregeni) = {
	{{"NoPregen"}, 1, 18, Male, {5, 5, 5, 5, 5, 5, 5}},
	{{"Narg"}, 1, 21, Male, {8, 5, 9, 3, 4, 7, 4}, {HeavyHanded, Gifted}, {SmallGuns, MeleeWeapon, Throwing}},
	{{"Mingun"}, 1, 32, Male, {5, 8, 4, 4, 5, 9, 5}, {SmallFrame, Skilled}, {Sneak, Lockpick, Steal}},
	{{"Chitsa"}, 1, 19, Female, {4, 5, 4, 10, 7, 6, 4}, {OneHanded, SexAppeal}, {Speech, Barter, FirstAid}},
	{{"SuperMutant"}, 3, 70, Male, {9, 5, 4, 1, 1, 6, 2}, {}, {BigGuns, Unarmed}, {}, MAMTNT},
};
assert_enum(pregeni, SuperMutant)