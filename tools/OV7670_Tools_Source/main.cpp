#include "stdlib.h"
#include "stdio.h"
#include "thread"

#include "miniwin.h"
#include "serial.h"
#include "tensor.h"

using namespace miniwin;
using namespace std;

//# define 0x39 //
//# define 0x38 //

//# define 0x37 //
//# define 0x36 //
//# define 0x35 //

# define YJmp 0x34 // Y Jump
# define XJmp 0x33 // X jump

# define RCh 0x32 // R Channel
# define GCh 0x31 // G Channel
# define BCh 0x30 // B Channel

tensor image(400,400);
int X,Y;
int C[3];

void init_conection(){		
	Serial.begin( 115200 );		
	printf("waiting device\n");
		
	while( !Serial.Connect() );
	printf("device connected\n");
}

void close_conection(){ Serial.Close(); }
	
void get_image(){
		
	while( Serial.Read()!=YJmp);
		
	Y=400;X=400; while(X>=0 and Y>=0){
						
		uint8_t P=Serial.Read();

		if( P==YJmp ){break;}	
		else if( P==10 or P==13 ){ continue; }
		else if( P==XJmp ){X=400; Y--; continue;}

		else if( P==RCh ){ UP1:
			P=Serial.Read(); if( P==10 or P==13 ){ goto UP1; }
			C[0] = (P-0x3A)*255/31; continue;}

		else if( P==GCh ){ UP2:
			P=Serial.Read(); if( P==10 or P==13 ){ goto UP2; }
			C[1] = (P-0x3A)*255/31; continue;}

		else if( P==BCh ){ UP3:
			P=Serial.Read(); if( P==10 or P==13 ){ goto UP3; }
			C[2] = (P-0x3A)*255/31; goto END;}

		else{ C[0] = (P-0x3A)*255/31;
			  C[1] = (P-0x3A)*255/31;
			  C[2] = (P-0x3A)*255/31; }

		END:
		color_rgb(C[0],C[1],C[2]);
		punto(Y,X);
	X--;}
}

int main(){
	vredimensiona(400,400); init_conection();
	while(1){ get_image(); refresca(); } close_conection();

return 0;}