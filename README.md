Gravity
=======
![Gravity](assets/gravity.png)

**An Orbital survival game**

A game I made for my school project.
The objective is to survive as long as you can in your space station avoiding other satellites and junk.
This is a clone of the HTML5 game [Orbital Debris][orbital debris].

The premise :
You are inside a space station with unlimited fuel while the space agencies at Earth have gone crazy , they have made a large number of satellites and are launching satellites at any time any place while you have to dodge them. :)

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
The following images have been *scraped* from [here][orbital debris]
* assets/satellite\<n>.png where n = {0,1,2,3}
* assets/junk\<n>.png where n = {0,1,2}
* assets/Earth.png
* assets/bg.png
* assets/station.png

Other than the above no rights have been reserved , but in the unlikely event that you use anything ,you could drop me a [mail][mailaddr] and link to the [GitHub repository][repo].

[orbital debris]: http://www.allworkallplay.org/games/orbitaldebris/
[repo]: https://github.com/amhndu/gravity
[mailaddr]: mailto:amhndu@gmail.com
