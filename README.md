FunWithParticles
================

This is a game/simulator that I started after having my wisdom teeth pulled. It started out as a bouncy ball simulator with random colors and has developed from there. This is, in my opinion, extremely fun to mess around with. By tinkering with the different variables and switching bewtween particle types (i.e. black hole -> frozen -> spark -> ball) you can make some really fascinating patterns (see video below or screenshots at bottom of readme for a few examples). 
### [Video demonstrating parts of this program](https://www.youtube.com/watch?v=ij0-msWfhqQ)
Features
========
### Particle Types
*  **Ball:** Bouncing balls that explode from your cursor and bounce of the walls and floor.
*  **Spark:** Like fireworks. Particles explode from your curosr leaving a trail of sparks that float whimsically toward the ground before dying out.
*  **Fizzle:** Originally a bug with spark that I kept. Basically just random concentrations of particles that fizzle out slowly. Its hard to describe....
*  **Circles** Cirlces whose center acts like a Ball, but whose radius is determined by its distance from the bottom.
*  **Frozen:** Particles don't update and look like balls.
*  **Black Hole:** All particles on screen (except for other black holes) will get pulled toward the black hole. While placing black holes, all other particles get frozen. Black Holes do not move.
*  **Gravity Ball:** All particles pull all other particles (except black holes) towards each other. Note: Gets very crazy very quickly.
*  **Flame:** A bunsen burner-like flame. Use Gravity to change its size. My friends were joking around with my fascination with the bunsen burner saying that there won't be bunsen burners in computer science. I proved them wrong.
*  **Spikes:** See screenshot left of flame. This makes 3Dish spike balls. This was inspired by [this gif](http://i.imgur.com/CCDduR6) and is my personal favorite.

### Variables
* **Coefficient of Restitution:** How much speed particles retain after bouncing off a wall.
* **Gravity:** Downward acceleration of particles.
* **Explosiveness:** Initial speed of particles. Set to 0 for particles to have initial speed of 0. Use the sliders to affect the direction the particles initially go in.
* **Particles Per Tick:** Number of particles released from cursor every 120th of a second.
* **Sparkliness:** Density of spark trail when using spark. Also affects fizzle.
* **# of Particles:** Maximum number of particles. Use this to limit the particles if it is lagging.
* **Radius:** Radius of particles. Does not change radius of particles that have already been created.
* **Realistic Gravity:** This will make gravity of particles stronger when they are closer together
* **Pretty Gravity:** Distance is not taken into account. This makes you able to make pretty things that look like solar systems.
* **Don't Erase:** This stops the program from erasing between frames. This leads to some pretty cool effects. 
* **Circle/Square:** Change shape of particles.

### Colors
* Uses HSV because it is superior to RGB for making pretty colors.
* Randomly selects color from selected ranges of H, S, and V.
* Change range by moving top and bottom sliders.

Tips
====
* Move S and V to the right for brighter colors.
* Move S to center for pastel colors.
* Set explosivness to 0 to see some cool stuff. Especially when using Circles or Black Holes.
* Fireworks are fun.
* Read the directions at the start...

Running the Program
===================
This program was written in C++ with the Allgro 5 library, so it should be compatible with all C++ compilers that have Allegro 5 installed.

I use minGW (with Allegro5), navigate to the folder and run:

`g++ main.cpp -o FunWithParticles.exe -lallegro-5.0.8-monolith-md`

Screenshots
===========

![screen2](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%283%29.png)

| Screenshots! | Yay!! |
| ------------ | ----- |
| ![screen3](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%282%29.png) | ![screen4](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%284%29.png) |
| ![screen5](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%285%29.png) | ![screen6](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%286%29.png) |
| ![screen7](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%287%29.png) | ![screen8](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%288%29.png) |
| ![screen9](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%289%29.png) | ![screen10](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2810%29.png) |
| ![screen11](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2861%29.png) | ![screen12](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2812%29.png) |
| ![screen13](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2815%29.png) | ![screen14](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2821%29.png) |
