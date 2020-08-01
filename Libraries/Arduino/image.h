#include <Wire.h>

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

# define Pxl 0x3A // First Pixel Value

///##########################################################################///
///								HARDWARE CONTROLLER 					 	 ///
///##########################################################################///

class OV7670{
  public:

  	struct STRUCT_GPIO{
  		uint8_t Href;
  		uint8_t Vref;
  		uint8_t Pclk;
  		uint8_t Xclk;
  		uint8_t Data[8];
  	}IO;

   	void I2C_WRITE( int ADDRESS, int DATA ){
      	Wire.beginTransmission( 0x21 );
      	Wire.write( ADDRESS );
      	Wire.write( DATA );
      	Wire.endTransmission();
      	delay(1); }

   	uint8_t I2C_READ( int ADDRESS ){
      	Wire.beginTransmission( 0x21 );
      	Wire.write( ADDRESS );
      	Wire.endTransmission();
      	Wire.requestFrom( 0x21,1 );
      	return Wire.read(); }

	void PWM_INIT(){
		ledcAttachPin( IO.Xclk,0 );
		ledcSetup( 0,8000000,1 ); //8 o 20
		ledcWrite( 0,1 ); }

	void set_IO_def(){  
		IO.Data[1] = 33;
		IO.Data[3] = 32;
		IO.Data[5] = 35;
		IO.Data[7] = 34;
		  
		IO.Data[0] = 4;
		IO.Data[2] = 16;
		IO.Data[4] = 17;
		IO.Data[6] = 5;

		IO.Href = 19;
		IO.Vref = 36;
		IO.Pclk = 39;
		IO.Xclk = 18; }

    void init(){ Wire.begin();

      	//initialization of pins
		set_IO_def();
      	pinMode( IO.Pclk,INPUT ); pinMode( IO.Xclk,OUTPUT );
      	pinMode( IO.Href,INPUT ); pinMode( IO.Vref,INPUT );
      	for(int D=8; D--;){ pinMode(IO.Data[D],INPUT); } 

    	PWM_INIT();

      	//Soft Reset
		I2C_WRITE( 0x12, 0x80 ); delay( 500 );
		I2C_WRITE( 0x09, 0x11 );

		// Clock and Frame Conf
		I2C_WRITE( 0x11, 0x01 ); 
		I2C_WRITE( 0x15, 0x08 ); 
		I2C_WRITE( 0x1E, 0x30 );
		
		// Color Format Conf
		I2C_WRITE( 0x12, 0x04 );
		I2C_WRITE( 0x40, 0xF0 );
		I2C_WRITE( 0x3A, 0x04 );

		// Magic Registers
		I2C_WRITE( 0x0E, 0x61 );
		I2C_WRITE( 0x0F, 0x4B );
		I2C_WRITE( 0x3C, 0x78 );
		I2C_WRITE( 0x16, 0x02 );
		I2C_WRITE( 0x21, 0x02 );
		I2C_WRITE( 0x22, 0x91 );
		I2C_WRITE( 0x29, 0x07 );
		I2C_WRITE( 0x33, 0x0B );
		I2C_WRITE( 0x35, 0x0B );
		I2C_WRITE( 0x37, 0x1D );
		I2C_WRITE( 0x38, 0x71 );
		I2C_WRITE( 0x39, 0x2A );
		I2C_WRITE( 0x4D, 0x40 );
		I2C_WRITE( 0x4E, 0x20 ); 
		I2C_WRITE( 0x74, 0x10 );
		I2C_WRITE( 0x8D, 0x4F );
		I2C_WRITE( 0x8E, 0x00 );
		I2C_WRITE( 0x8F, 0x00 );
		I2C_WRITE( 0x90, 0x00 );
		I2C_WRITE( 0x91, 0x00 );
		I2C_WRITE( 0x96, 0x00 );
		I2C_WRITE( 0x9A, 0x00 );
		I2C_WRITE( 0xB0, 0x84 );
		I2C_WRITE( 0xB1, 0x0C );
		I2C_WRITE( 0xB2, 0x0E );
		I2C_WRITE( 0xB3, 0x82 );
		I2C_WRITE( 0xB8, 0x0A );

		// Image Size Conf
		I2C_WRITE( 0x0C, 0x04 );
		I2C_WRITE( 0x3E, 0x1B );
		I2C_WRITE( 0x6b, 0x3A );
		I2C_WRITE( 0x70, 0x3A );
		I2C_WRITE( 0x71, 0x35 );
		I2C_WRITE( 0x72, 0x11 );
		I2C_WRITE( 0x73, 0xF9 );
		I2C_WRITE( 0xA2, 0x02 ); delay( 500 );

      	if( I2C_READ(0x0A) == 0x76 ){ 
			I2C_WRITE( 0x09, 0x01 ); //Soft Awake Model
      		Serial.print("Camera detected\n"); }
      		Serial.println("intializated cam\n"); 
    }

    void takeNsend(){ I_reg[0].relloc(80,120);

		// Vertical Sincronization
		while( !((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		while(  ((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		int i=I_reg[0].size[1], j=I_reg[0].size[0], k=8;
		Serial.printf("%c\n",YJmp);

		// Get RGB Image from Camera
		while( i-- ){ while( j-- ){ uint16_t buff = 0x00; 
				
			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			Serial.printf( "%c", (char)( (buff>>5)&0x1F ) + Pxl ); }
		
		while( !((GPIO.in >> IO.Href) & 0x1) ); 
		while(  ((GPIO.in >> IO.Href) & 0x1) );
		Serial.printf("%c\n",XJmp);
		j=I_reg[0].size[0];}
	}

	tensor img2tensor(){
		I_reg[0].relloc(80,120);

		// Vertical Sincronization
		while( !((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		while(  ((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		int i=I_reg[0].size[1], j=I_reg[0].size[0], k=8;

		// Get RGB Image from Camera
		while( i-- ){ while( j-- ){ uint16_t buff = 0x00; 
				
			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			I_reg[0].data[I_reg[0].get_index(i,j)]  = (float)((buff>>7)&0xF8); //R
			I_reg[0].data[I_reg[0].get_index(i,j)] += (float)((buff>>2)&0xF8); //G
			I_reg[0].data[I_reg[0].get_index(i,j)] += (float)((buff<<3)&0xF8); //B
			
			}
		
		while( !((GPIO.in >> IO.Href) & 0x1) ); 
		while(  ((GPIO.in >> IO.Href) & 0x1) );
		j=I_reg[0].size[0]; }
		
	return I_reg[0]/(3*255); }

	tensor* img2tensor_color(){

		for(int i=3;i--;){I_img[i].relloc(80,120);}

		// Vertical Sincronization
		while( !((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		while(  ((GPIO.in1.val >> (IO.Vref - 32)) & 0x1) );
		int i=I_img[0].size[1], j=I_img[0].size[0], k=8;

		// Get RGB Image from Camera
		while( i-- ){ while( j-- ){ uint16_t buff = 0x00; 
				
			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			while( !((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );
			k=8;while( k-- ){
				buff = (buff<<1) | ((GPIO.in1.val >> (IO.Data[k] - 32)) & 0x1);
				buff = (buff<<1) | ((GPIO.in >> IO.Data[k-1]) & 0x1); k--;}
			while(  ((GPIO.in1.val >> (IO.Pclk - 32)) & 0x1) );

			I_img[2].data[I_img[2].get_index(i,j)] = (float)((buff>>7)&0xF8); //R
			I_img[1].data[I_img[1].get_index(i,j)] = (float)((buff>>2)&0xF8); //G
			I_img[0].data[I_img[0].get_index(i,j)] = (float)((buff<<3)&0xF8); //B
			
			}
		
		while( !((GPIO.in >> IO.Href) & 0x1) ); 
		while(  ((GPIO.in >> IO.Href) & 0x1) );
		j=I_img[0].size[0]; }

		for(int i=3;i--;){I_img[i] /= 255;}
		
	return I_img; }

} camera;

///##########################################################################///
///								IMAGE FUNCTIONS							 	 ///
///##########################################################################///

void show_image( tensor* img ){ Serial.printf("%c\n",YJmp);
	for( int i=0; i<img[0].size[1]; i++ ){ for( int j=0; j<img[0].size[0]; j++ ){
		for(uint8_t k=3; k--;){
			Serial.printf( "%c%c", 0x30|k, (char)(img[k].data[img[k].get_index(i,j)]*31) + Pxl );
		}}	Serial.printf("%c\n",XJmp); }
}

void show_image( tensor img ){ Serial.printf("%c\n",YJmp);
	for( int i=0; i<img.size[1]; i++ ){ for( int j=0; j<img.size[0]; j++ ){
		Serial.printf( "%c", (char)(img.data[img.get_index(i,j)]*31) + Pxl );
	}	Serial.printf("%c\n",XJmp); }
}

///##########################################################################///
///							COMPRESSED IMAGE FUNCTIONS					 	 ///
///##########################################################################///

void show_image_compressed( tensor img ){
	Serial.printf("%c\n",YJmp);
	char pix[2]={'\0','\0'};
	int8_t rep=Pxl;

	for( int i=img.size[2]; i--; ){
		
		pix[0] = (char)(img.data[i]*31) + Pxl;

		if(pix[1] == pix[0]){ rep++; }
		else if(pix[1] == '\0'){ pix[1]=pix[0]; rep=Pxl; }
		else{ 
			Serial.printf( "%c%c%c", (char)rep,pix[1],XJmp );
			pix[1]=pix[0]; rep=Pxl; }
	}
}