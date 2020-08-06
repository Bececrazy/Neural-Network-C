#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"

using namespace std;

Mat frame;
tensor* img = new tensor[4];
	
void* Img_Detection( void* arg ){
	img[0] = decrease_image(img2tensor(frame),100);
	pthread_exit(NULL);}

int main(){

	pthread_t threads[3];
	void *status;
	
	VideoCapture cap("/home/enmanuel/Vídeos/Proyectos/MusicOsciloscope.mp4");
	char window_name[10];
	sprintf(window_name,"(%d,%d)",100,100);
	namedWindow(window_name, WINDOW_NORMAL);
	if (cap.isOpened() == false){ printf("Cannot open the video file \n"); exit(1);}

	while (true){
		bool bSuccess = cap.read(frame);
	  	if (bSuccess == false){ printf("Found the end of the video\n"); exit(1);}
		if (waitKey(10) == 27){ printf("Esc key is pressed by user. Stoppig the video\n"); exit(1);}

		if(pthread_create(&threads[0], NULL, Img_Detection, NULL) != 0){ puts("error"); exit(1); } pthread_join(threads[0], &status);
		imshow(window_name, tensor2img(img[0]));

	} pthread_exit(NULL);
return 0;}