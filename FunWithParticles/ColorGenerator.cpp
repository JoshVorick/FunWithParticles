#pragma once
#include "ColorGenerator.h"

ColorGenerator::ColorGenerator(){
	x=0;
	y=0;
	h=0;
	s=0.9;
	v=0.9;
	h_min = 0;
	s_min = 0;
	v_min = 0;
	h_max = 0.999;
	s_max = 0.999;
	v_max = 0.999;
	hBar_min = new SliderBar(x, y+10, 100, al_map_rgb(100,0,0), h_min, h_max, 0);
	sBar_min = new SliderBar(x, y+50, 100, al_map_rgb(0,100,0), s_min, s_max, 0);
	vBar_min = new SliderBar(x, y+90, 100, al_map_rgb(0,0,100), v_min, v_max, 0);
	hBar_max = new SliderBar(x, y+20, 100, al_map_rgb(100,0,0), h_min, h_max, 1);
	sBar_max = new SliderBar(x, y+60, 100, al_map_rgb(0,100,0), s_min, s_max, 1);
	vBar_max = new SliderBar(x, y+100, 100, al_map_rgb(0,0,100), v_min, v_max, 1);
	font18 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 18, 0);
}

ColorGenerator::ColorGenerator(int x, int y){
	this->x = x;
	this->y = y;
	h=0;
	s=0.9;
	v=0.9;
	h_min = 0;
	s_min = 0;
	v_min = 0;
	h_max = 0.999;
	s_max = 0.999;
	v_max = 0.999;
	hBar_min = new SliderBar(x, y+10, 100, al_map_rgb(100,0,0), h_min, h_max, 0);
	sBar_min = new SliderBar(x, y+50, 100, al_map_rgb(0,100,0), s_min, s_max, 0);
	vBar_min = new SliderBar(x, y+90, 100, al_map_rgb(0,0,100), v_min, v_max, 0);
	hBar_max = new SliderBar(x, y+20, 100, al_map_rgb(100,0,0), h_min, h_max, 1);
	sBar_max = new SliderBar(x, y+60, 100, al_map_rgb(0,100,0), s_min, s_max, 1);
	vBar_max = new SliderBar(x, y+100, 100, al_map_rgb(0,0,100), v_min, v_max, 1);
	font18 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 18, 0);
}

bool ColorGenerator::processMouseCoor(int mouseX, int mouseY, bool isDown){
	bool isUsingSlider = false;
	if(hBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(hBar_min->getLocation() > h_max)
			hBar_min->setLocation(h_max);
		h_min = hBar_min->getLocation();
		hBar_max->setMin(h_min);
		isUsingSlider = true;
	}
	else if(hBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(hBar_max->getLocation() < h_min)
			hBar_max->setLocation(h_min);
		h_max = hBar_max->getLocation();
		hBar_min->setMax(h_max);
		isUsingSlider = true;
	}
	else if(sBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(sBar_min->getLocation() > s_max)
			sBar_min->setLocation(s_max);
		s_min = sBar_min->getLocation();
		sBar_max->setMin(s_min);
		isUsingSlider = true;
	}
	else if(sBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(sBar_max->getLocation() < s_min)
			sBar_max->setLocation(s_min);
		s_max = sBar_max->getLocation();
		sBar_min->setMax(s_max);
		isUsingSlider = true;
	}
	else if(vBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(vBar_min->getLocation() > v_max)
			vBar_min->setLocation(v_max);
		v_min = vBar_min->getLocation();
		vBar_max->setMin(v_min);
		isUsingSlider = true;
	}
	else if(vBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(vBar_max->getLocation() < v_min)
			vBar_max->setLocation(v_min);
		v_max = vBar_max->getLocation();
		vBar_min->setMax(v_max);
		isUsingSlider = true;
	}
	return isUsingSlider;
}

ALLEGRO_COLOR ColorGenerator::getNextColor(){
	h += golden_ratio * (h_max - h_min);
	if(h > h_max)
		h -= h_max - h_min;
	if(h_max - h_min == 0)
		h = h_min;
	
	s = (rand()%500/500.0)*(s_max-s_min)+s_min;
	v = (rand()%500/500.0)*(v_max-v_min)+v_min;

	return map_hsv(h,s,v);
}

void ColorGenerator::draw(){
	float h = (h_max-h_min)/2 + h_min; //Create new ones to keep from messing up g_ratio algorithm
	float s = (s_max-s_min)/2 + s_min;
	float v = (v_max-v_min)/2 + v_min;
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+10, x + i*100, y+20, map_hsv(i,s,v), 1);
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+50, x + i*100, y+60, map_hsv(h,i,v), 1);
	for(float i=0; i<1; i+=0.01)
		al_draw_rectangle(x + i*100, y+90, x + i*100, y+100, map_hsv(h,s,i), 1);
	
	al_draw_text(font18,al_map_rgb(255,100,100),x-5,y+5,ALLEGRO_ALIGN_RIGHT, "Hue:");
	al_draw_text(font18,al_map_rgb(100,255,100),x-5,y+45,ALLEGRO_ALIGN_RIGHT, "Saturation:");
	al_draw_text(font18,al_map_rgb(100,100,255),x-5,y+85,ALLEGRO_ALIGN_RIGHT, "Variance:");

	hBar_min->draw();
	sBar_min->draw();
	vBar_min->draw();
	hBar_max->draw();
	sBar_max->draw();
	vBar_max->draw();
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