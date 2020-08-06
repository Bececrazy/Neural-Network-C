# Neural-Network-C
This is a simple c++ implementation of different image processing and IA algorithms that can be compiled on a microcontroller such as Arduino uno or an Esp32.

these algorithms contains 2 versions of the same code, one is a desktop version in which you can train a model using PC, and the other consist in a microcontroller version to import the pre-trained model into them.

to Compile a CV example, you should have to write this:
g++ -std=c++11 -I /usr/include/opencv -I/usr/include/opencv2 -L/usr/lib/ "EXAMPLE/PATH.CPP" -o main -lopencv_core -lopencv_highgui -lopencv_video -lX11 -pthread

if you want to compile a simple IA algorith, you should have to write this:
g++ -std=c++11 "EXAMPLE/PATH.CPP" -o main
