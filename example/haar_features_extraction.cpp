#include "stdio.h"
#include "stdlib.h"

#include "IA/tensor.h"
#include "IA/image.h"
#include "IA/tensor_extention.h"
		 
tensor* img = new tensor[3];

int main( int argc, char* argv[] ){
	img[0] = decrease_image(img2tensor("dataset/vr/3.jpg"),128);
	img[2] = integral_image(img[0]); 
	img[1].relloc(128,128);

	int sx=10,sy=10;

	//Tensor Vertical
	/*for(int i=img[0].size[1]-(sy*2); i--;){ for(int j=img[0].size[0]-sx; j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_edge_ver( img[2],i,j,sy,sx );
	}}*/

	//Tensor Horizontal
	/*for(int i=img[0].size[1]-sy; i--;){ for(int j=img[0].size[0]-(sx*2); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_edge_hor( img[2],i,j,sy,sx );
	}}*/

	//Tensor Vertical
	/*for(int i=img[0].size[1]-(sy*3); i--;){ for(int j=img[0].size[0]-sx; j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_line_ver( img[2],i,j,sy,sx );
	}}*/

	//Tensor Horizontal
	/*for(int i=img[0].size[1]-sy; i--;){ for(int j=img[0].size[0]-(sx*3); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_line_hor( img[2],i,j,sy,sx );
	}}*/

	/*for(int i=img[0].size[1]-(sy*2); i--;){ for(int j=img[0].size[0]-(sx*2); j--;){
		img[1].data[img[1].get_index(i,j)] = haar_filter_outline( img[0],i,j,sy,sx );
	}}*/

	img[1] = activation(img[1],sigmoid,true);
	show_image(img[1]);
 return 0;}