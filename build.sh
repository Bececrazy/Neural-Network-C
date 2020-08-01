rm -R main
clear clear

echo "...compiling..."
g++ -std=c++11 -I /usr/include/opencv -I/usr/include/opencv2 -L/usr/lib/ main.cpp -o main -lopencv_core -lopencv_highgui -lopencv_video -lX11 -pthread

echo "Done"
clear clear

./main
