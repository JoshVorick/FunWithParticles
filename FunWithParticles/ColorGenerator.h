#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "SliderBar.h"

const double golden_ratio = 0.618033988749895;

class ColorGenerator{
private:
	float h,s,v,h_min,h_max,s_min,s_max,v_min,v_max;
	SliderBar *hBar_min, *hBar_max, *sBar_min, *sBar_max, *vBar_min, *vBar_max;

public:
	ColorGenerator();

	void processMouseCoor(int mouseX, int mouseY, bool isDown);
	ALLEGRO_COLOR getNextColor();
	void draw();
};