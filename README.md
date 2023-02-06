# 8-Ball Pool

Created by Mikey Hayes Matt Gaetano and Gabe Atkins

Our program is trying to re-create the 8 ball pool game.  Our whole program runs in an OpenGL window, where it is
first initialized with the pool balls set up in a triangle with the cue ball ready to be hit into them.  We separated the game logic into a few screens, which we put into an enum to switch between.  The first screen starts 
the game, and shows just the balls on the table.  The user can then start the game which switches to a shot screen, 
where the pool cue is displayed and the user can choose their angle by moving the cursor around the screen, where the pool 
cue will be drawn a constant distance from the cue ball.  Once the user chooses the angle of their shot, another screen comes 
up for the user to choose the power of their shot.  The pool cue will be frozen during this screen and the user will be able to see 
the cue displayed with the angle they choose while they next choose the power.  Letting the user choose the power was done with two buttons,
one that increases the power of the shot and one that decreases the power.  Once the user has gotten to their desired power,
they can choose a third button that shoots the ball.  Once this button is chosen the screen changes again to hide the buttons and the pool cue.  All the balls collide with each other with non-elastic momentum, meaning that overall momentum is lost with each collision, and we also
implemented friction in order to simulate a real game.  Once all the balls stop moving, this process repeats, with the pool cue reappearing 
allowing for the user to shoot again.  This all continues until the balls are all pocketed at which point the game ends.

Concepts: 

We used concepts from modules 2 and 4 in this project.  From module 2 we used polymorphism and is-a relationships between classes to create our game.  
Everything drawn in our OpenGL window "is-a" shape, either a rect or a circle.  From there, the pool ball "is-a" circle and the pool cue and all the rectangles
making up the pool table itelf "are-a" rects.  This use of polymorphism let let us reuse large amounts of code across these different implementations of the sub-classes.
From module 4 we obviously used a lot of C++ graphics, as this is the whole output of our program.  We entirely used OpenGL, utiliing draw functions along with timers event listeners and cursor position to create our version of this game.

Operating Systems, Installations and Citations:

We ran this program on a Windows operating system entirely, the three of us all have windows machines so that's the only system its been tested on so far.  The only extra installations for this to run are freeGlut and OpenGL to make the graphics window work.  Beyond that we just used standard C++ libraries.  Our starting point was Lisa's screen-saver program from which we took the implementation of collisions and 2-D movement around a screen, modified for this project.

Please Fork!
