#include "stdio.h"
#include "stdlib.h"
#include "../Libraries/PC/tensor.h"

//Definicion de tensores
tensor A,B,C,D,E,F;

//Predefinir valores de B
float B_ARRAY[9] = {
	5,10,20,
	2, 4, 6,
	3, 6, 9,
}; 
//Siempre los arreglos en float

int main(){

	//Inicializacion de los tensores
	A.relloc(3,3); //Establecer tamaño de la matriz a 3x3
	B.relloc(3,3); //Establecer tamaño del vector a 1x3
	
	//Inicializar Valores de los tensores
	B.set_array(B_ARRAY);
	A.set_one();
	
	//Mostramos los tensores
	puts("Tensor A:"); A.show_tensor();
	puts("Tensor B:"); B.show_tensor();

	//Suma de tensores
	puts("Suma de tensores");
	C = A + B; C.show_tensor();
	
	//Resta de tensores
	puts("Resta de tensores");
	D = A - B; D.show_tensor();
	
	//Multiplicacion de tensores
	puts("Multiplicacion tensores");
	E = A * B; E.show_tensor();
	
	//Operaciones con Valores Escalares
	puts("Multiplicacion con Valor Escalar");
	F = B * 10; F.show_tensor();	
	
	//Eliminar contenido del Tensor
	B.delloc(); A.delloc();
	
	//Esto ultimo no es necesario hacer cada vez, el algoritmo tensor.h
	//lo hace automaticamente para optimizar el uso de la memoria.
	
	//Recomiendo leer el codigo tensor.h para conocer el uso de los registros.
	
	return 0;}