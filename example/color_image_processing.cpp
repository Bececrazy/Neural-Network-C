
#include "stdio.h"
#include "stdlib.h"

#include "../Libraries/PC/tensor.h"
#include "../Libraries/PC/image.h"

using namespace std;

int main(){

	tensor* A; A = img2tensor_color("Dataset/vr/4.jpg");

	A[0] = decrease_image(A[0],100);
	A[1] = decrease_image(A[1],100);
	A[2] = decrease_image(A[2],100);
	
	A[0] = convolution( A[0],get_filter(sobel_edge) );
	A[1] = convolution( A[1],get_filter(sobel_edge) );
	A[2] = convolution( A[2],get_filter(sobel_edge) );
	
	A[0] = activation(A[0],relu,true);
	A[1] = activation(A[1],relu,true);
	A[2] = activation(A[2],relu,true);

	show_image(A);

return 0;}