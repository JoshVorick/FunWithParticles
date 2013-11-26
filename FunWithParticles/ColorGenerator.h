#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "SliderBar.h"

enum {H,S,V};

const double golden_ratio = 0.618033988749895;

class ColorGenerator{
private:
	int x, y;
	float h,s,v, min[3], max[3];
	SliderBar *minBar[3], *maxBar[3];
	ALLEGRO_FONT *font18;
	ALLEGRO_COLOR color;
	ALLEGRO_COLOR map_hsv(float hue, float sat, float var);

public:
	ColorGenerator();
	ColorGenerator(int x, int y, ALLEGRO_COLOR color);

	bool processMouseCoor(int mouseX, int mouseY, bool isDown); //returns true if being used
	ALLEGRO_COLOR getNextColor();
	void draw();
};