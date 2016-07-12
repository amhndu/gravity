![Gravity](assets/gravity.png)
=================================

**An Orbital survival game**

Project page: http://amhndu.github.io/Projects/gravity.html

Gravity is a survial game set in space, you are in a space station orbiting Earth but things have gone awry and you have a situation with broken satellites, how long can you survie ? (Yes, the premise is very similar to that Hollywood movie of the same name).  
Programmed in C++11 with the [SFML](http://sfml-dev.org) library for graphics. I made this is as my final project in school. This is a clone of the HTML5 game [Orbital Debris][orbital debris].

Screenshots
-----------------------------

![Screenshot 1](http://amhndu.github.io/Projects/screenshots/gravity1.jpg)
![Screenshot 2](http://amhndu.github.io/Projects/screenshots/gravity2.jpg)
![Screenshot 3](http://amhndu.github.io/Projects/screenshots/gravity3.jpg)
![Screenshot 4](http://amhndu.github.io/Projects/screenshots/gravity4.jpg)

Gameplay video
------------------------------

(Note that this video is a bit outdated since the game now supports saving/loading, apart from the new buttons this video remains precise)
[Youtube video](https://www.youtube.com/watch?v=LI_u30P6zTA)

Compiling
------------------------------

You need a C++11 compliant compiler and [SFML 2.0+](http://sfml-dev.org) to compile *Gravity*. After you've downloaded the source and dependencies, you can use [cmake](http://cmake.org) to compile *Gravity*.  
On any Linux distribution, these are the typical steps :  
```
$ git clone https://github.com/amhndu/gravity
$ cd gravity
$ cmake .
$ make
```
If you don't have cmake and don't wish to install it either, then you can simply compile Gravity by compiling all the cpp files and then linking them with SFML using your IDE/compiler. e.g. With g++, you can enter this command :
```
$ g++ -o gravity *.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system
```
Note : To compile on windows, you also need to link the sfml-main component.

License
--------------------------------
The following images have been taken from [here][orbital debris]
* assets/satellite\<n>.png where n = {0,1,2,3}
* assets/junk\<n>.png where n = {0,1,2}
* assets/Earth.png
* assets/bg.png
* assets/station.png

Other than the above no rights have been reserved, but in the unlikely event that you use anything, please drop me a [mail][mailaddr].

Contact me
---------------------------------
Email : amhndu --at-- gmail --period-- com


[orbital debris]: http://www.allworkallplay.org/games/orbitaldebris/
[repo]: https://github.com/amhndu/gravity
