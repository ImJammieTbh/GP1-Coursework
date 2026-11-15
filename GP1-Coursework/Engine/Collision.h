#pragma once

struct Rect {
	float x, y, width, height;
};

//collision detection that checks overlap but not touching edges
inline bool Overlaps(const Rect& a, const Rect& b) {
	return (
		a.x < b.x + b.width && 
		a.x + a.width > b.x &&
		a.y < b.y + b.height && 
		a.y + a.height > b.y
		);
}