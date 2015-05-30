Gravity
=======
![Gravity](assets/gravity.png)

**An Orbital survival game**

A game I made for my school project.
The objective is to survive as long as you can in your space station avoiding other satellites and junk.
This is a clone of the HTML5 game [Orbital Debris][orbital debris].

###Screenshots

(Note that these screenshots are outdated since the game now supports saving/loading , apart from the new buttons they are correct)
http://i.imgur.com/4enX4oE.png  
http://i.imgur.com/EkbtdSM.png  
http://i.imgur.com/QEY2wz3.png  
http://i.imgur.com/cFigPKF.png  

###Gameplay video

(Note that this vidoe is outdated since the game now supports saving/loading , apart from the new buttons they are correct)
[Youtube video](https://www.youtube.com/watch?v=LI_u30P6zTA)

###Compiling
Once you've downloaded the source from the [repository][repo] , run cmake and then compile using your corresponding make system or IDE.
On a linux system these are the typical steps
```sh
$ git clone https://github.com/amhndu/gravity
$ cd gravity
$ cmake .
$ make
```
Now run it
```sh
$ ./gravity
```

###License
The following images have been taken from [here][orbital debris]
* assets/satellite\<n>.png where n = {0,1,2,3}
* assets/junk\<n>.png where n = {0,1,2}
* assets/Earth.png
* assets/bg.png
* assets/station.png

Other than the above no rights have been reserved, but in the unlikely event that you use anything, please drop me a [mail][mailaddr].

###Contact me
Email : amhndu@gmail.com


[orbital debris]: http://www.allworkallplay.org/games/orbitaldebris/
[repo]: https://github.com/amhndu/gravity
[mailaddr]: mailto:amhndu@gmail.com
