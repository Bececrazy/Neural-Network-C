#include "stdio.h"
#include "stdlib.h"

#include "IA/tensor.h"
#include "IA/image.h"
		 
tensor img[3];
tensor label;

float array[9] = {2,2,2,2,2,2,2,2,2};

int main( int argc, char* argv[] ){
	img[0]= decrease_image(img2tensor("dataset/vr/2.jpg"),100);
	img[1]= img2tensor("dataset/vr/base.jpg");

	label = template_matching(img[0],img[1],0.70);
	show_image(img[0],label);

 return 0;}