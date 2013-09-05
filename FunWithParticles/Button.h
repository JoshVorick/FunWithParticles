#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

class Button{
private:
	int x1, x2, y1, y2, timeHeldDown;
	ALLEGRO_COLOR colorUp, colorHover, colorDown;
	bool isHoveredOver, isDown;

public:
	Button();
	Button(int x, int y, int width, int height, 
		ALLEGRO_COLOR down, ALLEGRO_COLOR hover, ALLEGRO_COLOR up);
	bool processMouseCoor(int x, int y, bool isClicked);
	bool isBeingHeld();
	void draw();
};