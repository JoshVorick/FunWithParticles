#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "Button.h"

enum buttons{CO_OF_REST_UP, CO_OF_REST_DOWN, GRAVITY_UP, GRAVITY_DOWN, SELECT_BALL, SELECT_SPARK, SELECT_FROZEN,
	EXPLOSIVE_UP, EXPLOSIVE_DOWN, PART_PER_TICK_UP, PART_PER_TICK_DOWN};
const int NUM_BUTTONS = 11;

const double golden_ratio = 0.618033988749895;
const int FPS = 60;
const int maxParticles = 10000;
const int width = 800;
const int height = 800;

void processButtonClick(Button *button, int i);
void createNewParticle(struct particle *newParticle);
void updateAsBall(struct particle *theParticle);
void updateAsSpark(struct particle *theParticle);

struct particle{
	ALLEGRO_COLOR color;
	float x, y, vx, vy;
	int age;
};

float co_of_restitution = -0.95;
float gravity = 6;
float explosiveness = 10;
int mouseX = 0;
int mouseY = 0;
int particleType = SELECT_BALL; //i.e. ball, spark, etc.
int particlesPerTick = 1;

int main(void){
	struct particle liveParticles[maxParticles];
	int curParticle = 0;
	bool mouseDown = false;
	bool done = false;
	bool redraw = true;
	Button *buttons[NUM_BUTTONS];

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font24;

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
	
	buttons[CO_OF_REST_UP] = new Button(350,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[CO_OF_REST_DOWN] = new Button(385,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[GRAVITY_UP] = new Button(190,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[GRAVITY_DOWN] = new Button(225,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_BALL] = new Button(width-30,15,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_SPARK] = new Button(width-30,45,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[SELECT_FROZEN] = new Button(width-30,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[EXPLOSIVE_UP] = new Button(240,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[EXPLOSIVE_DOWN] = new Button(275,75,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[PART_PER_TICK_UP] = new Button(225,105,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	buttons[PART_PER_TICK_DOWN] = new Button(260,105,20,20,al_map_rgb(100,0,200),al_map_rgb(100,0,100),al_map_rgb(200,0,100));
	
	font24 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 24, 0);
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/FPS);
	
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	while(!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		switch(ev.type){
		case ALLEGRO_EVENT_TIMER:
			redraw = true;

			for(int i=0; i<NUM_BUTTONS; i++){
				if(buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown))
					if(buttons[i]->isBeingHeld())
						processButtonClick(buttons[i], i);
			}
			
			if(mouseDown){
				for(int i=0; i<particlesPerTick; i++){
					createNewParticle(&liveParticles[curParticle]);
					curParticle = (curParticle+1) % maxParticles;
				}
			}
			for(int i=0;i<maxParticles;i++){
				if(particleType == SELECT_BALL)
					updateAsBall(&liveParticles[i]);
				else if(particleType == SELECT_SPARK)
					updateAsSpark(&liveParticles[i]);
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			mouseDown = true;
			for(int i=0; i<NUM_BUTTONS; i++){
				if(buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown))
					processButtonClick(buttons[i], i);
			}
			createNewParticle(&liveParticles[curParticle]);
			curParticle = (curParticle+1) % maxParticles;
			break;
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
			mouseDown = false;
			for(int i=0; i<NUM_BUTTONS; i++){
				buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown);
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
			al_draw_rectangle(mouseX,mouseY,mouseX,mouseY,al_map_rgb(0,0,100), 20);
			for(int i=0; i<maxParticles; i++){
				al_draw_rounded_rectangle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].x, liveParticles[i].y, 1, 1, liveParticles[i].color, 5);
			}
			for(int i=0; i<NUM_BUTTONS; i++){
				buttons[i]->draw();
			}
			
			al_draw_textf(font24, al_map_rgb(50,250,50), 15, 15, 0, "Coefficient of Restitution: %f", co_of_restitution * -1);
			al_draw_text(font24, al_map_rgb(50,250,50), 355, 15, 0, "+  -");
			al_draw_textf(font24, al_map_rgb(50,250,50), 15, 45, 0, "Gravity: %f", gravity * -1);
			al_draw_text(font24, al_map_rgb(50,250,50), 195, 45, 0, "+  -");
			al_draw_textf(font24, al_map_rgb(50,250,50), 15, 75, 0, "Explosiveness: %f", explosiveness);
			al_draw_text(font24, al_map_rgb(50,250,50), 245, 75, 0, "+  -");
			al_draw_textf(font24, al_map_rgb(50,250,50), 15, 105, 0, "Particles Per Tick: %i", particlesPerTick);
			al_draw_text(font24, al_map_rgb(50,250,50), 230, 105, 0, "+  -");
			al_draw_text(font24, al_map_rgb(50,250,50), width-40, 15, ALLEGRO_ALIGN_RIGHT, "Ball");
			al_draw_text(font24, al_map_rgb(50,250,50), width-40, 45, ALLEGRO_ALIGN_RIGHT, "Spark");
			al_draw_text(font24, al_map_rgb(50,250,50), width-40, 75, ALLEGRO_ALIGN_RIGHT, "Frozen");
			al_draw_rectangle(width-23, (particleType-4)*30 + 21, width-17, (particleType-4)*30 + 28, al_map_rgb(100,100,100), 6);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
}

void processButtonClick(Button *button, int i){
	switch(i){
	case CO_OF_REST_UP:
		co_of_restitution -= 0.05;
		if(co_of_restitution < -1)
			co_of_restitution = -1;
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
		break;
	case SELECT_SPARK:
		particleType = SELECT_SPARK;
		break;
	case SELECT_FROZEN:
		particleType = SELECT_FROZEN;
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
	}
};

void createNewParticle(struct particle *newParticle){
	newParticle->age = 0;
	newParticle->x = mouseX; 
	newParticle->y = mouseY;
	newParticle->vx = ((rand()%100)/100.0 - 0.5)*explosiveness;
	newParticle->vy = ((rand()%100)/100.0 - 0.5)*explosiveness;
	newParticle->color = al_map_rgb(rand(), rand(), rand());
};

void updateAsBall(struct particle *theParticle){
	theParticle->x += theParticle->vx;
	theParticle->y += theParticle->vy;
	theParticle->vy += gravity/FPS;

	if(theParticle->x > width || theParticle->x < 0){
		theParticle->vx *= co_of_restitution;
		if(theParticle->x < 0)
			theParticle->x = 0;
		else
			theParticle->x = width;
	}
	if(theParticle->y > height || theParticle->y < 0){
		theParticle->vy *= co_of_restitution;
		if(theParticle->y < 0)
			theParticle->y = 0;
		else
			theParticle->y = height;
	}
};

void updateAsSpark(struct particle * theParticle){
	unsigned char r, g, b;
	al_unmap_rgb(theParticle->color, &r, &g, &b);
	theParticle->color = al_map_rgba(r*(1000-theParticle->age)/1000,g*(1000-theParticle->age)/1000,b*(1000-theParticle->age)/1000, theParticle->age+50);
	theParticle->age++;
	if(theParticle->age > 600){
		theParticle->x = -10;
		theParticle->vx = 0;
	}

	theParticle->vx *= 0.95;
	theParticle->vy *= 0.95;

	theParticle->x += theParticle->vx;
	theParticle->y += theParticle->vy;
	theParticle->vy += gravity/FPS * 0.6;

	if(theParticle->x > width || theParticle->x < 0){
		theParticle->vx *= co_of_restitution;
		if(theParticle->x < 0)
			theParticle->x = 0;
		else
			theParticle->x = width;
	}
	if(theParticle->y > height || theParticle->y < 0){
		theParticle->vy *= co_of_restitution;
		if(theParticle->y < 0)
			theParticle->y = 0;
		else
			theParticle->y = height;
	}
};