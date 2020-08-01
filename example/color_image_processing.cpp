
#include "stdio.h"
#include "stdlib.h"

#include "IA/tensor.h"
#include "IA/image.h"

using namespace std;

int main(){

	tensor* A; A = img2tensor_color("/home/enmanuel/Imágenes/pc/wallpaper/forest-1920x1080-fog-trees-mist-green-4k-11416.jpg");

	A[0] = decrease_image(A[0],128);
	A[1] = decrease_image(A[1],128);
	A[2] = decrease_image(A[2],128);

	show_image(A);

return 0;}