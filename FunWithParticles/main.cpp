#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "Button.h"
#include "ColorGenerator.h"

using namespace std;

enum buttons{CO_OF_REST_UP, CO_OF_REST_DOWN, GRAVITY_UP, GRAVITY_DOWN, SELECT_BALL, SELECT_SPARK, SELECT_FIZZLE, SELECT_CIRCLES, 
	SELECT_FROZEN, SELECT_BLACKHOLE, SELECT_GRAVITY_BALL, SELECT_BUNSEN, 
	EXPLOSIVE_UP, EXPLOSIVE_DOWN, PART_PER_TICK_UP, PART_PER_TICK_DOWN, SPARK_TRAIL_UP, SPARK_TRAIL_DOWN, 
	PARTICLE_NUMBER_UP, PARTICLE_NUMBER_DOWN, RADIUS_UP, RADIUS_DOWN, SELECT_SHAPE_CIRCLE, SELECT_SHAPE_SQUARE}; //names of all the buttons

const int NUM_BUTTONS = 24; //number of buttons

const int FPS = 120; //FPS of game
const int width = 800; //Screen width
const int height = 800; //Screen height

void processButtonClick(Button *button, int i); //Does stuff based on which button was clicked
void createNewParticle(struct particle *newParticle); //Creates a non-spark, non-flame particle
void createNewSparkle(struct particle *spark, struct particle *sparkle); //Creates a spark (the little trailing spark)
void createNewBunsen(struct particle *newFlame); //Creates a flame particle
void updateAsBall(struct particle *theParticle); //Updates non-spark, non-flame particles
void updateAsSpark(struct particle *theParticle); //Updates sparks (All particles, not just trailing ones)
void updateAsBunsen(struct particle *flame); //Updates flame particles
void processBlackHoleGravity(struct particle *theParticle, struct particle *theBlackHole); //Changes theParticle's vx and vy based on where blackHole is
void drawText(ALLEGRO_FONT *font24, ALLEGRO_COLOR color, ALLEGRO_COLOR buttonColor); //draws the text and fills in selected buttons

struct particle{
	ALLEGRO_COLOR color;
	float size, x, y, vx, vy, age;
	bool alive;
};
int gameTime = 0;
float co_of_restitution = -0.85; //bounciness
float gravity = 9.8; //downward acceleration
float explosiveness = 15;
int mouseX = 0;
int mouseY = 0;
int particleType = SELECT_BALL; //which type the particle is (buttons on top right)
int particleShape = SELECT_SHAPE_CIRCLE; //What shape particles are drawn as (buttons on bottom left)
int particlesPerTick = 1; //How many particles to add per tick (1/FPS of a second)
int sparkliness = 5; //Density of Spark's trail
int particleNumber = 10; //Affects maximum size of vector<struct particle> liveParticles
int radius = 5; //Radius of particles when drawn
int bunsenRadius = 40; //Width of bottom of flame
bool isShowing[NUM_BUTTONS]; //Used to hide some buttons from screen when using different particle types
bool showButtons = true; //Used to hide all buttons
ColorGenerator *colorGen; //Generates colors for the particles
bool showColors = true; //Used to hide color sliders (held in ColorGenerator class)

int main(void){
	vector<struct particle> liveParticles; //Holds all non-blackhole particles
	vector<struct particle> vblackHoles; //Holds black holes

	int curSparkle = 0; //Used with sparkliness to change density of spark trail
	bool mouseDown = false; //Is left click held down
	bool done = false; //Game ends when done = true
	bool redraw = true; //Doesn't redraw screen (as often) if event queue is full
	bool start = false; //Changes from start menu to game when start = true
	bool isUsingButton; //If a player is using ANY button/slider, no new particles are created
	bool saveNewBackground = false; //Used to save background when player presses 'S'
	Button *buttons[NUM_BUTTONS]; //Array of buttons

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font24;
	ALLEGRO_COLOR limegreen; //Text color
	ALLEGRO_COLOR button1, button2, button3;
	ALLEGRO_BITMAP *background;

	al_init(); //Initialize Allegro

	//Create display window
	//al_set_new_display_flags(ALLEGRO_RESIZABLE);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	display = al_create_display(width, height);
	al_set_window_position(display, 0, 0);
	al_set_window_title(display, "Particle Simulator");

	//Install addons. MUST be done BEFORE using the addons' functions. 
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	//Initialize ALLEGRO variables
	font24 = al_load_font("Fonts/A_Sensible_Armadillo.ttf", 24, 0);
	limegreen = al_map_rgb(50,250,50);
	button1 = al_map_rgb(200,0,100);
	button2 = al_map_rgb(100,0,100);
	button3 = al_map_rgb(100,0,200);
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/FPS);
	background = al_create_bitmap(width, height); //Create background image
	al_set_target_bitmap(background); //Set bg to be the one drawn on
	al_clear_to_color(al_map_rgb(10,10,10)); //Clear bg to black
	al_set_target_bitmap(al_get_backbuffer(display)); //Set display back to display

	//Initialize each button
	buttons[CO_OF_REST_UP] = new Button(15,15,20,20,button1,button2,button3);
	buttons[CO_OF_REST_DOWN] = new Button(50,15,20,20,button1,button2,button3);
	buttons[GRAVITY_UP] = new Button(15,45,20,20,button1,button2,button3);
	buttons[GRAVITY_DOWN] = new Button(50,45,20,20,button1,button2,button3);
	buttons[SELECT_BALL] = new Button(width-30,15,20,20,button1,button2,button3);
	buttons[SELECT_SPARK] = new Button(width-30,45,20,20,button1,button2,button3);
	buttons[SELECT_FIZZLE] = new Button(width-30,75,20,20,button1,button2,button3);
	buttons[SELECT_CIRCLES] = new Button(width-30,105,20,20,button1,button2,button3);
	buttons[SELECT_FROZEN] = new Button(width-30,135,20,20,button1,button2,button3);
	buttons[SELECT_BLACKHOLE] = new Button(width-30,165,20,20,button1,button2,button3);
	buttons[SELECT_GRAVITY_BALL] = new Button(width-30,195,20,20,button1,button2,button3);
	buttons[SELECT_BUNSEN] = new Button(width-30,225,20,20,button1,button2,button3);
	buttons[EXPLOSIVE_UP] = new Button(15,75,20,20,button1,button2,button3);
	buttons[EXPLOSIVE_DOWN] = new Button(50,75,20,20,button1,button2,button3);
	buttons[PART_PER_TICK_UP] = new Button(15,105,20,20,button1,button2,button3);
	buttons[PART_PER_TICK_DOWN] = new Button(50,105,20,20,button1,button2,button3);
	buttons[SPARK_TRAIL_UP] = new Button(15,135,20,20,button1,button2,button3);
	buttons[SPARK_TRAIL_DOWN] = new Button(50,135,20,20,button1,button2,button3);
	buttons[PARTICLE_NUMBER_UP] = new Button(15,165,20,20,button1,button2,button3);
	buttons[PARTICLE_NUMBER_DOWN] = new Button(50,165,20,20,button1,button2,button3);
	buttons[RADIUS_UP] = new Button(15,195,20,20,button1,button2,button3);
	buttons[RADIUS_DOWN] = new Button(50,195,20,20,button1,button2,button3);
	buttons[SELECT_SHAPE_CIRCLE] = new Button(width-30,height-55,20,20,button1,button2,button3);
	buttons[SELECT_SHAPE_SQUARE] = new Button(width-30,height-25,20,20,button1,button2,button3);

	//Set all buttons to be showing
	for(int i=0; i<NUM_BUTTONS; i++)
		isShowing[i] = true;
	processButtonClick(buttons[SELECT_BALL], SELECT_BALL); //Set intial values to ball selection

	//Register event sources
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	
	colorGen = new ColorGenerator(515,0, al_map_rgb(250,250,250)); //Init color generator

	al_start_timer(timer);
	
	//Start menu loop
	while(!start){
		//Wait for an event (timer tick, keyboard, mouse, etc.)
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		switch(ev.type){
		case ALLEGRO_EVENT_TIMER:
			redraw = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE){
				start = true; //Exits loop, starts game
			}else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				start = true; //Exits loop
				done = true; //Doesn't even start game
			}
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE: //Red X is clicked
			start = true; //Exits loops
			done = true; //Doesn't start game
			break;
		}
		if(redraw && al_is_event_queue_empty(event_queue)){ //Draw start menu
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
		//Wait for event
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		switch(ev.type){
		case ALLEGRO_EVENT_TIMER: //Do all updating goop
			redraw = true; //Resets redraw to true (this way game draws at a more consistent rate)
			isUsingButton = false;
			gameTime++;

			if(showColors) 
				if(colorGen->processMouseCoor(mouseX, mouseY, mouseDown)) //True if slider is being moved
					isUsingButton = true; //If a slider is being used, sets this to true

			if(showButtons)
				for(int i=0; i<NUM_BUTTONS; i++){
					if(buttons[i]->processMouseCoor(mouseX, mouseY, mouseDown)){ //True if button is clicked
						if(buttons[i]->isBeingHeld())
							processButtonClick(buttons[i], i); //If clicked, processes the click
						isUsingButton = true; 
					}
				}
				
			//Trim array based on particle type and number of black holes to limit lag
			if(particleType == SELECT_GRAVITY_BALL)
				while(liveParticles.size() + vblackHoles.size() > 40 * particleNumber){
					liveParticles.erase(liveParticles.begin());
				}
			else if(particleType == SELECT_FIZZLE)
				while(liveParticles.size()*(vblackHoles.size()+1) > 200 * particleNumber){
					liveParticles.erase(liveParticles.begin());
				}
			else
				while(liveParticles.size()*(vblackHoles.size()+1) > 3000 * particleNumber){
					liveParticles.erase(liveParticles.begin());
				}
			
			//If mouse is clicked, and they're not using a button, and black hole isn't selected, create particles
			if(mouseDown && !isUsingButton && particleType != SELECT_BLACKHOLE){
				for(int i=0; i<particlesPerTick; i++){ //Creates multiple particles per tick if needed
					if(particleType == SELECT_BUNSEN){ //If type=flame, creates more particles
						for(int i=0; i<30;i++){
							struct particle newParticle;
							createNewBunsen(&newParticle); //flame specific constructor
							liveParticles.push_back(newParticle);
						}
					}else{
						struct particle newParticle;
						createNewParticle(&newParticle);
						liveParticles.push_back(newParticle);
					}
				}
			}
			//Update particles
			if(particleType != SELECT_FROZEN){ //Don't update if FROZEN is selected
				for(int i=0;i<liveParticles.size();i++){ //Iterates through all particles
					for(int j=0;j<vblackHoles.size();j++){ //Iterate through black holes
						processBlackHoleGravity(&liveParticles[i], &vblackHoles[j]); //update velocity based on position relative to black hole
					}
					if(particleType == SELECT_BALL)
						updateAsBall(&liveParticles[i]); //Update as a ball
					else if(particleType == SELECT_CIRCLES){
						updateAsBall(&liveParticles[i]); //Update as a circle (same as ball)
						liveParticles[i].size = height - liveParticles[i].y; //Change size so that bottom of circle is touching the floor
					}else if(particleType == SELECT_BUNSEN){
						updateAsBunsen(&liveParticles[i]); //Update as flame
					}else if(particleType == SELECT_SPARK){
						updateAsSpark(&liveParticles[i]); //Update as spark
						if(liveParticles[i].size == radius && curSparkle == 0){ //Create a new trailing if the particle isn't a trailing spark (has original radius) and if curSparkle=0 (based on sparkliness)
							struct particle newSparkle;
							createNewSparkle(&liveParticles[i], &newSparkle); //Create new trailing spark
							liveParticles.push_back(newSparkle);
						}
					}else if(particleType == SELECT_FIZZLE){ //Not really sure how it works... Ooops...
						updateAsSpark(&liveParticles[i]);
						if(i % sparkliness == 0){
							struct particle newSparkle;
							createNewSparkle(&liveParticles[i], &newSparkle);
							liveParticles.push_back(newSparkle);
						}
						if(liveParticles[i].alive == false){
							liveParticles.erase(liveParticles.begin()+i); //Remove dead flame
							i--; //Subtract one so that the next element doesn't get skipped
						}
					}else if(particleType == SELECT_GRAVITY_BALL){
						updateAsBall(&liveParticles[i]); //Updates like a ball
						for(int j=i+1; j<liveParticles.size();j++){
							processBlackHoleGravity(&liveParticles[i],&liveParticles[j]); //Calculate direction i is pulled by j
							processBlackHoleGravity(&liveParticles[j],&liveParticles[i]); //Calculate direction j is pulled by i
						}
					}
				}
			}
			liveParticles.erase(std::remove_if (liveParticles.begin(), liveParticles.end(), [](const particle p) {return p.alive != true;}), liveParticles.end());

			curSparkle = (curSparkle + 1) % sparkliness; //Lower sparkliness means  curSparkle will be 0 more often, meaning more sparkles will be created

			//Delete all timer events that piled up, so that timer events don't jam the queue and mouse movements can be processed (and screen can be redrawn)
			ALLEGRO_EVENT ev2;
			al_peek_next_event(event_queue, &ev2);
			while(ev2.type == ALLEGRO_EVENT_TIMER && !al_is_event_queue_empty(event_queue)){
				al_drop_next_event(event_queue);
				al_peek_next_event(event_queue, &ev2);
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			mouseX = ev.mouse.x;
			mouseY = ev.mouse.y;
			ALLEGRO_EVENT ev3;
			al_peek_next_event(event_queue, &ev3);
			while(ev3.type == ALLEGRO_EVENT_TIMER && !al_is_event_queue_empty(event_queue)){
				al_drop_next_event(event_queue);
				al_peek_next_event(event_queue, &ev3);
			}
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
					if(particleType == SELECT_BUNSEN)
						createNewBunsen(&newParticle);
					else
						createNewParticle(&newParticle);
					liveParticles.push_back(newParticle);
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
			case ALLEGRO_KEY_8:
				processButtonClick(buttons[SELECT_BUNSEN], SELECT_BUNSEN);
				break;
			case ALLEGRO_KEY_S:
				saveNewBackground = true;
				break;
			case ALLEGRO_KEY_X:
				for(int i=0; i<liveParticles.size(); i++){
					liveParticles.clear();
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
		if(redraw && (al_is_event_queue_empty(event_queue))){
			redraw = false;
			al_draw_bitmap(background, 0, 0, 0);

			if(saveNewBackground){
				al_set_target_bitmap(background);
				al_clear_to_color(al_map_rgb(0,0,0));
			}

			for(int i=0; i<vblackHoles.size(); i++){
				al_draw_filled_circle(vblackHoles[i].x, vblackHoles[i].y, vblackHoles[i].size, al_map_rgb(255, 255, 255));
			}
			if(particleType == SELECT_BUNSEN){
				if(particleShape == SELECT_SHAPE_CIRCLE){
					for(int i=0; i<liveParticles.size(); i++)
						if(liveParticles[i].age != 0)
							al_draw_filled_circle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].size-1, liveParticles[i].color);
					for(int i=0; i<liveParticles.size(); i++)
						if(liveParticles[i].age == 0)
							al_draw_filled_circle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].size-1, liveParticles[i].color);
				}else{
					for(int i=0; i<liveParticles.size(); i++)
						if(liveParticles[i].age != 0)
							al_draw_filled_rectangle(liveParticles[i].x-liveParticles[i].size, liveParticles[i].y-liveParticles[i].size, 
								liveParticles[i].size+liveParticles[i].x, liveParticles[i].size+liveParticles[i].y, liveParticles[i].color);
					for(int i=0; i<liveParticles.size(); i++)
						if(liveParticles[i].age == 0)
							al_draw_filled_rectangle(liveParticles[i].x-liveParticles[i].size, liveParticles[i].y-liveParticles[i].size, 
								liveParticles[i].size+liveParticles[i].x, liveParticles[i].size+liveParticles[i].y, liveParticles[i].color);
				}
			}else{
				if(particleShape == SELECT_SHAPE_CIRCLE)
					for(int i=0; i<liveParticles.size(); i++)
						al_draw_filled_circle(liveParticles[i].x, liveParticles[i].y, liveParticles[i].size, liveParticles[i].color);
				else
					for(int i=0; i<liveParticles.size(); i++)
						al_draw_filled_rectangle(liveParticles[i].x-liveParticles[i].size, liveParticles[i].y-liveParticles[i].size, 
								liveParticles[i].size+liveParticles[i].x, liveParticles[i].size+liveParticles[i].y, liveParticles[i].color);
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
				drawText(font24, limegreen, button3);
			
			al_draw_textf(font24, limegreen, 10, height-60, 0, "Number of Balls: %i", liveParticles.size());
			al_draw_textf(font24, limegreen, 10, height-30, 0, "Number of Black Holes: %i", vblackHoles.size());

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
		break;
	case SELECT_SPARK:
		particleType = SELECT_SPARK;
		break;
	case SELECT_FIZZLE:
		particleType = SELECT_FIZZLE;
		break;
	case SELECT_CIRCLES:
		particleType = SELECT_CIRCLES;
		break;
	case SELECT_FROZEN:
		particleType = SELECT_FROZEN;
		break;
	case SELECT_BLACKHOLE:
		particleType = SELECT_BLACKHOLE;
		break;
	case SELECT_GRAVITY_BALL:
		particleType = SELECT_GRAVITY_BALL;
		break;
	case SELECT_BUNSEN:
		particleType = SELECT_BUNSEN;
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
		if(particlesPerTick > 10)
			particlesPerTick = 10;
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
	case PARTICLE_NUMBER_UP:
		particleNumber += 1;
		if(particleNumber > 30)
			particleNumber = 30;
		break;
	case PARTICLE_NUMBER_DOWN:
		particleNumber -= 1;
		if(particleNumber < 0)
			particleNumber = 0;
		break;
	case RADIUS_UP:
		radius += 1;
		if(radius > 100)
			radius = 100;
		break;
	case RADIUS_DOWN:
		radius -= 1;
		if(radius < 2)
			radius = 2;
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
	sparkle->size = radius/2;
	sparkle->vx = ((rand()%((int)(0.6*FPS)))/FPS - 0.3);
	sparkle->vy = ((rand()%((int)(0.6*FPS)))/FPS - 0.3);
	sparkle->color = spark->color;
	sparkle->alive = true;
};

void createNewBunsen(struct particle *newFlame){
	float d = (rand()%10000) / (5000.0/bunsenRadius);
	newFlame->x = mouseX+d-bunsenRadius; 
	newFlame->y = mouseY;
	newFlame->size = radius;
	newFlame->color = al_map_rgb(5,5,80);
	newFlame->alive = true;

	if(d>bunsenRadius)
		newFlame->vy = (bunsenRadius*2-d)*-0.1;
	else
		newFlame->vy = d*-0.1;
	if(!(rand()%6)){
		newFlame->age = 0;
		newFlame->vx = 0;
	}else{
		if(d>bunsenRadius)
			newFlame->vx = (d-bunsenRadius)*0.05;
		else if(d==bunsenRadius)
			newFlame->vx = 0 + sin(mouseY/180.0)*0.1;
		else
			newFlame->vx = (bunsenRadius-d)*-0.05;
		newFlame->age = d;
		newFlame->vy *= 2;
	}
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
	if(theParticle->age > 55){
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

void updateAsBunsen(struct particle *flame){
	flame->x += flame->vx + 0.3*sin((gameTime + flame->y/2)/40);
	flame->y += flame->vy;
	flame->vy += gravity/FPS;
	
	if(flame->x + radius > width || flame->x - radius < 0){
		flame->vx *= co_of_restitution;
		if(flame->x < radius)
			flame->x = radius;
		else
			flame->x = width - radius;
	}
	if(flame->y + radius > height || flame->y - radius < 0){
		flame->vy *= co_of_restitution;
		if(flame->y + radius > height)
			flame->y = height - radius;
		else
			flame->y = radius;
	}

	if(flame->vy > 0.01)
		flame->alive = false;
	if(flame->age == 0 && flame->vy > -4.9){
		int rg = 150-(flame->vy - 0.01)*-30;
		flame->color = al_map_rgb(rg/3, rg/3, 125);
	}else{
		if(flame->age == bunsenRadius){}
		else if(flame->age > bunsenRadius){
			flame->vx -= (2*bunsenRadius-flame->age)*0.00005;
		}else{
			flame->vx +=  (flame->age) * 0.00005;
		}
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

void drawText(ALLEGRO_FONT *font24, ALLEGRO_COLOR color, ALLEGRO_COLOR buttonColor){
	if(isShowing[CO_OF_REST_UP]){
		al_draw_textf(font24, color, 75, 15, 0, "Coefficient of Restitution: %f", co_of_restitution * -1);
		al_draw_text(font24, color, 20, 15, 0, "+  -");
	}if(isShowing[GRAVITY_UP]){
		al_draw_textf(font24, color, 75, 45, 0, "Gravity: %f", gravity * -1);
		al_draw_text(font24, color, 20, 45, 0, "+  -");
	}if(isShowing[EXPLOSIVE_UP]){
		al_draw_textf(font24, color, 75, 75, 0, "Explosiveness: %f", explosiveness);
		al_draw_text(font24, color, 20, 75, 0, "+  -");
	}if(isShowing[PART_PER_TICK_UP]){
		al_draw_textf(font24, color, 75, 105, 0, "Particles Per Tick: %i", particlesPerTick);
		al_draw_text(font24, color, 20, 105, 0, "+  -");
	}if(isShowing[SPARK_TRAIL_UP]){
		al_draw_textf(font24, color, 75, 135, 0, "Spark Trail: %f", 1.0/sparkliness);
		al_draw_text(font24, color, 20, 135, 0, "+  -");
	}if(isShowing[PARTICLE_NUMBER_UP]){
		al_draw_textf(font24, color, 75, 165, 0, "# Particles: %i", particleNumber);
		al_draw_text(font24, color, 20, 165, 0, "+  -");
	}if(isShowing[RADIUS_UP]){
		al_draw_textf(font24, color, 75, 195, 0, "Radius: %i", radius);
		al_draw_text(font24, color, 20, 195, 0, "+  -");
	}if(true){
		al_draw_text(font24, color, width-40, 15, ALLEGRO_ALIGN_RIGHT, "Ball");
		al_draw_text(font24, color, width-40, 45, ALLEGRO_ALIGN_RIGHT, "Spark");
		al_draw_text(font24, color, width-40, 75, ALLEGRO_ALIGN_RIGHT, "Fizzle");
		al_draw_text(font24, color, width-40, 105, ALLEGRO_ALIGN_RIGHT, "Circles");
		al_draw_text(font24, color, width-40, 135, ALLEGRO_ALIGN_RIGHT, "Frozen");
		al_draw_text(font24, color, width-40, 165, ALLEGRO_ALIGN_RIGHT, "Black Hole");
		al_draw_text(font24, color, width-40, 195, ALLEGRO_ALIGN_RIGHT, "Gravity Ball");
		al_draw_text(font24, color, width-40, 225, ALLEGRO_ALIGN_RIGHT, "Flame");
		al_draw_text(font24, color, width-40, height-25, ALLEGRO_ALIGN_RIGHT, "Square");
		al_draw_text(font24, color, width-40, height-55, ALLEGRO_ALIGN_RIGHT, "Circle");
	}
	al_draw_rectangle(width-24, (particleType-4)*30 + 20, width-16, (particleType-4)*30 + 29, buttonColor, 8);
	al_draw_rectangle(width-24, (particleShape-NUM_BUTTONS)*30 + height+9, width-16, (particleShape-NUM_BUTTONS)*30 + height+18, buttonColor, 8);
};