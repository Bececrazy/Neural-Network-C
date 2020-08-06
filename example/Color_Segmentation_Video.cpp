#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"

using namespace std;

char pos[100] = "Dataset/color/pos";
char neg[100] = "Dataset/color/neg";
char path[100];

tensor* img = new tensor[3];
tensor* gen = new tensor[3];
tensor label;
Mat frame;

int main(){

	mlp nn(2,0.003,sgdw,mean_squared);
	nn.add_layer(3,null,input_layer);
	nn.add_layer(4,sigmoid,output_layer);
	nn.compile();
	
	for(int m=50; m--;){

		for(int n=10; n--;){ for(int k=4; k--;){
			sprintf( path,"%s/%d.jpg",pos,k );
			img = img2tensor_color( path );
			for( int i=img[0].size[2]; i--; ){
				nn.input.data[0] = img[0].data[i];
				nn.input.data[1] = img[1].data[i];
				nn.input.data[2] = img[2].data[i];
				nn.output.set_zero();
				nn.output.data[k]=1;
				nn.forwrd(); nn.backwrd();
		}}}
		
		for(int n=10; n--;){ for(int k=4; k--;){
			sprintf( path,"%s/%d.jpg",neg,k );
			img = img2tensor_color( path );
			for( int i=img[0].size[2]; i--; ){
				nn.input.data[0] = img[0].data[i];
				nn.input.data[1] = img[1].data[i];
				nn.input.data[2] = img[2].data[i];
				nn.output.set_value(0);
				nn.forwrd(); nn.backwrd();
		}}}
	}

	VideoCapture cap(0); 
	String window_name = "My First Video";
	namedWindow(window_name, WINDOW_NORMAL);
	if (cap.isOpened() == false){ printf("Cannot open the video file \n"); exit(1);}

	int s=50;
	gen[0].relloc( s,s );
	gen[1].relloc( s,s );
	gen[2].relloc( s,s );

	while(1){
		bool bSuccess = cap.read(frame);
		if (waitKey(10) == 27){ printf("Esc key is pressed by user. Stoppig the video\n"); exit(1);}

		img = img2tensor_color(frame);
		for(int i=3; i--;){ img[i] = decrease_image(img[i],s); }

		for( int i=img[0].size[2]; i--; ){
				nn.input.data[0] = img[0].data[i];
				nn.input.data[1] = img[1].data[i];
				nn.input.data[2] = img[2].data[i]; nn.forwrd();
				gen[0].data[i] = (nn.get_output().data[2]);
				gen[1].data[i] = (nn.get_output().data[1]);
				gen[2].data[i] = (nn.get_output().data[0]);
		}

		for(int i=3; i--;){ gen[i] = thresholding(gen[i],0.15); }

		imshow(window_name, tensor2img_color(gen)); }

return 0;}