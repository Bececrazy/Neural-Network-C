#include "stdio.h"
#include "stdlib.h"

#include "dirent.h"
#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"

using namespace std;

char trainingPath[10][100] = {
	"Dataset/mnist/trainingSet/0",
	"Dataset/mnist/trainingSet/1",
	"Dataset/mnist/trainingSet/2",
	"Dataset/mnist/trainingSet/3",
	"Dataset/mnist/trainingSet/4",
	"Dataset/mnist/trainingSet/5",
	"Dataset/mnist/trainingSet/6",
	"Dataset/mnist/trainingSet/7",
	"Dataset/mnist/trainingSet/8",
	"Dataset/mnist/trainingSet/9",
};

float TrainingOutput[10][10] = {
	{1,0,0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0},
	{0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,1},
};

char testSet[100] = "Dataset/mnist/testSet";

int main(){

	tensor image;
	char path[100];
	int n_data = 1000;
	struct dirent *entry;

	mlp nn(3,0.003,momentum,mean_squared);
	nn.add_layer(784,null	,input_layer);
	nn.add_layer(255,sigmoid,input_layer);
	nn.add_layer(10	,softmax,output_layer);
	nn.compile();

	for(int i=10; i--;){
		for(int j=10; j--;){
			DIR *dir = opendir(trainingPath[j]);

			for(int k=n_data; k--;){ entry=readdir(dir);
				if(n_data-k<3){continue;}

				sprintf(path,"%s/%s",trainingPath[j],entry->d_name);
				image = img2tensor(path);
				nn.fit(image.data, TrainingOutput[j]);
				nn.get_output().show_tensor();

		} closedir(dir); }}
	
	system("clear");
	puts("predictions: \n");

	DIR *dir = opendir(testSet);

	for(int k=30; k--;){ entry=readdir(dir);
		if(30-k<3){continue;}

		sprintf(path,"%s/%s",testSet,entry->d_name);
		image = img2tensor(path); show_image(image);
		nn.predict(image.data);
	}
	closedir(dir);
	return 0;
}