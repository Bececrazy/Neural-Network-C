#include <stdlib.h>
#include <stdio.h>

#include "IA/tensor.h"
#include "IA/image.h"

using namespace std;

char pos[100] = "/home/enmanuel/Documentos/develop/cpp/ML/dataset/vr/pos";
char neg[100] = "/home/enmanuel/Documentos/develop/cpp/ML/dataset/vr/neg";
char tst[100] = "/home/enmanuel/Documentos/develop/cpp/ML/dataset/vr";

int main(){

	tensor* image;
	char path[100];
	tensor i_gen(128,128);

	mlp nn(2,0.003,sgdw,mean_squared);
	nn.add_layer(3,sigmoid,input_layer);
	nn.add_layer(1,sigmoid,output_layer);
	nn.compile();

	for(int m=5; m--;){
		for(int i=5; i--;){ for(int k=5; k--;){
			sprintf(path,"%s/%d.jpg",pos,k);
			image = img2tensor_color(path); for( int l=3; l--;){ image[l] = decrease_image(image[l],64); }
			for( int l=image[0].size[2]; l--;){
				nn.output.set_value(1);	
				nn.input.data[0] = image[0].data[l];
				nn.input.data[1] = image[1].data[l];
				nn.input.data[2] = image[2].data[l];
				nn.forwrd(); nn.backwrd();
			}}}

		for(int i=5; i--;){ for(int k=5; k--;){
			sprintf(path,"%s/%d.jpg",neg,k);
			image = img2tensor_color(path); for( int l=3; l--;){ image[l] = decrease_image(image[l],64); }
			for( int l=image[0].size[2]; l--;){
				nn.output.set_value(0);	
				nn.input.data[0] = image[0].data[l];
				nn.input.data[1] = image[1].data[l];
				nn.input.data[2] = image[2].data[l];
				nn.forwrd(); nn.backwrd();
			}}}
	}
	
	for(int k=12; k--;){
		puts("predictions: \n");
		sprintf(path,"%s/%d.jpg",tst,k);
		image = img2tensor_color(path); for( int l=3; l--;){ image[l] = decrease_image(image[l],128); }
		for( int l=image[0].size[2]; l--;){
			nn.input.data[0] = image[0].data[l];
			nn.input.data[1] = image[1].data[l];
			nn.input.data[2] = image[2].data[l];
			nn.forwrd(); i_gen.data[l] = nn.get_output().data[0];}
		show_image(image);
		show_image( thresholding(i_gen,0.05) );
	}
return 0;}