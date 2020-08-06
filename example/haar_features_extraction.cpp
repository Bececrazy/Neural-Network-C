#include "stdio.h"
#include "stdlib.h"

#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"
		 
tensor* img = new tensor[3];

int main( int argc, char* argv[] ){
	img[0] = decrease_image(img2tensor("Dataset/vr/3.jpg"),100);
	img[2] = integral_image(img[0]); 
	img[1].relloc(100,100);

	int sx=1,sy=1;

	//Vertical Edge Haar Detector 1x2
	/*for(int i=img[0].size[1]-(sy*2); i--;){ for(int j=img[0].size[0]-sx; j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_edge_ver( img[2],i,j,sy,sx );
	}}*/

	//Horizontal Edge Haar Detector 2x1
	/*for(int i=img[0].size[1]-sy; i--;){ for(int j=img[0].size[0]-(sx*2); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_edge_hor( img[2],i,j,sy,sx );
	}}*/

	//Vertical Line Haar Detector 1x3
	for(int i=img[0].size[1]-(sy*3); i--;){ for(int j=img[0].size[0]-sx; j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_line_ver( img[2],i,j,sy,sx );
	}}

	//Horizontal Line Haar Detector 3x1
	/*for(int i=img[0].size[1]-sy; i--;){ for(int j=img[0].size[0]-(sx*3); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_line_hor( img[2],i,j,sy,sx );
	}}*/

	//Outline Haar Detector 2x2
	/*for(int i=img[0].size[1]-(sy*2); i--;){ for(int j=img[0].size[0]-(sx*2); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_outline( img[0],i,j,sy,sx );
	}}*/

	img[1] = activation(img[1],sigmoid,true);
	show_image(img[1]);
 return 0;}