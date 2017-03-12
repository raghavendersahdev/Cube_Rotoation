# Cube_Rotoation

This repository contains simple cube rotations and translations using the openGL and the MRPT api, It also contains a pioneer robot which can be navigated on the 2D ground plane using the the arrow keys <br/>

To run the code follow the instructions:

```
  $ cd PROJECT_DIR
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make 
```

* for translations use the following keys:  <br/>
a,d = move the cube along along +/- x axis  <br/>
z,x = move the cube along along +/- y axis  <br/>
w,s = move the cube along along +/- z axis  <br/>

* for rotations, use the following keys:

8, 2 : yaw (increase, decrease)  <br/>
4, 6 : pitch (increase, decrease) <br/>
1, 0 : roll (increase, decrease)   <br/>

* for moving the robot (Pioneer 3AT), use the following keys:

up : move robot forward    <br/>
down : move robot backward <br/>
left : turn robot leftwards <br/>
right : turn robot rightwards <br/>
