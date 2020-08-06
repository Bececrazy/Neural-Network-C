//#define MLP	

#include "stdio.h"
#include "stdlib.h"
#include "../Libraries/PC/tensor.h"

float dataset[30][2][6]{
	{{51, 30, 39, 61, 92, 45}, {43}},
	{{64, 51, 54, 63, 73, 47}, {63}},
	{{70, 68, 69, 76, 86, 48}, {71}},
	{{63, 45, 47, 54, 84, 35}, {61}},
	{{78, 56, 66, 71, 83, 47}, {81}},
	{{55, 49, 44, 54, 49, 34}, {43}},
	{{67, 42, 56, 66, 68, 35}, {58}},
	{{75, 50, 55, 70, 66, 41}, {71}},
	{{82, 72, 67, 71, 83, 31}, {72}},
	{{61, 45, 47, 62, 80, 41}, {67}},
	{{53, 53, 58, 58, 67, 34}, {64}},
	{{60, 47, 39, 59, 74, 41}, {67}},
	{{62, 57, 42, 55, 63, 25}, {69}},
	{{83, 83, 45, 59, 77, 35}, {68}},
	{{77, 54, 72, 79, 77, 46}, {77}},
	{{90, 50, 72, 60, 54, 36}, {81}},
	{{85, 64, 69, 79, 79, 63}, {74}},
	{{60, 65, 75, 55, 80, 60}, {65}},
	{{70, 46, 57, 75, 85, 46}, {65}},
	{{58, 68, 54, 64, 78, 52}, {50}},
	{{40, 33, 34, 43, 64, 33}, {50}},
	{{61, 52, 62, 66, 80, 41}, {64}},
	{{66, 52, 50, 63, 80, 37}, {53}},
	{{37, 42, 58, 50, 57, 49}, {40}},
	{{54, 42, 48, 66, 75, 33}, {63}},
	{{77, 66, 63, 88, 76, 72}, {66}},
	{{75, 58, 74, 80, 78, 49}, {78}},
	{{57, 44, 45, 51, 83, 38}, {48}},
	{{85, 71, 71, 77, 74, 55}, {85}},
	{{82, 39, 59, 64, 78, 39}, {82}}
};

float test[4][6]{
	{60, 47, 39, 59, 74, 41}, //67
	{57, 44, 45, 51, 83, 38}, //48
	{55, 49, 44, 54, 49, 34}, //43
	{85, 64, 69, 79, 79, 63}, //85
};

int main(){
	rbfnn M(0.03,momentum,mean_squared);
	M.add_layer(6,softplus,input_layer);
	M.add_layer(6,softplus,hidden_layer);
	M.add_layer(1,linear,output_layer);
	M.compile();

	M.layer[0].Array[1].set_array(dataset[2][0]);
	
	for(int i=500; i--;){ for(int j=0; j<30 ;j++){ M.fit(dataset[j][0],dataset[j][1]);}}
	for(int j=0; j<4 ;j++){M.predict(test[j]);}
	return 0;}