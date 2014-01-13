#pragma once
#include <allegro5/allegro_primitives.h>

class SliderBar{
private:
	int x, y, width, min, max;		//location of the bar
	float sliderX;	//location of the slider on the bar
	bool isHeld; //Holds whether or not the user is using the current slider
	ALLEGRO_COLOR myColor;

public:
	SliderBar();
	SliderBar(int x, int y, int width, ALLEGRO_COLOR color, int min, int max, float startPos);

	bool processMouseCoor(int mouseX, int mouseY, bool isDown); //return true if being moved
	float getLocation(){return sliderX;} 
	void setLocation(float xLoc) {sliderX = xLoc;}
	void setMin(int min){this->min = min;}
	void setMax(int max){this->max = max;}
	void draw();
};