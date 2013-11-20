#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <math.h>
#include <vector>

#include "Button.h"
#include "ColorGenerator.h"

using namespace std;

enum buttons{CO_OF_REST_UP, CO_OF_REST_DOWN, GRAVITY_UP, GRAVITY_DOWN, SELECT_BALL, SELECT_SPARK, SELECT_FIZZLE, SELECT_CIRCLES, SELECT_FROZEN, SELECT_BLACKHOLE, SELECT_GRAVITY_BALL, 
	EXPLOSIVE_UP, EXPLOSIVE_DOWN, PART_PER_TICK_UP, PART_PER_TICK_DOWN, SPARK_TRAIL_UP, SPARK_TRAIL_DOWN, SELECT_SHAPE_CIRCLE, SELECT_SHAPE_SQUARE};
const int NUM_BUTTONS = 19;

const int FPS = 120;
const int maxParticles = 3000;
const int maxBlackHoles = 4;
const int width = 800;
const int height = 800;

void processButtonClick(Button *button, int i);
void createNewParticle(struct particle *newParticle);
void createNewSparkle(struct particle *spark, struct particle *sparkle);
void updateAsBall(struct particle *theParticle);
void updateAsSpark(struct particle *theParticle);
void processBlackHoleGravity(struct particle *theParticle, struct particle *theBlackHole);
void drawText(ALLEGRO_FONT *font24, ALLEGRO_COLOR color);

struct particle{
	ALLEGRO_COLOR color;
	float size, x, y, vx, vy;
	int age;
	bool alive;
};

float co_of_restitution = -0.85;
float gravity = 9.8;
float explosiveness = 15;
int mouseX = 0;
int mouseY = 0;
int particleType = SELECT_BALL; //i.e. ball, spark, etc.
int particleShape = SELECT_SHAPE_CIRCLE;
int particlesPerTick = 1;
int sparkliness = 5;
int radius = 5;
bool isShowing[NUM_BUTTONS];
bool showButtons = true;
ColorGenerator *colorGen;
bool showColors = true;

int main(void){
	//struct particle liveParticles[maxParticles];
	//struct particle blackHoles[maxBlackHoles];
	
	vector<struct particle> vliveParticles;
	vector<struct particle> vblackHoles;

	//int curParticle = 0;
	int curSparkle = 0;
	//int curBlackHole = 0;
	bool mouseDown = false;
	bool done = false;
	bool redraw = true;
	bool start = false;
	bool isUsingButton;
	bool saveNewBackground = false;
	Button *buttons[NUM_BUTTONS];

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font24;
	ALLEGRO_COLOR limegreen;
	ALLEGRO_BITMAP *background;

	al_init();

	//al_set_new_display_flags(ALLEGRO_RESIZABLE);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(width, height);
	al_set_window_position(display, 0, 0);
	al_set_window_title(display, "Particle Simulator");

	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	/*
	for(int i=0; i<maxParticles; i++){
		liveParticles[i].age=0;
		liveParticles[i].x=0;
		liveParticles[i].y=0;
		liveParticles[i].vx=0;
		liveParticles[i].vy=0;
		liveParticles[i].size=0;
		liveParticles[i].alive=false;
	}
	for(int i=0; i<maxBlackHoles;i++){
		blackHoles[i].age=0;
		blackHoles[i].x=0;
		blackHoles[i].y=0;
		blackHoles[i].vx=0;
		blackHoles[i].vy=0;
		blackHoles[i].size=0;
		blackHoles[i].alive=false;
	}*/
	
	buttons[CO_OF_REST_UP] = new Button(350,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[CO_OF_REST_DOWN] = new Button(385,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[GRAVITY_UP] = new Button(190,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[GRAVITY_DOWN] = new Button(225,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_BALL] = new Button(width-30,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_SPARK] = new Button(width-30,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_FIZZLE] = new Button(width-30,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_CIRCLES] = new Button(width-30,105,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_FROZEN] = new Button(width-30,135,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_BLACKHOLE] = new Button(width-30,165,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_GRAVITY_BALL] = new Button(width-30,195,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[EXPLOSIVE_UP] = new Button(240,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[EXPLOSIVE_DOWN] = new Button(275,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[PART_PER_TICK_UP] = new Button(225,105,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[PART_PER_TICK_DOWN] = new Button(260,105,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SPARK_TRAIL_UP] = new Button(225,135,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SPARK_TRAIL_DOWN] = new Button(260,135,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_SHAPE_CIRCLE] = new Button(width-30,height-55,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_SHAPE_SQUARE] = new Button(width-30,height-25,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));

	for(int i=0; i<NUM_BUTTONS; i++)
		isShowing[i] = true;
	processButtonClick(buttons[SELECT_BALL], SELECT_BALL);
	
	font24 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 24, 0);
	limegreen = al_map_rgb(50,250,50);
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/FPS);
	background = al_create_bitmap(width, height);
	al_set_target_bitmap(background);
	al_clear_to_color(al_map_rgb(10,10,10));
	al_set_target_bitmap(al_get_backbuffer(display));

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	
	colorGen = new ColorGenerator(515,0, al_map_rgb(250,250,250));

	al_start_timer(timer);
	
	while(!start){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		switch(ev.type){
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
				start = true;
			}else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				start = true;
				done = true;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			start = true;
			done = true;
			break;
		}
		if(redraw && al_is_event_queue_empty(event_queue)){
			al_draw_text(font24, al_map_rgb(20,250,250), width/2, height/2 - 120, ALLEGRO_ALIGN_CENTRE, "Press B to toggle button options. Press C to toggle color options.");
			al_draw_text(font24, al_map_rgb(100,200,255), width/2, height/2 - 90, ALLEGRO_ALIGN_CENTRE, "Press S to save as background. Press X to clear the screen.");
			al_draw_text(font24, al_map_rgb(180,150,255), width/2, height/2 - 60, ALLEGRO_ALIGN_CENTRE, "Use keys 1-6 to toggle the particle type.");
			al_draw_text(font24, al_map_rgb(250,50,250), width/2, height/2 - 30, ALLEGRO_ALIGN_CENTRE, "Press escape to exit.");
			al_draw_text(font24, al_map_rgb(250,50,30), width/2, height/2 + 30, ALLEGRO_ALIGN_CENTRE, "Press space to continue.");
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	while(!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		switch(ev.type){
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			isUsingButton = false;

			if(showColors)
				if(colorGen->processMouseCoor(mouseX, mouseY, mouseDown))
					isUsingButton = true;

			if(showButtons)
				for(int i=0; i<NUM_BUTTONS; i++){
					if(buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown)){
						if(buttons[i]->isBeingHeld())
							processButtonClick(buttons[i], i);
						isUsingButton = true;
					}
				}
			/*
			if(mouseDown && !isUsingButton && particleType != SELECT_BLACKHOLE && mouseY>=0){
				for(int i=0; i<particlesPerTick; i++){
					createNewParticle(&liveParticles[curParticle]);
					curParticle = (curParticle+1) % maxParticles;
				}
			}
			if(particleType != SELECT_FROZEN){
				for(int i=0;i<maxParticles;i++){
					for(int j=0;j<maxBlackHoles;j++){
						if(blackHoles[j].alive)
							processBlackHoleGravity(&liveParticles[i], &blackHoles[j]);
					}
					if(liveParticles[i].alive){
						if(particleType == SELECT_BALL)
							updateAsBall(&liveParticles[i]);
						else if(particleType == SELECT_CIRCLES){
							updateAsBall(&liveParticles[i]);
							liveParticles[i].size = height - liveParticles[i].y;
						}else if(particleType == SELECT_SPARK){
							updateAsSpark(&liveParticles[i]);
							if(liveParticles[i].size == radius && curSparkle == 0){
								createNewSparkle(&liveParticles[i], &liveParticles[curParticle]);
								curParticle = (curParticle+1) % maxParticles;
							}
						}else if(particleType == SELECT_FIZZLE){
							updateAsSpark(&liveParticles[i]);
							if(i % sparkliness == 0){
								createNewSparkle(&liveParticles[i], &liveParticles[curParticle]);
								curParticle = (curParticle+1) % maxParticles;
							}
						}
					}
				}
			}*/
			if(mouseDown && !isUsingButton && particleType != SELECT_BLACKHOLE && mouseY>=0){
				for(int i=0; i<particlesPerTick; i++){
					struct particle newParticle;
					createNewParticle(&newParticle);
					vliveParticles.push_back(newParticle);
				}
			}
			if(particleType != SELECT_FROZEN){
				for(int i=0;i<vliveParticles.size();i++){
					for(int j=0;j<vblackHoles.size();j++){
						processBlackHoleGravity(&vliveParticles[i], &vblackHoles[j]);
					}
					if(particleType == SELECT_BALL)
						updateAsBall(&vliveParticles[i]);
					else if(particleType == SELECT_CIRCLES){
						updateAsBall(&vliveParticles[i]);
						vliveParticles[i].size = height - vliveParticles[i].y;
					}else if(particleType == SELECT_SPARK){
						updateAsSpark(&vliveParticles[i]);
						if(vliveParticles[i].size == radius && curSparkle == 0){
							struct particle newSparkle;
							createNewSparkle(&vliveParticles[i], &newSparkle);
							vliveParticles.push_back(newSparkle);
						}else if(vliveParticles[i].alive == false){
							vliveParticles.erase(vliveParticles.begin()+i);
							i--;
						}
					}else if(particleType == SELECT_FIZZLE){
						updateAsSpark(&vliveParticles[i]);
						if(i % sparkliness == 0){
							struct particle newSparkle;
							createNewSparkle(&vliveParticles[i], &newSparkle);
							vliveParticles.push_back(newSparkle);
						}
						if(vliveParticles[i].alive == false){
							vliveParticles.erase(vliveParticles.begin()+i);
							i--;
						}
					}else if(particleType == SELECT_GRAVITY_BALL){
						updateAsBall(&vliveParticles[i]);
						for(int j=i+1; j<vliveParticles.size();j++){
							processBlackHoleGravity(&vliveParticles[i],&vliveParticles[j]);
							processBlackHoleGravity(&vliveParticles[j],&vliveParticles[i]);
						}
					}
				}
			}
			if(particleType == SELECT_GRAVITY_BALL)
				while(vliveParticles.size()*vliveParticles.size()*(vblackHoles.size()+1) > 20000){
					vliveParticles.erase(vliveParticles.begin());
				}
			else
				while(vliveParticles.size()*(vblackHoles.size()+1) > 20000){
					vliveParticles.erase(vliveParticles.begin());
				}

			curSparkle = (curSparkle + 1) % sparkliness;
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			mouseDown = true;
			isUsingButton = false;

			if(showColors)
				if(colorGen->processMouseCoor(mouseX, mouseY, mouseDown))
					isUsingButton = true;

			for(int i=0; i<NUM_BUTTONS; i++){
				if(isShowing[i] && buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown)){
					processButtonClick(buttons[i], i);
					isUsingButton = true;
				}
			}
			if(!isUsingButton){
				if(particleType != SELECT_BLACKHOLE){
					struct particle newParticle;
					createNewParticle(&newParticle);
					vliveParticles.push_back(newParticle);
				}else{
					struct particle newblackHole;
					createNewParticle(&newblackHole);
					vblackHoles.push_back(newblackHole);
				}
			}
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouseDown = false;
			for(int i=0; i<NUM_BUTTONS; i++){
				buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown);
			}
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			switch(ev.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_B:
				if(showButtons){
					showButtons = false;
					processButtonClick(buttons[particleType], particleType);
				}else{
					for(int i=0; i<NUM_BUTTONS; i++)
						isShowing[i] = true;
					showButtons = true;
				}
				break;
			case ALLEGRO_KEY_C:
				showColors = !showColors;
				break;
			case ALLEGRO_KEY_1:
				processButtonClick(buttons[SELECT_BALL], SELECT_BALL);
				break;
			case ALLEGRO_KEY_2:
				processButtonClick(buttons[SELECT_SPARK], SELECT_SPARK);
				break;
			case ALLEGRO_KEY_3:
				processButtonClick(buttons[SELECT_FIZZLE], SELECT_FIZZLE);
				break;
			case ALLEGRO_KEY_4:
				processButtonClick(buttons[SELECT_CIRCLES], SELECT_CIRCLES);
				break;
			case ALLEGRO_KEY_5:
				processButtonClick(buttons[SELECT_FROZEN], SELECT_FROZEN);
				break;
			case ALLEGRO_KEY_6:
				processButtonClick(buttons[SELECT_BLACKHOLE], SELECT_BLACKHOLE);
				break;
			case ALLEGRO_KEY_7:
				processButtonClick(buttons[SELECT_GRAVITY_BALL], SELECT_GRAVITY_BALL);
				break;
			case ALLEGRO_KEY_S:
				saveNewBackground = true;
				break;
			case ALLEGRO_KEY_X:
				for(int i=0; i<vliveParticles.size(); i++){
					vliveParticles.clear();
				}
				for(int i=0; i<vblackHoles.size(); i++){
					vblackHoles.clear();
				}
				break;
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw = false;
			al_draw_bitmap(background, 0, 0, 0);

			if(saveNewBackground){
				al_set_target_bitmap(background);
				al_clear_to_color(al_map_rgb(0,0,0));
			}
			/*
			for(int i=0; i<maxBlackHoles; i++){
				if(blackHoles[i].alive)
					al_draw_filled_circle(blackHoles[i].x, blackHoles[i].y, blackHoles[i].size, al_map_rgb(255, 255, 255));
			}
			if(particleShape == SELECT_SHAPE_CIRCLE){
				for(int i=curParticle; i<maxParticles; i++){
					if(liveParticles[i].alive)
						al_draw_filled_circle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].size, liveParticles[i].color);
				}
				for(int i=0; i<curParticle; i++){
					if(liveParticles[i].alive)
						al_draw_filled_circle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].size, liveParticles[i].color);
				}
			}
			if(particleShape == SELECT_SHAPE_SQUARE){
				for(int i=curParticle; i<maxParticles; i++){
					if(liveParticles[i].alive)
						al_draw_filled_rectangle(liveParticles[i].x-liveParticles[i].size, liveParticles[i].y-liveParticles[i].size, 
							liveParticles[i].size+liveParticles[i].x, liveParticles[i].size+liveParticles[i].y, liveParticles[i].color);
				}
				for(int i=0; i<curParticle; i++){
					if(liveParticles[i].alive)
						al_draw_filled_rectangle(liveParticles[i].x-liveParticles[i].size, liveParticles[i].y-liveParticles[i].size, 
							liveParticles[i].size+liveParticles[i].x, liveParticles[i].size+liveParticles[i].y, liveParticles[i].color);
				}
			}*/		
			for(int i=0; i<vblackHoles.size(); i++){
				al_draw_filled_circle(vblackHoles[i].x, vblackHoles[i].y, vblackHoles[i].size, al_map_rgb(255, 255, 255));
			}
			if(particleShape == SELECT_SHAPE_CIRCLE){
				for(int i=0; i<vliveParticles.size(); i++){
					al_draw_filled_circle(vliveParticles[i].x, vliveParticles[i].y, vliveParticles[i].size, vliveParticles[i].color);
				}
			}
			if(particleShape == SELECT_SHAPE_SQUARE){
				for(int i=0; i<vliveParticles.size(); i++){
					al_draw_filled_rectangle(vliveParticles[i].x-vliveParticles[i].size, vliveParticles[i].y-vliveParticles[i].size, 
							vliveParticles[i].size+vliveParticles[i].x, vliveParticles[i].size+vliveParticles[i].y, vliveParticles[i].color);
				}
			}	
			if(saveNewBackground){
				al_set_target_bitmap(al_get_backbuffer(display));
				saveNewBackground = false;
			}
			if(showButtons)
				for(int i=0; i<NUM_BUTTONS; i++){
					if(isShowing[i])
						buttons[i]->draw();
				}

			if(showColors)
				colorGen->draw();
			if(showButtons)
				drawText(font24, limegreen);
			
			al_draw_textf(font24, limegreen, 300, 250, 0, "Number of Balls: %i", vliveParticles.size());
			al_draw_textf(font24, limegreen, 300, 280, 0, "Number of Black Holes: %i", vblackHoles.size());

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	al_destroy_timer(timer);
	al_destroy_font(font24);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

void processButtonClick(Button *button, int i){
	switch(i){
	case CO_OF_REST_UP:
		co_of_restitution -= 0.05;
		if(co_of_restitution < -0.99)
			co_of_restitution = -0.99;
		break;
	case CO_OF_REST_DOWN:
		co_of_restitution += 0.05;
		if(co_of_restitution > -0.1)
			co_of_restitution = -0.1;
		break;
	case GRAVITY_UP:
		gravity += 0.1;
		if(gravity > 50)
			gravity = 50;
		break;
	case GRAVITY_DOWN:
		gravity -= 0.1;
		if(gravity < 0)
			gravity = 0;
		break;
	case SELECT_BALL:
		particleType = SELECT_BALL;
		isShowing[CO_OF_REST_DOWN] = true;
		isShowing[CO_OF_REST_UP] = true;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = false;
		isShowing[SPARK_TRAIL_DOWN] = false;
		break;
	case SELECT_SPARK:
		particleType = SELECT_SPARK;
		isShowing[CO_OF_REST_DOWN] = false;
		isShowing[CO_OF_REST_UP] = false;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = true;
		isShowing[SPARK_TRAIL_DOWN] = true;
		break;
	case SELECT_FIZZLE:
		particleType = SELECT_FIZZLE;
		isShowing[CO_OF_REST_DOWN] = false;
		isShowing[CO_OF_REST_UP] = false;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = true;
		isShowing[SPARK_TRAIL_DOWN] = true;
		break;
	case SELECT_CIRCLES:
		particleType = SELECT_CIRCLES;
		isShowing[CO_OF_REST_DOWN] = true;
		isShowing[CO_OF_REST_UP] = true;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = false;
		isShowing[SPARK_TRAIL_DOWN] = false;
		break;
	case SELECT_FROZEN:
		particleType = SELECT_FROZEN;
		isShowing[CO_OF_REST_DOWN] = true;
		isShowing[CO_OF_REST_UP] = true;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = true;
		isShowing[SPARK_TRAIL_DOWN] = true;
		break;
	case SELECT_BLACKHOLE:
		particleType = SELECT_BLACKHOLE;
		isShowing[CO_OF_REST_DOWN] = true;
		isShowing[CO_OF_REST_UP] = true;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = true;
		isShowing[SPARK_TRAIL_DOWN] = true;
		break;
	case SELECT_GRAVITY_BALL:
		particleType = SELECT_GRAVITY_BALL;
		isShowing[CO_OF_REST_DOWN] = true;
		isShowing[CO_OF_REST_UP] = true;
		isShowing[GRAVITY_UP] = true;
		isShowing[GRAVITY_DOWN] = true;
		isShowing[EXPLOSIVE_UP] = true;
		isShowing[EXPLOSIVE_DOWN] = true;
		isShowing[PART_PER_TICK_UP] = true;
		isShowing[PART_PER_TICK_DOWN] = true;
		isShowing[SPARK_TRAIL_UP] = true;
		isShowing[SPARK_TRAIL_DOWN] = true;
		break;
	case EXPLOSIVE_UP:
		explosiveness += 0.2;
		if(explosiveness > 100)
			explosiveness = 100;
		break;
	case EXPLOSIVE_DOWN:
		explosiveness -= 0.2;
		if(explosiveness < 0)
			explosiveness = 0;
		break;
	case PART_PER_TICK_UP:
		particlesPerTick += 1;
		if(particlesPerTick > 5)
			particlesPerTick = 5;
		break;
	case PART_PER_TICK_DOWN:
		particlesPerTick -= 1;
		if(particlesPerTick < 0)
			particlesPerTick = 0;
		break;
	case SPARK_TRAIL_UP:
		sparkliness -= 1;
		if(sparkliness < 2)
			sparkliness = 2;
		break;
	case SPARK_TRAIL_DOWN:
		sparkliness += 1;
		if(sparkliness > 25)
			sparkliness = 25;
		break;
	case SELECT_SHAPE_CIRCLE:
		particleShape = SELECT_SHAPE_CIRCLE;
		break;
	case SELECT_SHAPE_SQUARE:
		particleShape = SELECT_SHAPE_SQUARE;
		break;
	}
};

void createNewParticle(struct particle *newParticle){
	newParticle->age = 0;
	newParticle->x = mouseX; 
	newParticle->y = mouseY;
	newParticle->size = radius;
	newParticle->vx = ((rand()%FPS)/(float)FPS - 0.5)*explosiveness;
	newParticle->vy = ((rand()%FPS)/(float)FPS - 0.5)*explosiveness;
	newParticle->color = colorGen->getNextColor();
	newParticle->alive = true;
};

void createNewSparkle(struct particle *spark, struct particle *sparkle){
	sparkle->age = 0;
	sparkle->x = spark->x + (rand()%4 - 2); 
	sparkle->y = spark->y + (rand()%4 - 2);
	sparkle->size = 2;
	sparkle->vx = ((rand()%((int)(0.6*FPS)))/FPS - 0.3);
	sparkle->vy = ((rand()%((int)(0.6*FPS)))/FPS - 0.3);
	sparkle->color = spark->color;
	sparkle->alive = true;
};

void updateAsBall(struct particle *theParticle){
	theParticle->x += theParticle->vx;
	theParticle->y += theParticle->vy;
	theParticle->vy += gravity/FPS;
	
	if(theParticle->x + radius > width || theParticle->x - radius < 0){
		theParticle->vx *= co_of_restitution;
		if(theParticle->x < radius)
			theParticle->x = radius;
		else
			theParticle->x = width - radius;
	}
	if(theParticle->y + radius > height || theParticle->y - radius < 0){
		theParticle->vy *= co_of_restitution;
		if(theParticle->y + radius > height)
			theParticle->y = height - radius;
		else
			theParticle->y = radius;
	}
};

void updateAsSpark(struct particle * theParticle){
	unsigned char r, g, b;
	al_unmap_rgb(theParticle->color, &r, &g, &b);
	theParticle->color = al_map_rgba(r*(1000-theParticle->age)/1000,g*(1000-theParticle->age)/1000,b*(1000-theParticle->age)/1000, theParticle->age + 50);
	theParticle->age++;
	if(theParticle->age > 50){
		theParticle->x = -10;
		theParticle->vx = 0;
		theParticle->y = -10;
		theParticle->vy = 0;
		theParticle->alive = false;
	}

	if(theParticle->vx > 1 || theParticle->vx < -1)
		theParticle->vx *= 0.95;
	theParticle->vy *= 0.95;

	theParticle->x += theParticle->vx;
	theParticle->y += theParticle->vy;
	theParticle->vy += gravity/FPS * 0.8;

	if(theParticle->x + theParticle->size > width || theParticle->x - theParticle->size < 0){
		theParticle->vx *= co_of_restitution;
		if(theParticle->x - theParticle->size < 0)
			theParticle->x = theParticle->size;
		else
			theParticle->x = width - theParticle->size;
	}
	if(theParticle->y + theParticle->size > height){
		theParticle->vy *= co_of_restitution;
		if(theParticle->size != theParticle->y)
			theParticle->y = height - theParticle->size;
	}
};

void processBlackHoleGravity(struct particle *theParticle, struct particle *theBlackHole){
    double deltaX = (theParticle->x - theBlackHole->x);
    double deltaY = (theParticle->y - theBlackHole->y);
	float angle = atan2(deltaY,deltaX);
	double dist = 1+(int)abs(((int)theParticle->x - (int)theBlackHole->x)^2 + ((int)theParticle->y - (int)theBlackHole->y)^2);
    theParticle->vx += (-6 * cos(angle)/FPS);// / dist;
    theParticle->vy += (-6 * sin(angle)/FPS);// / dist;

};

void drawText(ALLEGRO_FONT *font24, ALLEGRO_COLOR color){
	if(isShowing[CO_OF_REST_UP]){
		al_draw_textf(font24, color, 15, 15, 0, "Coefficient of Restitution: %f", co_of_restitution * -1);
		al_draw_text(font24, color, 355, 15, 0, "+  -");
	}if(isShowing[GRAVITY_UP]){
		al_draw_textf(font24, color, 15, 45, 0, "Gravity: %f", gravity * -1);
		al_draw_text(font24, color, 195, 45, 0, "+  -");
	}if(isShowing[EXPLOSIVE_UP]){
		al_draw_textf(font24, color, 15, 75, 0, "Explosiveness: %f", explosiveness);
		al_draw_text(font24, color, 245, 75, 0, "+  -");
	}if(isShowing[PART_PER_TICK_UP]){
		al_draw_textf(font24, color, 15, 105, 0, "Particles Per Tick: %i", particlesPerTick);
		al_draw_text(font24, color, 230, 105, 0, "+  -");
	}if(isShowing[SPARK_TRAIL_UP]){
		al_draw_textf(font24, color, 15, 135, 0, "Spark Trail: %f", 1.0/sparkliness);
		al_draw_text(font24, color, 230, 135, 0, "+  -");
	}if(true){
		al_draw_text(font24, color, width-40, 15, ALLEGRO_ALIGN_RIGHT, "Ball");
		al_draw_text(font24, color, width-40, 45, ALLEGRO_ALIGN_RIGHT, "Spark");
		al_draw_text(font24, color, width-40, 75, ALLEGRO_ALIGN_RIGHT, "Fizzle");
		al_draw_text(font24, color, width-40, 105, ALLEGRO_ALIGN_RIGHT, "Circles");
		al_draw_text(font24, color, width-40, 135, ALLEGRO_ALIGN_RIGHT, "Frozen");
		al_draw_text(font24, color, width-40, 165, ALLEGRO_ALIGN_RIGHT, "Black Hole");
		al_draw_text(font24, color, width-40, 195, ALLEGRO_ALIGN_RIGHT, "Gravity Ball");
		al_draw_text(font24, color, width-40, height-25, ALLEGRO_ALIGN_RIGHT, "Square");
		al_draw_text(font24, color, width-40, height-55, ALLEGRO_ALIGN_RIGHT, "Circle");
	}
	al_draw_rectangle(width-24, (particleType-4)*30 + 20, width-16, (particleType-4)*30 + 29, al_map_rgb(200,0,100), 8);
	al_draw_rectangle(width-24, (particleShape-16)*30 + height-81, width-16, (particleShape-16)*30 + height-72, al_map_rgb(200,0,100), 8);
};