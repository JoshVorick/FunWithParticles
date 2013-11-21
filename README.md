FunWithParticles
================

This is a game/simulator that I started after having my wisdom teeth pulled after reading a neat article on algorithmic color generation. It started out as a bouncy ball simulator with random colors and has developed from there.

### ![Super Amazing YT video showing off this program!](https://www.youtube.com/watch?v=ij0-msWfhqQ)

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

### Variables
* **Coefficient of Restitution:** How much speed particles retain after bouncing off a wall.
* **Gravity:** Downward acceleration of particles.
* **Explosiveness** Initial speed of particles. Set to 0 for particles to have initial speed of 0.
* **Particles Per Tick:** Number of particles released from cursor every 120th of a second.
* **Sparkliness** Density of spark trail when using spark. Also affects fizzle.
* **# of Particles** Maximum number of particles. Use this to limit the particles if it is lagging.
* **Radius:** Radius of particles. Does not change radius of particles that have already been created.
* **Circle/Square** Change shape of particles.

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
For now, you have to have the Allegro 5 library installed on Microsoft Visual Studio (C++). Then just press F5 after opening the solution!

Screenshots
===========

![screen4](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%284%29.png)
![screen1](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%281%29.png)
![screen5](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%285%29.png)
![screen3](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%283%29.png)
![screen2](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%282%29.png)
![screen6](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%286%29.png)
![screen7](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%287%29.png)
![screen8](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%288%29.png)
![screen9](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%289%29.png)
![screen10](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2810%29.png)
![screen11](https://raw.github.com/TheBananaWhale/FunWithParticles/master/img/Screenshot%20%2811%29.png)
