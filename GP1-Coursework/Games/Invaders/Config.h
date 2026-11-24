#pragma once

//shared across every game so the engine and games agree on console size
namespace Config {
	constexpr int screenWidth = 80;
	constexpr int screenHeight = 30; //matches GroundY + 1, change if your console buffer differs
}

namespace Invaders {
	//keeps existing Invaders::screenWidth references working
	using Config::screenWidth;

	//fixed rows so objects line up with the sprites drawn on them
	constexpr int PlayerY = 28;
	constexpr int GroundY = 29;

	//5x11 matches the original arcade formation
	constexpr int alienRows = 5;
	constexpr int alienCols = 11;
	constexpr int aliensCount = alienRows * alienCols;

	constexpr int alienSpacingX = 2;
	constexpr int alienSpacingY = 2;
	constexpr int alienStartY = 1;
	//derived from formation width so it stays centred if cols or spacing change
	constexpr int alienStartX = (screenWidth - ((alienCols - 1) * alienSpacingX + 1)) / 2;

	//bounds are defined as an offset from centre so the play area stays symmetrical
	constexpr int BoundsHalfWidth = 20;
	constexpr int BoundsThickness = 2;
	constexpr int BoundsTopY = 0;
	constexpr int BoundsBottomY = PlayerY; //stops above the player row
	constexpr int BoundsLeftX = screenWidth / 2 - BoundsHalfWidth;
	//+1 mirrors the left wall, since both walls are multiple cells wide
	constexpr int BoundsRightX = screenWidth / 2 + BoundsHalfWidth - BoundsThickness + 1;

	//what aliens and the player collide against, derived so it can't drift from the walls
	constexpr int PlayMinX = BoundsLeftX + BoundsThickness; //first free cell
	constexpr int PlayMaxX = BoundsRightX; //first wall cell, exclusive

	//barrier NEW
	constexpr int barrierY = 22;
	constexpr int barrierGroupCount = 4;
	constexpr int barrierWidth = 3;
	constexpr int barrierHeight = 3;
	constexpr int barrierGaps = ((PlayMaxX - PlayMinX) - barrierGroupCount * barrierWidth) / (barrierGroupCount + 1);
	constexpr int barrierX = PlayMinX + barrierGaps; //first barrier group starts after the first gap
}