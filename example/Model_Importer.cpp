#include "stdio.h"
#include "stdlib.h"
#include "../Libraries/PC/tensor.h"

float test[12][4]{
	{5.3,3.7,1.5,0.2},//{1,0,0}},
	{4.6,3.2,1.4,0.2},//{1,0,0}},
	{5.0,3.3,1.4,0.2},//{1,0,0}},
	{5.1,3.8,1.6,0.2},//{1,0,0}},
	
	{6.2,2.9,4.3,1.3},//{0,1,0}},
	{5.1,2.5,3.0,1.1},//{0,1,0}},
	{5.7,2.8,4.1,1.3},//{0,1,0}},
	{5.7,2.9,4.2,1.3},//{0,1,0}},
	
	{6.5,3.0,5.2,2.0},//{0,0,1}},
	{5.9,3.0,5.1,1.8},//{0,0,1}},
	{6.2,3.4,5.4,2.3},//{0,0,1}},
	{6.3,2.5,5.0,1.9} //{0,0,1}},
};

float w[35] = {9.6,-2.3,8.6,5.2,-1.4,-7.4,-0.5,-8.6,17.9,-0.6,-5.9,5.7,-9.1,-13.3,5.4,8.6,11.2,-2.7,-5.7,1.0,0.5,1.0,1.1,18.7,21.4,-13.5,-14.7,2.4,4.1,-2.0,-0.7,0.5,0.6,1.0,1.0};
float b[8]  = {-13.1,-7.4,-2.9,-0.7,-2.4,-4.5,-1.5,-2.4};
mlp nn;

int main(){
	nn.add_layer(4,sigmoid,input_layer);
	nn.add_layer(5,sigmoid,hidden_layer);
	nn.add_layer(3,softmax,output_layer);
	nn.compile(); model_import(w,b,nn);

	for(int j=0; j<12 ;j++){nn.predict(test[j]);}
	return 0;}