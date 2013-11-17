#pragma once
#include "SliderBar.h"

SliderBar::SliderBar(){
	x=0;
	y=0;
	sliderX = 0;
	myColor = al_map_rgb(100,100,100);
	isHeld = false;
}

SliderBar::SliderBar(int x, int y, int width, ALLEGRO_COLOR color, int min, int max, int startPos){
	this->min = min;
	this->max = max;
	sliderX = startPos;
	this->x = x;
	this->y = y;
	this->width = width;
	myColor = color;
	isHeld = false;
}

bool SliderBar::processMouseCoor(int mouseX, int mouseY, bool isDown){

	if(!isDown)
		isHeld = false;
	else if((mouseY-y)*(mouseY-y) + (mouseX-(x+sliderX*width))*(mouseX-(x+sliderX*width)) < 50){
		isHeld = true;
	}
	if(isHeld){
		sliderX = (mouseX-x)/(float)width;
		if(sliderX < 0)
			sliderX = 0;
		else if(sliderX >= 1)
			sliderX = 0.999;
	}
	return isHeld;
}

void SliderBar::draw(){
	al_draw_rectangle(x, y, x+width, y, myColor, 3);
	al_draw_filled_circle(x+(width*sliderX), y, 5, myColor);
}