#include "main.h"

BSDATA(pregeni) = {
	{{"NoPregen"}, VaultDweller, 1, 18, Male, {5, 5, 5, 5, 5, 5, 5}},
	{{"Narg"}, VaultDweller, 1, 21, Male, {8, 5, 9, 3, 4, 7, 4}, {HeavyHanded, Gifted}, {SmallGuns, MeleeWeapon, Throwing}},
	{{"Mingun"}, VaultDweller, 1, 32, Male, {5, 8, 4, 4, 5, 9, 5}, {SmallFrame, Skilled}, {Sneak, Lockpick, Steal}},
	{{"Chitsa"}, VaultDweller, 1, 19, Female, {4, 5, 4, 10, 7, 6, 4}, {OneHanded, SexAppeal}, {Speech, Barter, FirstAid}},
};
assert_enum(pregeni, Chitsa)