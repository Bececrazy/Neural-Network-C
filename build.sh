rm -R main
clear clear

echo "...compiling..."
g++ -std=c++11 -I /usr/include/opencv -I/usr/include/opencv2 -L/usr/lib/ "example/haar_features_extraction.cpp" -o example1 -lopencv_core -lopencv_highgui -lopencv_video -lX11 -pthread

echo "...compiling..."
g++ -std=c++11 -I /usr/include/opencv -I/usr/include/opencv2 -L/usr/lib/ "example/color_image_processing.cpp" -o example2 -lopencv_core -lopencv_highgui -lopencv_video -lX11 -pthread

echo "...compiling..."
g++ -std=c++11 "example/clasification(MLP).cpp" -o example3

echo "Done"
clear clear
