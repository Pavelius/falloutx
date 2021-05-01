#include "main.h"

BSDATA(commandi) = {
	{{"Cancel"}},
	{{"Next"}},
	{{"Back"}},
	{{"Options"}},
	{{"Save"}},
	{{"Load"}},
	{{"Delete"}},
	{{"Print"}},
	{{"Quit"}},
	{{"Bonuses"}, 54},
	{{"Karma"}, 47},
	{{"KilledCreatures"}, 46},
	{{"InvalidTag"}},
	{{"InvalidMaximum"}},
	{{"InvalidMinimal"}},
	{{"InvalidRemove"}},
	{{"NotEnoughtScoresToRaise"}},
	{{"InvalidStartGame"}},
	{{"InvalidTrait"}},
};
assert_enum(commandi, LastCommand)