#pragma once
#include "Button.h"

Button::Button(){
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	timeHeldDown = 0;
	colorUp = al_map_rgb(100,0,200);
	colorHover = al_map_rgb(100,0,100);
	colorDown = al_map_rgb(200,0,100);
	isHoveredOver = false;
	isDown = false;
}

Button::Button(int x, int y, int width, int height, 
		ALLEGRO_COLOR down, ALLEGRO_COLOR hover, ALLEGRO_COLOR up){
	x1 = x;
	x2 = x + width;
	y1 = y;
	y2 = y + height;
	timeHeldDown = 0;
	colorUp = up;
	colorHover = hover;
	colorDown = down;
	isHoveredOver = false;
	isDown = false;
}

void Button::setNewColors(ALLEGRO_COLOR down, ALLEGRO_COLOR hover, ALLEGRO_COLOR up){
	colorUp = up;
	colorHover = hover;
	colorDown = down;
}

bool Button::processMouseCoor(int x, int y, bool isClicked){
	if(x1 < x &&  x < x2 && y1 < y && y < y2){
		isHoveredOver = true;
		if(isClicked){
			isDown = true;
			timeHeldDown++;
		}else{
			isDown = false;
			timeHeldDown = 0;
		}
		return true;
	}else{
		isHoveredOver = false;
		isDown = false;
		timeHeldDown = 0;
	}
	return false;
}

bool Button::isBeingHeld(){
	if(timeHeldDown >= 30)
		return true;
	return false;
}

void Button::draw(){
	if(isDown)
		al_draw_rectangle(x1, y1, x2, y2, colorDown, 5);
	else if(isHoveredOver)
		al_draw_rectangle(x1, y1, x2, y2, colorHover, 5);
	else
		al_draw_rectangle(x1, y1, x2, y2, colorUp, 5);
}