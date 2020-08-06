#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"

using namespace std;

tensor img[3];
tensor label;
Mat frame;

int main(){

	VideoCapture cap(0); 
	String window_name = "My First Video";
	namedWindow(window_name, WINDOW_NORMAL);
	
	if (cap.isOpened() == false){ printf("Cannot open the video file \n"); exit(1);}

	while(1){
		bool bSuccess = cap.read(frame);
		if (waitKey(10) == 27){ printf("Esc key is pressed by user. Stoppig the video\n"); exit(1);}

		img[0] = decrease_image(img2tensor(frame),200);
		img[1] = decrease_image(img[0],50);
		img[2] = img2tensor("Dataset/base.jpg");
		label = template_matching(img[1],img[2],0.89);

		imshow(window_name, tensor2img(img[0],label)); }

return 0;}