#pragma once
#include "ColorGenerator.h"

ColorGenerator::ColorGenerator(){
	h=0;
	s=0.9;
	v=0.9;
	h_min = 0;
	s_min = 0;
	v_min = 0;
	h_max = 0.999;
	s_max = 0.999;
	v_max = 0.999;
	hBar_min = new SliderBar(515, 15, 100, al_map_rgb(100,0,0), h_min, h_max, 0);
	sBar_min = new SliderBar(515, 75, 100, al_map_rgb(0,100,0), s_min, s_max, 0);
	vBar_min = new SliderBar(515, 135, 100, al_map_rgb(0,0,100), v_min, v_max, 0);
	hBar_max = new SliderBar(515, 45, 100, al_map_rgb(100,0,0), h_min, h_max, 1);
	sBar_max = new SliderBar(515, 105, 100, al_map_rgb(0,100,0), s_min, s_max, 1);
	vBar_max = new SliderBar(515, 165, 100, al_map_rgb(0,0,100), v_min, v_max, 1);
}

void ColorGenerator::processMouseCoor(int mouseX, int mouseY, bool isDown){
	if(hBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(hBar_min->getLocation() > h_max)
			hBar_min->setLocation(h_max);
		h_min = hBar_min->getLocation();
		hBar_max->setMin(h_min);
	}
	else if(hBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(hBar_max->getLocation() < h_min)
			hBar_max->setLocation(h_min);
		h_max = hBar_max->getLocation();
		hBar_min->setMax(h_max);
	}
	else if(sBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(sBar_min->getLocation() > s_max)
			sBar_min->setLocation(s_max);
		s_min = sBar_min->getLocation();
		sBar_max->setMin(s_min);
	}
	else if(sBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(sBar_max->getLocation() < s_min)
			sBar_max->setLocation(s_min);
		s_max = sBar_max->getLocation();
		sBar_min->setMax(s_max);
	}
	else if(vBar_min->processMouseCoor(mouseX, mouseY, isDown)){
		if(vBar_min->getLocation() > v_max)
			vBar_min->setLocation(v_max);
		v_min = vBar_min->getLocation();
		vBar_max->setMin(v_min);
	}
	else if(vBar_max->processMouseCoor(mouseX, mouseY, isDown)){
		if(vBar_max->getLocation() < v_min)
			vBar_max->setLocation(v_min);
		v_max = vBar_max->getLocation();
		vBar_min->setMax(v_max);
	}
}

ALLEGRO_COLOR ColorGenerator::getNextColor(){
	h += golden_ratio * (h_max - h_min);
	if(h > h_max)
		h -= h_max - h_min;
	if(h_max - h_min == 0)
		h = h_min;
	
	s = (rand()%500/500.0)*(s_max-s_min)+s_min;
	v = (rand()%500/500.0)*(v_max-v_min)+v_min;

	//Convert H,S,V to R,G,B
	int h_i = (h*6);
	float f = h*6 - h_i;
	float p = v*(1-s);
	float q = v*(1-f*s);
	float t = v*(1-(1-f)*s);
	float r, g, b;
	switch(h_i){
	case 0:
		r=v;
		g=t;
		b=p;
		break;
	case 1:
		r=q;
		g=v;
		b=p;
		break;
	case 2:
		r=p;
		g=v;
		b=t;
		break;
	case 3:
		r=p;
		g=q;
		b=v;
		break;
	case 4:
		r=t;
		g=p;
		b=v;
		break;
	case 5:
		r=v;
		g=p;
		b=q;
		break;
	}
	return al_map_rgb((int)(r*256), (int)(g*256), (int)(b*256));
}

void ColorGenerator::draw(){
	hBar_min->draw();
	sBar_min->draw();
	vBar_min->draw();
	hBar_max->draw();
	sBar_max->draw();
	vBar_max->draw();
}