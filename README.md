# embree-raytracer
 A simple offline path tracer written in C++ with intel embree library.
## How to Build & Run
```shell
git clone https://github.com/Makalou/embree-raytracer.git
```
or 
```shell
git clone git@github.com:Makalou/embree-raytracer.git
```
This project is managed by CMake so please make sure that CMake has been installed before following proccess.

At the root path of project:
```shell
mkdir build
```
```shell
cd build
```
```shell
cmake ..
```
Then CMake should automatically check dependencies and generate platform-specific project manage file : (e.g. .sln on Windows). You can then open the project within your favorite IDE then build and run.

## Showcases

![plot](./img/IMG_0258.jpeg)

![plot](./img/IMG_0259.jpeg)

![plot](./img/random_scene_light.PNG)

![alt-text](./img/room_with_color.gif)
