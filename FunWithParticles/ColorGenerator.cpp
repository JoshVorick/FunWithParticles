#pragma once
#include "ColorGenerator.h"

ColorGenerator::ColorGenerator(){
	x=0;
	y=0;
	h=0;
	s=0.9;
	v=0.9;
	min[H] = 0;
	min[S] = 0.6;
	min[V] = 0.6;
	for(int i=0; i<3; i++){
		max[i] = 0.999;
		minBar[i] = new SliderBar(x, y+10+(40*i), 100, al_map_rgb(100,0,0), 0, 1, min[i]);
		maxBar[i] = new SliderBar(x, y+20+(40*i), 100, al_map_rgb(100,0,0), 0, 1, max[i]);
	}
	font18 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 18, 0);
}

ColorGenerator::ColorGenerator(int x, int y, ALLEGRO_COLOR color){
	this->color = color;
	this->x = x;
	this->y = y;
	h=0;
	s=0.9;
	v=0.9;
	min[H] = 0;
	min[S] = 0.6;
	min[V] = 0.6;
	for(int i=0; i<3; i++){
		max[i] = 0.999;
		minBar[i] = new SliderBar(x, y+10+(40*i), 100, color, 0, 1, min[i]);
		maxBar[i] = new SliderBar(x, y+20+(40*i), 100, color, 0, 1, max[i]);
	}
	font18 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 18, 0);
}

bool ColorGenerator::processMouseCoor(int mouseX, int mouseY, bool isDown){
	bool isUsingSlider = false;
	for(int i=0; i<3; i++){
		if(minBar[i]->processMouseCoor(mouseX, mouseY, isDown)){
			if(minBar[i]->getLocation() > max[i])
				minBar[i]->setLocation(max[i]);
			min[i] = minBar[i]->getLocation();
			maxBar[i]->setMin(min[i]);
			isUsingSlider = true;
			break;
		}if(maxBar[i]->processMouseCoor(mouseX, mouseY, isDown)){
			if(maxBar[i]->getLocation() < min[i])
				maxBar[i]->setLocation(min[i]);
			max[i] = maxBar[i]->getLocation();
			minBar[i]->setMax(max[i]);
			isUsingSlider = true;
			break;
		}
	}
	return isUsingSlider;
}

ALLEGRO_COLOR ColorGenerator::getNextColor(){
	h += golden_ratio * (max[H] - min[H]);
	if(h > max[H])
		h -= max[H] - min[H];
	if(max[H] - min[H] == 0)
		h = min[H];
	
	s = (rand()%500/500.0)*(max[S] - min[S]) + min[S];
	v = (rand()%500/500.0)*(max[V] - min[V]) + min[V];

	return map_hsv(h,s,v);
}

void ColorGenerator::draw(){
	al_draw_text(font18,color,x-5,y+5,ALLEGRO_ALIGN_RIGHT, "Hue:");
	al_draw_text(font18,color,x-5,y+45,ALLEGRO_ALIGN_RIGHT, "Saturation:");
	al_draw_text(font18,color,x-5,y+85,ALLEGRO_ALIGN_RIGHT, "Variance:");

	for(int i=0; i<3; i++){
		minBar[i]->draw();
		maxBar[i]->draw();
	}
	float h = (max[H] - min[H])/2 + min[H]; //Create new ones to keep from messing up g_ratio algorithm
	float s = (max[S] - min[S])/2 + min[S];
	float v = (max[S] - min[V])/2 + min[V];
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+10, x + i*100, y+20, map_hsv(i,s,v), 1);
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+50, x + i*100, y+60, map_hsv(h,i,v), 1);
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+90, x + i*100, y+100, map_hsv(h,s,i), 1);
}
	
ALLEGRO_COLOR ColorGenerator::map_hsv(float hue, float sat, float var){
	//Convert H,S,V to R,G,B
	int h_i = (hue*6);
	float f = hue*6 - h_i;
	float p = var*(1-sat);
	float q = var*(1-f*sat);
	float t = var*(1-(1-f)*sat);
	float r, g, b;
	switch(h_i){
	case 0:
		r=var;
		g=t;
		b=p;
		break;
	case 1:
		r=q;
		g=var;
		b=p;
		break;
	case 2:
		r=p;
		g=var;
		b=t;
		break;
	case 3:
		r=p;
		g=q;
		b=var;
		break;
	case 4:
		r=t;
		g=p;
		b=var;
		break;
	case 5:
		r=var;
		g=p;
		b=q;
		break;
	}
	return al_map_rgb((int)(r*256), (int)(g*256), (int)(b*256));
}