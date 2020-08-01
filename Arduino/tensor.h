///##########################################################################///
///									Constants								 ///
///##########################################################################///

// Optimization ##############################################################//
#define sgd					0
#define momentum			1

// Activation function kernel ################################################//
#define null 				0
#define relu				1
#define tangh				2
#define atang				3
#define linear				4
#define sigmoid				5
#define softmax				6
#define softplus			7
	
// Layer Const ###############################################################//
#define input_layer			0
#define hidden_layer		1
#define output_layer		2

// Cost function kernel ######################################################//
#define mean_squared 		0
#define cross_entropy		1

// Filter Types ##############################################################//
#define gauss				0
#define sobel_edge			1
#define prewitt_edge		2
#define sobel_vertical		3
#define prewitt_vertical	4
#define roberts_vertical	5
#define sobel_horizontal	6
#define prewitt_horizontal	7
#define roberts_horizontal	8

///##########################################################################///
///								Lib Dependences								 ///
///##########################################################################///

#include "math.h"

///##########################################################################///
///									Tensor									 ///
///##########################################################################///

//TODO: Tensor class #########################################################//
class tensor{
	public:		
		// Tensor variables ##################################################//
		float* data; 
		int size[3];

		tensor(int x=0, int y=0){
			size[2] = x*y;
			size[0] = x; size[1] = y;
			data = new float[size[2]];
			for(int i=size[2]; i--;){data[i] = 0;}
		}
			
		// Tensor definition & indexing function #############################//
		void delloc_reg();
		void show_tensor();
		void relloc(int x, int y);

		inline void delloc();
		inline void set_one();
		inline void set_zero();
		inline void set_order();
		inline tensor transpose();
		inline int get_index(int i, int j);
		inline void set_value(float value);
		inline void set_array(float* array);	
		inline void set_random(float x=1, float b=0);	

		void operator=(tensor A){
			if(this->data==NULL || this->size[2]!=A.size[2]){ 
				this->relloc(A.size[0],A.size[1]); }
				this->set_array(A.data);
				//delloc_reg();
		}

	};

	//* TODO: Tensor Register *//
		tensor T_reg[6];	//Normal operator Tensor register 
		tensor I_img[3];	//3 dimentional tensor Register
		tensor I_reg[6];	//Image Tensor Register
	//* TODO: Tensor Register *//

		//T_reg[0] = fot +- operations
		//T_reg[1] = for .* operations
		//T_reg[2] = for .> operations
		//T_reg[3] = for Const Functions
		//T_reg[4] = for transpose functions
		//T_reg[5] = for activations functions

		//I_reg[0] = for image extraction
		//I_reg[1] = for image scaling
		//I_reg[2] = for padding images
		//I_reg[3] = for image processing
		//I_reg[4] = for image labelling
		//I_reg[5] = fot template matching

	inline void tensor::delloc(){ this->relloc(0,0); }
	inline int  tensor::get_index(int i, int j){return j+i*size[0];}
	inline int get_index(int i, int j, int center){return center*i+j;}
	inline void tensor::set_one(){for(int i=size[2]; i--;){data[i] = 1;}}
	inline void tensor::set_zero(){for(int i=size[2]; i--;){data[i] = 0;}}
	inline void tensor::set_order(){for(int i=size[2]; i--;){data[i] = i;}}
	inline void tensor::set_value(float value){for(int i=size[2]; i--;){data[i] = value;}}
	inline void tensor::set_array(float* array){for(int i=size[2]; i--;){data[i] = array[i];}}
	inline void tensor::set_random(float x, float b){for(int i=size[2]; i--;){data[i] = ((rand() % 100) * 0.01)*x+b;}}
	
	void tensor::relloc(int x, int y){
		if(this->data != NULL){delete [] data;}
		this->size[2] = x*y;
		this->size[0] = x; this->size[1] = y;
		this->data = new float[this->size[2]];
		for(int i=this->size[2]; i--;){this->data[i] = 0;}}

	void tensor::show_tensor(){
		Serial.printf("(%d,%d):\n", size[0], size[1]);
		for(int i=0; i<size[1] ;i++){ for(int j=0; j<size[0] ;j++){Serial.printf("%.1f\t",data[get_index(i,j)]);}
		Serial.printf("\n");}Serial.printf("\n");}

	void tensor::delloc_reg(){ 
		for(int i=6; i--;){ T_reg[i].relloc(0,0); I_reg[i].relloc(0,0);}
	//	for(int i=3; i--;){ I_img[i].relloc(0,0); }
	}

	inline tensor tensor::transpose(){
		T_reg[4].relloc(size[1], size[0]);
		for(int i=0, m=0; i<size[0] ;i++){ for(int j=0; j<size[1] ;j++){
			T_reg[4].data[m] = this->data[get_index(j,i)];m++;}}
		return T_reg[4];}

///##########################################################################///
///									Math									 ///
///##########################################################################///
//TODO: Math Tensor ##########################################################//	

// Tensor math function //
inline bool near(float x, float b, float r=0.01){ if(x>=b-r and x<=b+r){return true;} else {return false;}}

inline float sum(tensor A){ 
	float res_sum = 0.0;
	for(int i=A.size[2]; i--;){ res_sum += A.data[i]; } 
	return res_sum;}

// Tensor math operator //
tensor operator*(tensor A, tensor B){
	 T_reg[1].relloc(B.size[0],A.size[1]);
		
	if(A.size[0] != B.size[1]){ Serial.printf("producto . erroneo (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
	
	for(int x2=0; x2<B.size[0]; x2++){ for(int y1=0; y1<A.size[1]; y1++){
		for(int x1=0; x1<A.size[0]; x1++){ for(int y2=0; y2<B.size[1]; y2++){
			if(x1 != y2){continue;}
			T_reg[1].data[T_reg[1].get_index(y1,x2)] += A.data[A.get_index(y1,x1)] * B.data[B.get_index(y2,x2)];
	}}}}
	
	return T_reg[1];}
			
tensor operator+(tensor A, tensor B){
	T_reg[0].relloc(B.size[0],B.size[1]);
	if(A.size[2]==0){ return B; }
	else{ if(A.size[0] != B.size[0] or A.size[1] != B.size[1]){Serial.printf("adicion erronea: (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
		  for(int i=A.size[2]; i--;){T_reg[0].data[i] = A.data[i] + B.data[i];}}
	return T_reg[0];}
			
tensor operator-(tensor A, tensor B){
	T_reg[0].relloc(B.size[0],B.size[1]);
	if(A.size[2]==0){ return B; }
	else{ if(A.size[0] != B.size[0] or A.size[1] != B.size[1]){Serial.printf("substraccion erronea: (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
		  for(int i=A.size[2]; i--;){T_reg[0].data[i] = A.data[i] - B.data[i];}}
	return T_reg[0];}
					
// Tensor math  ##############################################################//
void operator+=(tensor A, tensor B){
	if(A.size[0] != B.size[0] or A.size[1] != B.size[1]){Serial.printf("adicion erronea: (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
	for(int i=A.size[2]; i--;){A.data[i] += B.data[i];}}
		
void operator-=(tensor A, tensor B){
	if(A.size[0] != B.size[0] or A.size[1] != B.size[1]){ Serial.printf("sustraccion erronea: (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
	for(int i=A.size[2]; i--;){A.data[i] -= B.data[i];}}
			
// Tensor math with point to point ###########################################//
tensor operator>(tensor A, tensor B){
	T_reg[2].relloc(A.size[0],A.size[1]);
	if(A.size[0] != B.size[0] or A.size[1] != B.size[1]){ Serial.printf("producto X erronea: (%d,%d) (%d,%d)\n", A.size[0],A.size[1],B.size[0],B.size[1]); exit(1);}
	for(int i=A.size[2]; i--;){T_reg[2].data[i] = A.data[i] * B.data[i];}
	return T_reg[2];}	
			
// Tensor math with scalar ###########################################//
inline tensor operator*(tensor A, float B){ for(int i=A.size[2]; i--;){A.data[i] *= B;} return A;}
inline tensor operator/(tensor A, float B){ for(int i=A.size[2]; i--;){A.data[i] /= B;} return A;}
inline void operator*=(tensor A, float B){ for(int i=A.size[2]; i--;){A.data[i] *= B;}}
inline void operator/=(tensor A, float B){ for(int i=A.size[2]; i--;){A.data[i] /= B;}}

///##########################################################################///
///							1 dimention Image Prosessing 					 ///
///##########################################################################///
//TODO: Image Processing #####################################################//

	tensor* color_normalization(tensor* img){
		for(int j=img[0].size[2]; j--;){ float acum=0.0;
			for (int i=3; i--;){ acum += img[i].data[j]; }
			for (int i=3; i--;){ img[i].data[j] /= acum; }}
	return img;}
		
	tensor local_binary_pattern(tensor A){	
		I_reg[3].relloc(A.size[0]-2, A.size[1]-2);
		for(int i=I_reg[3].size[1]; i--;){for(int j=I_reg[3].size[0]; j--;){ 
			for(int k=3,index=0; k--;){for(int l=3; l--;){
				if(k==1 and l==1){continue;}
				if(A.data[A.get_index(i+k, j+l)] > A.data[A.get_index(i+1, j+1)]){
					I_reg[3].data[I_reg[3].get_index(i,j)] += pow(2,index);} 
					index++;
		}}}}
	return I_reg[3]/255;} 
	
	tensor difference_edge_detection(tensor A){
		I_reg[3].relloc(A.size[0]-3+1, A.size[1]-3+1);
		for(int i=I_reg[3].size[1]; i--;){for(int j=I_reg[3].size[0]; j--;){ 
			for(int k=2; k--;){for(int l=2; l--;){
				if(k==1 and l==1){continue;} float sum=0, res=0;
				if((sum=A.data[A.get_index(i+k, j+l)]-A.data[A.get_index(i-k+2, j-l+2)]) > res){res=sum;}
				I_reg[3].data[I_reg[3].get_index(i,j)] = res;
		}}}}
	return I_reg[3];}

	tensor thresholding(tensor A, float B){
		I_reg[3].relloc(A.size[0], A.size[1]);
		for(int i=I_reg[3].size[1]; i--;){for(int j=I_reg[3].size[0]; j--;){
			if(A.data[A.get_index(i,j)]>B){ I_reg[3].data[I_reg[3].get_index(i,j)]=1; }
			else{I_reg[3].data[I_reg[3].get_index(i,j)]=0;}
		}}
	return I_reg[3];}
	
	tensor decrease_image(tensor A,int new_size){
		if(A.size[2] < pow(new_size,2)){
			Serial.printf("error en resize (%d,%d) (%d,%d)",
			A.size[0],A.size[1], new_size,new_size);
			exit(1);}

		I_reg[1].relloc(new_size,new_size);
		int h_ratio=(A.size[1]/new_size), w_ratio=(A.size[0]/new_size);
		for(int i=new_size; i--;){ for(int j=new_size ;j--;){
			I_reg[1].data[I_reg[1].get_index(i,j)] = A.data[A.get_index(i*h_ratio,j*w_ratio)]; 
		}}

	return I_reg[1];}

// FIXME: Create a Tensor increase_image(){};

	tensor padding(tensor A, int p){
		I_reg[2].relloc(2*p+A.size[0], 2*p+A.size[1]);
		for(int i=p,n=0; n<A.size[1]; i++){for(int j=p,m=0; m<A.size[0]; j++){
			I_reg[2].data[I_reg[2].get_index(i,j)] = A.data[A.get_index(n,m)];
		m++;}n++;}
	return I_reg[2];}

	tensor maxpooling(tensor A){ //FIXME: to Optimize
		I_reg[3].relloc(A.size[0]/2,A.size[1]/2);
			
		for(int i=0,n=0; n<I_reg[3].size[1] ;n++){for(int j=0,m=0; m<I_reg[3].size[0] ;m++){ 
			for(int k=2; k--;){for(int l=2; l--;){
				if(I_reg[3].data[I_reg[3].get_index(n,m)] < A.data[A.get_index(i+k,j+l)])
					I_reg[3].data[I_reg[3].get_index(n,m)] = A.data[A.get_index(i+k,j+l)];
					
		}}j+=2;}i+=2;}
	return I_reg[3];}
	
	tensor dmaxpooling(tensor A, tensor B){ //FIXME: to Optimize
		I_reg[3].relloc(A.size[0],A.size[1]);
			
		for(int i=0,n=0; n<B.size[1] ;n++){for(int j=0,m=0; m<B.size[0] ;m++){	
			float acum=0; int indx=0;	

			for(int k=2; k--;){for(int l=2; l--;){
				if(acum < A.data[A.get_index(i+k,j+l)]){
					acum = A.data[A.get_index(i+k,j+l)];
					indx = I_reg[3].get_index(i+k,j+l);
				}}}
					
			I_reg[3].data[indx] = B.data[B.get_index(n,m)];

		j+=2;}i+=2;}				
	return I_reg[3];}
	
	tensor convolution(tensor A, tensor B){
		I_reg[3].relloc(A.size[0]-B.size[0]+1, A.size[1]-B.size[1]+1);
		if(A.size[2] < B.size[2]){Serial.printf("error en la convolution (%d,%d)*(%d,%d)", A.size[0], 	A.size[1], B.size[0],	B.size[1]); exit(1);}

		I_reg[4].relloc(B.size[0],B.size[1]); 
		for(int i=B.size[2],j=0; i--;){ I_reg[4].data[i] = B.data[j]; j++;}

		for(int i=I_reg[3].size[1]; i--;){for(int j=I_reg[3].size[0]; j--;){ 
			for(int k=B.size[1]; k--;){for(int l=B.size[0]; l--;){
				I_reg[3].data[I_reg[3].get_index(i,j)] += A.data[A.get_index(i+k,j+l)] * I_reg[4].data[B.get_index(k,l)];
		}}}}

	return I_reg[3];}

	tensor correlation(tensor A, tensor B){
		I_reg[3].relloc(A.size[0]-B.size[0]+1, A.size[1]-B.size[1]+1);
		if(A.size[2] < B.size[2]){Serial.printf("error en la correlation (%d,%d)*(%d,%d)", A.size[0], 	A.size[1], B.size[0],	B.size[1]); exit(1);}
		
		for(int i=I_reg[3].size[1]; i--;){for(int j=I_reg[3].size[0]; j--;){ 
			for(int k=B.size[1]; k--;){for(int l=B.size[0]; l--;){
				I_reg[3].data[I_reg[3].get_index(i,j)] += A.data[A.get_index(i+k,j+l)] * B.data[B.get_index(k,l)];
		}}}}

	return I_reg[3];}

	tensor labelling(tensor A){
		I_reg[1].relloc(A.size[0],A.size[1]);
		int label=1;
	
		for(int i=I_reg[1].size[1]-2; i--;){for(int j=I_reg[1].size[0]-2; j--;){
			if(A.data[A.get_index(i,j)] != 0){
				for(int k=3; k--;){ for(int l=3; l--;){
					if(I_reg[1].data[I_reg[1].get_index(i+k,j+l)] != 0){
						I_reg[1].data[I_reg[1].get_index(i+1,j+1)] = I_reg[1].data[I_reg[1].get_index(i+k,j+l)];
				}}}
				if(I_reg[1].data[I_reg[1].get_index(i+1,j+1)] == 0){ I_reg[1].data[I_reg[1].get_index(i+1,j+1)]=label; label++; }
		}}}
	
		I_reg[2].relloc(4,label-1);
		for(int i=I_reg[1].size[1], lb=0; i--;){for(int j=I_reg[1].size[0]; j--;){
			if((lb=I_reg[1].data[I_reg[1].get_index(i,j)]) != 0){
				if( I_reg[2].data[I_reg[2].get_index(lb-1,0)] == 0 || I_reg[2].data[I_reg[2].get_index(lb-1,0)] > j){ I_reg[2].data[I_reg[2].get_index(lb-1,0)] = j; }
				if(	I_reg[2].data[I_reg[2].get_index(lb-1,1)] == 0 || I_reg[2].data[I_reg[2].get_index(lb-1,1)] > i){ I_reg[2].data[I_reg[2].get_index(lb-1,1)] = i; }
				if( I_reg[2].data[I_reg[2].get_index(lb-1,2)] < j ){ I_reg[2].data[I_reg[2].get_index(lb-1,2)] = j;}
				if( I_reg[2].data[I_reg[2].get_index(lb-1,3)] < i ){ I_reg[2].data[I_reg[2].get_index(lb-1,3)] = i;}
		}}}

		for(int i=I_reg[2].size[1]; i--;){
			I_reg[2].data[I_reg[2].get_index(i,0)] /= I_reg[1].size[0];
			I_reg[2].data[I_reg[2].get_index(i,1)] /= I_reg[1].size[1];
			I_reg[2].data[I_reg[2].get_index(i,2)] /= I_reg[1].size[0];
			I_reg[2].data[I_reg[2].get_index(i,3)] /= I_reg[1].size[1];}

	return I_reg[2];}

	tensor get_filter(int type){
		int size=0, index=0;
		float filter[9][9] = {
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1}, //gauss		0
			{-1,-2,-1, 0, 0, 0, 1, 2, 1}, //sobel   hor 1
			{-1, 0, 1,-2, 0, 2,-1, 0, 1}, //sobel   ver	2
			{ 1, 0, 0,-1, 0, 0, 0, 0, 0}, //robert  hor 3
			{ 0, 1,-1, 0, 0, 0, 0, 0, 0}, //robert  ver	4
			{-1,-1,-1, 0, 0, 0, 1, 1, 1}, //prewitt hor 5
			{-1, 0, 1,-1, 0, 1,-1, 0, 1}, //prewitt ver 6
			{ 0,-1, 0,-1, 4,-1, 0,-1, 0}, //sobel   edg 7
			{-1,-1,-1,-1, 8,-1,-1,-1,-1}  //prewitt edg 8
		};

		switch(type){
			case gauss: size=3; index=0; break;
			case sobel_edge: size=3; index=7; break;
			case prewitt_edge: size=3; index=8; break;
			case sobel_vertical: size=3; index=2; break;
			case sobel_horizontal: size=3; index=1; break;
			case roberts_vertical: size=2; index=4; break;
			case prewitt_vertical: size=3; index=6; break;
			case roberts_horizontal: size=2; index=3; break;
			case prewitt_horizontal: size=3; index=5; break;
		} T_reg[0].relloc(size,size); T_reg[0].set_array(filter[index]);
	return T_reg[0];}

	tensor integral_image(tensor A){ I_reg[3] = A;
		for(int i=0; i<I_reg[3].size[1]; i++){ for(int j=0; j<I_reg[3].size[0]; j++){

			if(i==0 and j!=0){ I_reg[3].data[I_reg[3].get_index(i,j)] +=
							   I_reg[3].data[I_reg[3].get_index(i,j-1)]; }

			else if(i!=0 and j==0){ I_reg[3].data[I_reg[3].get_index(i,j)] += 
									I_reg[3].data[I_reg[3].get_index(i-1,j)]; }

			else if(i!=0 and j!=0){ I_reg[3].data[I_reg[3].get_index(i,j)] += 
									I_reg[3].data[I_reg[3].get_index(i-1,j)] +
									I_reg[3].data[I_reg[3].get_index(i,j-1)] - 
									I_reg[3].data[I_reg[3].get_index(i-1,j-1)]; }
		}} return I_reg[3]; }

	tensor template_matching(tensor A, tensor B, float sens=0.90 ){
		I_reg[5].relloc(A.size[0]-B.size[0]+1, A.size[1]-B.size[1]+1);
	
		for(int i=I_reg[5].size[1]; i--;){for(int j=I_reg[5].size[0]; j--;){float a=0,b=0,c=0;
			for(int k=B.size[1]; k--;){for(int l=B.size[0]; l--;){
				
				a += B.data[B.get_index(k,l)] * A.data[A.get_index(i+k,j+l)];
				c += pow(A.data[A.get_index(i+k,j+l)],2);
				b += pow(B.data[B.get_index(k,l)],2);}}

		I_reg[5].data[I_reg[5].get_index(i,j)] = a/sqrt(b*c);}}
		I_reg[5] = thresholding( I_reg[5],sens );
		//I_reg[5] = padding( I_reg[5], B.size[0]/4 );
		I_reg[4] = labelling( I_reg[5] );

	return I_reg[4];}

///##########################################################################///
///								HAAR CASCADE FILTERS						 ///
///##########################################################################///

float haar_filter_edge_hor(tensor int_img, int i, int j, int sy, int sx){

	//filtro negro
	return(int_img.data[int_img.get_index(i-1,j+sx-1)]    - //NA
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    - //NB
		int_img.data[int_img.get_index(i-1,j+2*sx-1)]     +	//NC
		int_img.data[int_img.get_index(i+sy-1,j+2*sx-1)]) -	//ND

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j-1)]         - //BA
		int_img.data[int_img.get_index(i+sy-1,j-1)]       - //BB
		int_img.data[int_img.get_index(i-1,j+sx-1)]       +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]);	//BD
}

float haar_filter_edge_ver(tensor int_img, int i, int j, int sy, int sx){

	//filtro negro
	return(int_img.data[int_img.get_index(i+sy-1,j-1)]    - //NA
		int_img.data[int_img.get_index(i+2*sy-1,j-1)]     - //NB
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    +	//NC
		int_img.data[int_img.get_index(i+2*sy-1,j+sx-1)]) -	//ND

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j-1)]         - //BA
		int_img.data[int_img.get_index(i+sy-1,j-1)]       - //BB
		int_img.data[int_img.get_index(i-1,j+sx-1)]       +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]);	//BD
}

float haar_filter_line_hor(tensor int_img, int i, int j, int sy, int sx){

	//filtro negro
	return(int_img.data[int_img.get_index(i-1,j+sx-1)]    - //NA
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    - //NB
		int_img.data[int_img.get_index(i-1,j+2*sx-1)]     +	//NC
		int_img.data[int_img.get_index(i+sy-1,j+2*sx-1)]) -	//ND

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j-1)]         - //BA
		int_img.data[int_img.get_index(i+sy-1,j-1)]       - //BB
		int_img.data[int_img.get_index(i-1,j+sx-1)]       +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)])	  -	//BD

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j+2*sx-1)]    - //BA
		int_img.data[int_img.get_index(i+sy-1,j+2*sx-1)]  - //BB
		int_img.data[int_img.get_index(i-1,j+3*sx-1)]     +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+3*sx-1)]);	//BD
}

float haar_filter_line_ver(tensor int_img, int i, int j, int sy, int sx){

	//filtro negro
	return(int_img.data[int_img.get_index(i+sy-1,j-1)]    - //NA
		int_img.data[int_img.get_index(i+2*sy-1,j-1)]     - //NB
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    +	//NC
		int_img.data[int_img.get_index(i+2*sy-1,j+sx-1)]) -	//ND

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j-1)]         - //BA
		int_img.data[int_img.get_index(i+sy-1,j-1)]       - //BB
		int_img.data[int_img.get_index(i-1,j+sx-1)]       +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)])	  - //BD

	//filtro blanco
		(int_img.data[int_img.get_index(i+2*sy-1,j-1)]	  - //BA
		int_img.data[int_img.get_index(i+3*sy-1,j-1)]	  - //BB
		int_img.data[int_img.get_index(i+2*sy-1,j+sx-1)]  +	//BC
		int_img.data[int_img.get_index(i+3*sy-1,j+sx-1)]);	//BD
}

float haar_filter_outline(tensor int_img, int i, int j, int sy, int sx){

	//filtro negro
	return(int_img.data[int_img.get_index(i-1,j+sx-1)]    - //NA
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    - //NB
		int_img.data[int_img.get_index(i-1,j+2*sx-1)]     +	//NC
		int_img.data[int_img.get_index(i+sy-1,j+2*sx-1)]) +	//ND

	//filtro negro
		(int_img.data[int_img.get_index(i+sy-1,j-1)]      -	//NA
		int_img.data[int_img.get_index(i+2*sy-1,j-1)]     - //NB
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)]    +	//NC
		int_img.data[int_img.get_index(i+2*sy-1,j+sx-1)]) -	//ND

	//filtro blanco
		(int_img.data[int_img.get_index(i-1,j-1)]         - //BA
		int_img.data[int_img.get_index(i+sy-1,j-1)]       - //BB
		int_img.data[int_img.get_index(i-1,j+sx-1)]       +	//BC
		int_img.data[int_img.get_index(i+sy-1,j+sx-1)])	  -	//BD

	//filtro blanco
		(int_img.data[int_img.get_index(i+sy-1,j+sx-1)]   - //BA
		int_img.data[int_img.get_index(i+2*sy-1,j+sx-1)]  - //BB
		int_img.data[int_img.get_index(i+sy-1,j+2*sx-1)]  +	//BC
		int_img.data[int_img.get_index(i+2*sy-1,j+2*sx-1)]);//BD
}

///##########################################################################///
///								HAAR CASCADE CLASIFIER						 ///
///##########################################################################///
	
///##########################################################################///
///									Activation								 ///
///##########################################################################///
//TODO: Activation Functions #################################################//

// Clamp //
tensor clamp( tensor A, float min, float max ){
	for(int i=A.size[2]; i--; ){
		if(A.data[i] < min){ A.data[i] = min; }
		else if(A.data[i] > max){ A.data[i] = max; }}
	return A;}

// linear //
tensor lin(tensor X, bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){T_reg[5]=X;} else {T_reg[5].set_one();}
	return T_reg[5];}
		
// tangh //
tensor tnh(tensor X, bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=tanh(X.data[i]); }}
	else{ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=1-(pow(X.data[i],2)); }}
	return T_reg[5];}

// softplus //
tensor sfp(tensor X, bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=log(1+exp(X.data[i])); }}
	else{ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=1/(1+exp(-X.data[i])); }}
	return T_reg[5];}

// atang //
tensor atn(tensor X, bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=atan(X.data[i]); }}
	else{ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=1/(1+pow(X.data[i],2)); }}
	return T_reg[5];}

// sigmoid //
tensor log(tensor X,bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=1/(1+exp(-X.data[i])); }}
	else{ for(int i=X.size[2]; i--;){ T_reg[5].data[i]=X.data[i]*(1-X.data[i]); }}
	return T_reg[5];}
	
// Relu //
tensor rlu(tensor X,bool dev){ T_reg[5].relloc(X.size[0],X.size[1]);
	if(dev==true){ for(int i=X.size[2]; i--;){ if(X.data[i]>0){ T_reg[5].data[i]=X.data[i];}else{ T_reg[5].data[i]=0; }}}
	else{ for(int i=X.size[2]; i--;){ if(X.data[i]>0){ T_reg[5].data[i]=1; }else{ T_reg[5].data[i]=0; }}}
	return T_reg[5];}

// softmax //
tensor sfm(tensor X,bool dev){
	if(dev==true){ float acum=0.0;
		for(int i=X.size[2]; i--;){ acum += exp(X.data[i]); }
		for(int i=X.size[2]; i--;){ X.data[i] = exp(X.data[i])/acum; }
	} else { T_reg[5].relloc(1,X.size[2]);
		for(int i=X.size[2]; i--;){ for(int j=X.size[2]; j--;){
			if(i==j){continue;} T_reg[5].data[i] += X.data[j];}}
		for(int i=X.size[2]; i--;){ X.data[i] = X.data[i] * T_reg[5].data[i]; }}
return X;}

// gauss //
tensor gss(tensor X, tensor C, float T, bool dev){ T_reg[5].relloc(C.size[0],C.size[1]);
	if(dev==true){ for(int i=C.size[2]; i--;){ for(int j=X.size[2]; j--;){ 
		T_reg[5].data[i] = exp(-pow(X.data[j]-C.data[i],2) / (2*pow(T,2))); 
	}}} else{ 
		for(int i=C.size[2]; i--;){ for(int j=X.size[2]; j--;){ 
		T_reg[5].data[i] = exp(-pow(X.data[j]-C.data[i],2) / (2*pow(T,2))) * ((X.data[j]-C.data[i]) / (pow(T,2))); 
	}}}
	return T_reg[5];}

tensor activation(tensor X, int kernel, bool dev){
	switch(kernel){
		case relu:		return(rlu(X,dev));break;
		case atang:	 	return(atn(X,dev));break;
		case tangh:	 	return(tnh(X,dev));break;
		case linear:	return(lin(X,dev));break;
		case sigmoid: 	return(log(X,dev));break;
		case softmax:	return(sfm(X,dev));break;
		case softplus:	return(sfp(X,dev));break;
	}}
	
tensor cost_function(tensor Y, tensor A, int kernel){ T_reg[3].relloc(Y.size[0], Y.size[1]);
	for(int i=T_reg[3].size[2]; i--;){ switch(kernel){
		case mean_squared:  T_reg[3].data[i] = Y.data[i] - A.data[i]; break;
		case cross_entropy: T_reg[3].data[i] = Y.data[i]/(A.data[i]) - (1-Y.data[i])/(1-A.data[i]); break;
	}} return T_reg[3]; }

void flatten(tensor* A, int depth, tensor B, bool dev){
	if(dev){ for( int i=depth,size=0; i--;){ for( int j=A[0].size[2]; j--;){
		B.data[size] = A[i].data[j]; size++;
	}}} else {
		for( int i=depth,size=0; i--;){ for( int j=A[0].size[2]; j--;){ 
		A[i].data[j] = B.data[size]; size++;
	}}}
}

///##########################################################################///
///										MLP								 	 ///
///##########################################################################///
class mlp{
	public:
	/// Variables ###################################### layer state ///
		int cost;
		int i_layer;
		int topology_size;
						
	/// Variables ################################### layer features ///
		int* type;
		int* kernel;
		int* topology;
				
	/// Variables #################################### learning rate ///
		int optimization;
		float learning_rate;
		float acceleration_rate;

	/// layer #######################################################///
		tensor input, output;
		struct layer_struct{ tensor* Array = new tensor[6]; }* layer;
				
	/// neural network ################################# constructor ///
		mlp(int n_layer=3, float n_learning_rate=0.1, int n_optimization=sgd, int n_cost_type=mean_squared){
			i_layer = 0;
			cost = n_cost_type;
			acceleration_rate = 0.9;
			topology_size = n_layer;
			optimization = n_optimization;
			learning_rate = n_learning_rate;
					
			type = new int[topology_size];
			kernel = new int[topology_size];
			topology = new int[topology_size];
			layer = new layer_struct[topology_size-1];}
		
		void init(int n_layer=3, float n_learning_rate=0.1, int n_optimization=sgd, int n_cost_type=mean_squared){
			i_layer = 0;
			cost = n_cost_type;
			acceleration_rate = 0.9;
			topology_size = n_layer;
			optimization = n_optimization;
			learning_rate = n_learning_rate;
					
			type = new int[topology_size];
			kernel = new int[topology_size];
			topology = new int[topology_size];
			layer = new layer_struct[topology_size-1];}
			
	/// neural network ########################### compile functions ///
		void add_layer(int n_neuron, int k_neuron , int n_type){
			if(i_layer>0){kernel[i_layer-1] = k_neuron;}
			topology[i_layer] = n_neuron;
			type[i_layer] = n_type;
			i_layer++;}
				
		void compile(){
			if(topology_size < i_layer){Serial.printf("maximo de capas superado %d mlp\n",(i_layer-topology_size));exit(1);}
			else if(topology_size > i_layer){Serial.printf("capas faltantes %d mlp\n",(topology_size-i_layer));exit(1);}

			input.relloc(1,topology[0]); output.relloc(1,topology[topology_size-1]);
					
			for(int i=1; i<topology_size ;i++){
				layer[i-1].Array[5].relloc(1,topology[i]);layer[i-1].Array[5].set_value(-1);
				layer[i-1].Array[1].relloc(1,topology[i]);layer[i-1].Array[1].set_value(-1);}
	
			for(int i=0; i<topology_size-1 ;i++){
				layer[i].Array[4].relloc(topology[i],topology[i+1]);layer[i].Array[4].set_one();
				layer[i].Array[2].relloc(topology[i],topology[i+1]);layer[i].Array[2].set_random();}}
				
	/// neural network ############################# print functions ///
		void show_model(){for(int i=0; i<i_layer-1; i++){layer[i].Array[1].show_tensor();layer[i].Array[2].show_tensor();}}
		tensor get_loss(){ return ( layer[0].Array[2].transpose() * layer[0].Array[0]); }
		void show_output(){ layer[i_layer-2].Array[3].show_tensor();}
		tensor get_output(){ return layer[i_layer-2].Array[3]; }

	/// neural network ########################### traning functions ///
		//Array[0] = Loss
		//Array[1] = Bias
		//Array[2] = Weight
		//Array[3] = Output
		//Array[4] = Loss_Weight
		//Array[5] = Loss_Bias

		bool if_detect( float Value ){
			for(int i=layer[i_layer-2].Array[3].size[2]; i--;){
				if(layer[i_layer-2].Array[3].data[i]>Value){return true;}
		} return false; }
			
	/// neural network ########################### traning functions ///
		void forwrd(){
			for(int i=0; i<topology_size-1; i++){
				switch(type[i]){
				case input_layer: layer[i].Array[3] = activation(((layer[i].Array[2] * input) + layer[i].Array[1]),kernel[i],true); break;
				default: layer[i].Array[3] = activation(((layer[i].Array[2] * layer[i-1].Array[3]) + layer[i].Array[1]),kernel[i],true); break;
		}}}
				
	/// neural network ################ Gradien descent with momentum ///
		void backwrd(){
			switch(optimization){
				case momentum: if(topology_size>2){
	
					for(int i=topology_size-1; i--;){
						layer[i].Array[5] *= acceleration_rate;
						layer[i].Array[4] *= acceleration_rate;
						
						if(type[i+1]==output_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > cost_function(output, layer[i].Array[3],cost);
							layer[i].Array[4] += layer[i].Array[0] * layer[i-1].Array[3].transpose();
							layer[i].Array[5] += layer[i].Array[0];}
	
						else if(type[i]==hidden_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > (layer[i+1].Array[2].transpose() * layer[i+1].Array[0]);
							layer[i].Array[4] += layer[i].Array[0] * layer[i-1].Array[3].transpose();
							layer[i].Array[5] += layer[i].Array[0];}
	
						else{
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > (layer[i+1].Array[2].transpose() * layer[i+1].Array[0]);
							layer[i].Array[4] += layer[i].Array[0] * input.transpose();
							layer[i].Array[5] += layer[i].Array[0];}

						layer[i].Array[2] += layer[i].Array[4] * learning_rate;
						layer[i].Array[1] += layer[i].Array[5] * learning_rate;
					}}
	
					else{
						layer[0].Array[0]  = activation(layer[0].Array[3],kernel[0],false) > cost_function(output, layer[0].Array[3],cost);
						
						layer[0].Array[5] *= acceleration_rate;
						layer[0].Array[4] *= acceleration_rate;

						layer[0].Array[5] += layer[0].Array[0];
						layer[0].Array[4] += layer[0].Array[0] * input.transpose();
						
						layer[0].Array[2] += layer[0].Array[4] * learning_rate;
						layer[0].Array[1] += layer[0].Array[5] * learning_rate;}
					break;
						
				default: if(topology_size>2){
					for(int i=topology_size-1; i--;){
	
						if(type[i+1]==output_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > cost_function(output, layer[i].Array[3],cost);
							layer[i].Array[2] += layer[i].Array[0] * layer[i-1].Array[3].transpose() * learning_rate;}
	
						else if(type[i]==hidden_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > (layer[i+1].Array[2].transpose() * layer[i+1].Array[0]);
							layer[i].Array[2] += layer[i].Array[0] * (layer[i-1].Array[3]).transpose() * learning_rate;}
	
						else{
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > (layer[i+1].Array[2].transpose() * layer[i+1].Array[0]);
							layer[i].Array[2] += layer[i].Array[0] * input.transpose() * learning_rate;}
						layer[i].Array[1] += layer[i].Array[0] * learning_rate;
					}}
				else{
					layer[0].Array[0]  = activation(layer[0].Array[3],kernel[0],false) > cost_function(output, layer[0].Array[3],cost);
					layer[0].Array[2] += layer[0].Array[0] * input.transpose() * learning_rate;
					layer[0].Array[1] += layer[0].Array[0] * learning_rate;}
				break;
				}}
			
		void fit(float* X, float* Y, float norm=1){
			input.set_array(X); output.set_array(Y);
			input /= norm; output /= norm;
			forwrd(); backwrd();}
				
		void predict(float* X, float norm=1){
			input.set_array(X); input /= norm; 
			forwrd(); show_output();}
};

///##########################################################################///
///										RBFNN							 	 ///
///##########################################################################///
class rbfnn{
	public:
	/// Variables ###################################### layer state ///
		int cost;
		int i_layer;
		int topology_size;
						
	/// Variables ################################### layer features ///
		int* type;
		int* kernel;
		int* topology;
				
	/// Variables #################################### learning rate ///
		int optimization;
		float learning_rate;
		float acceleration_rate;

	/// layer #######################################################///
		tensor input, output;
		struct layer_struct{ tensor* Array = new tensor[6]; }* layer;
	/// neural network ########################### traning functions ///
		//Array[0] = Loss
		//Array[1] = Bias
		//Array[2] = Weight
		//Array[3] = Output
		//Array[4] = Loss_Weight
		//Array[5] = Loss_Bias
				
	/// neural network ################################# constructor ///
		rbfnn(float n_learning_rate=0.1, int n_optimization=sgd, int n_cost_type=mean_squared){
			i_layer = 0;
			topology_size = 3;
			cost = n_cost_type;
			acceleration_rate = 0.9;
			optimization = n_optimization;
			learning_rate = n_learning_rate;
					
			type = new int[topology_size];
			kernel = new int[topology_size];
			topology = new int[topology_size];
			layer = new layer_struct[topology_size-1];}
			
	/// neural network ########################### compile functions ///
		void add_layer(int n_neuron, int k_neuron , int n_type){
			if(i_layer>0){kernel[i_layer-1] = k_neuron;}
			topology[i_layer] = n_neuron;
			type[i_layer] = n_type;
			i_layer++;}
				
		void compile(){
			if(topology_size < i_layer){Serial.printf("maximo de capas superado %d rbf\n",(i_layer-topology_size));exit(1);}
			else if(topology_size > i_layer){Serial.printf("capas faltantes %d rbf\n",(topology_size-i_layer));exit(1);}
					
			input.relloc(1,topology[0]); output.relloc(1,topology[topology_size-1]);

			for(int i=1; i<topology_size ;i++){
				layer[i-1].Array[5].relloc(1,topology[i]);layer[i-1].Array[5].set_value(-1);
				layer[i-1].Array[1].relloc(1,topology[i]);layer[i-1].Array[1].set_value(-1);}
	
			for(int i=0; i<topology_size-1 ;i++){
				layer[i].Array[4].relloc(topology[i],topology[i+1]);layer[i].Array[4].set_one();
				layer[i].Array[2].relloc(topology[i],topology[i+1]);layer[i].Array[2].set_random();}
			}
				
	/// neural network ############################# print functions ///
		void show_model(){for(int i=0; i<i_layer-1; i++){layer[i].Array[1].show_tensor();layer[i].Array[2].show_tensor();}}
		void show_output(){ layer[i_layer-2].Array[3].show_tensor();}
		tensor get_output(){ return layer[i_layer-2].Array[3]; }
			
	/// neural network ########################### traning functions ///
		void forwrd(){
			for(int i=0; i<topology_size-1; i++){
				switch(type[i]){
				case input_layer: layer[i].Array[3] = gss( input, layer[i].Array[1], 4, true ); break;
				default: layer[i].Array[3] = activation(((layer[i].Array[2] * layer[i-1].Array[3]) + layer[i].Array[1]),kernel[i],true); break;
		}}}
				
	/// neural network ################ Gradien descent with momentum ///
		void backwrd(){
			switch(optimization){
				case momentum:	
					for(int i=topology_size-1; i--;){
						if(type[i+1]==output_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > cost_function(output, layer[i].Array[3],cost);
							
							layer[i].Array[5] *= acceleration_rate;
							layer[i].Array[4] *= acceleration_rate;

							layer[i].Array[5] += layer[i].Array[0];
							layer[i].Array[4] += layer[i].Array[0] * layer[i-1].Array[3].transpose();
														
							layer[i].Array[2] += layer[i].Array[4] * learning_rate;
							layer[i].Array[1] += layer[i].Array[5] * learning_rate;}
	
						else{layer[i].Array[2].relloc(0,0); layer[i].Array[4].relloc(0,0);
							layer[i].Array[0]  = (layer[i+1].Array[2].transpose() * layer[i+1].Array[0]);
							layer[i].Array[5] *= acceleration_rate;
							layer[i].Array[5] += layer[i].Array[0] > gss(input, layer[i].Array[1], 4, false);
							layer[i].Array[1] += layer[i].Array[5] * learning_rate;}}
				break;
						
				default:	
					for(int i=topology_size-1; i--;){
						if(type[i+1]==output_layer){
							layer[i].Array[0]  = activation(layer[i].Array[3],kernel[i],false) > cost_function(output, layer[i].Array[3],cost);
							layer[i].Array[2] += layer[i].Array[0] * layer[i-1].Array[3].transpose() * learning_rate;
							layer[i].Array[1] += layer[i].Array[0] * learning_rate;}
	
						else{layer[i].Array[2].relloc(0,0);
							layer[i].Array[0]  = layer[i+1].Array[2].transpose() * layer[i+1].Array[0];
							layer[i].Array[1] += layer[i].Array[0] > gss(input, layer[i].Array[1], 4, false) * learning_rate;}}
				break;
			}}
			
	void fit(float* X, float* Y){
		input.set_array(X); output.set_array(Y); 
		forwrd(); backwrd();}
				
	void predict(float* X){
		input.set_array(X); forwrd(); show_output();}
};

///##########################################################################///
///										RNN								 	 ///
///##########################################################################///
class rnn{
	public:
	/// Variables ###################################### layer state ///
		int cost;
		int i_layer;
		int topology_size;
						
	/// Variables ################################### layer features ///
		int* type;
		int* kernel;
		int* topology;
				
	/// Variables #################################### learning rate ///
		int optimization;
		float learning_rate;
		float acceleration_rate;
				
	/// layer #######################################################///
		tensor input, output;
		struct layer_struct{ tensor* Array = new tensor[9]; }* layer;
	/// neural network ########################### traning functions ///
		//Array[0] = weight_tmp
		//Array[1] = Loss
		//Array[2] = Bias
		//Array[3] = output_tmp
		//Array[4] = weight
		//Array[5] = Output
		//Array[6] = loss_weight
		//Array[7] = loss_bias
		//Array[8] = loss_weight_tmp
				
	/// neural network ################################# constructor ///
		rnn(int n_layer=3, float n_learning_rate=0.1, int n_optimization=sgd, int n_cost_type=mean_squared){
			i_layer = 0;
			cost = n_cost_type;
			topology_size = n_layer;
			acceleration_rate = 0.9;
			optimization = n_optimization;
			learning_rate = n_learning_rate;
				
			type = new int[topology_size];
			kernel = new int[topology_size];
			topology = new int[topology_size];
			layer = new layer_struct[topology_size-1];}
				
	/// neural network ########################### compile functions ///
		void add_layer(int n_neuron, int k_neuron , int n_type){
			if((i_layer) > 0){kernel[i_layer-1] = k_neuron;}
			topology[i_layer] = n_neuron;
			type[i_layer] = n_type;
			i_layer++;}
				
		void compile(){
			if(topology_size < i_layer){Serial.printf("maximo de capas superado %d RNN\n",(i_layer-topology_size));exit(1);}
			else if(topology_size > i_layer){Serial.printf("capas faltantes %d RNN\n",(topology_size-i_layer));exit(1);}
			
			output.relloc(1,topology[topology_size-1]); input.relloc(1,topology[0]);

			for(int i=1; i<topology_size ;i++){
				layer[i-1].Array[8].relloc(topology[i],topology[i]);layer[i-1].Array[8].set_value(-1);
				layer[i-1].Array[2].relloc(1,topology[i]);layer[i-1].Array[2].set_value(-1);
				layer[i-1].Array[7].relloc(1,topology[i]);layer[i-1].Array[7].set_value(-1);
				layer[i-1].Array[5].relloc(1,topology[i]);layer[i-1].Array[5].set_zero();
				layer[i-1].Array[3].relloc(1,topology[i]);layer[i-1].Array[3].set_zero();}
						
			for(int i=0; i<topology_size-1 ;i++){
				layer[i].Array[6].relloc(topology[i],topology[i+1]);layer[i].Array[6].set_one();
				layer[i].Array[0].relloc(topology[i],topology[i]);layer[i].Array[0].set_random();
				layer[i].Array[4].relloc(topology[i],topology[i+1]);layer[i].Array[4].set_random();}}
				
	/// neural network ############################# print functions ///
		void show_model(){for(int i=0; i<i_layer-1; i++){layer[i].Array[2].show_tensor();layer[i].Array[4].show_tensor();}}
		void show_output(){layer[i_layer-2].Array[5].show_tensor();}
		tensor get_output(){ return layer[i_layer-2].Array[5]; }

	/// neural network ############################ FrontPropagation ///
		void forwrd(){
			for(int i=0; i<topology_size-1; i++){
				switch(type[i]){
					
				case hidden_layer:
					layer[i].Array[5] = layer[i].Array[4] * layer[i-1].Array[5] + layer[i].Array[2];
					layer[i-1].Array[3] = layer[i].Array[0] * layer[i-1].Array[5];
					layer[i].Array[5] += layer[i].Array[3];
					break;
					
				case input_layer:
					layer[i].Array[5] = layer[i].Array[4] * input + layer[i].Array[2];
					layer[i].Array[5] += layer[i].Array[3];
					break;
					
				default: layer[i].Array[5] = layer[i].Array[4] * layer[i-1].Array[5] + layer[i].Array[2];break; }
						
				layer[i].Array[5] = activation(layer[i].Array[5],kernel[i],true);}}

	/// neural network ############################# BackPropagation ///
	/// neural network ############### Gradien descent with momentum ///
		void backwrd(){
			switch(optimization){
				case momentum:
					for(int i=topology_size-1; i--;){
						
						if(type[i+1]==output_layer){
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > cost_function(output, layer[i].Array[5],cost);
							layer[i].Array[7] *= acceleration_rate;
							layer[i].Array[6] *= acceleration_rate;
							layer[i].Array[7] += layer[i].Array[1];
							layer[i].Array[6] += layer[i].Array[1] * (layer[i-1].Array[5]).transpose();
							layer[i].Array[2] += layer[i].Array[7] * learning_rate;
							layer[i].Array[4] += layer[i].Array[6] * learning_rate;}
	
						else if(type[i]==hidden_layer){
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > (layer[i+1].Array[4].transpose() * layer[i+1].Array[1]);

							layer[i].Array[8] *= acceleration_rate;
							layer[i].Array[7] *= acceleration_rate;
							layer[i].Array[6] *= acceleration_rate;

							layer[i].Array[6] += layer[i].Array[1] * layer[i-1].Array[5].transpose();
							layer[i].Array[8] += layer[i].Array[1] * layer[i].Array[3].transpose();
							layer[i].Array[7] += layer[i].Array[1];

							layer[i].Array[0] += layer[i].Array[8] * learning_rate;
							layer[i].Array[2] += layer[i].Array[7] * learning_rate;
							layer[i].Array[4] += layer[i].Array[6] * learning_rate;}
	
						else{
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > (layer[i+1].Array[4].transpose() * layer[i+1].Array[1]);
							layer[i].Array[6] += layer[i].Array[1] * input.transpose();
							layer[i].Array[7] += layer[i].Array[1];
							layer[i].Array[4] += layer[i].Array[6] * learning_rate;
							layer[i].Array[2] += layer[i].Array[7] * learning_rate;}

					}break;
						
	/// neural network ############### Gradien descent without momentum ///
				default:
					for(int i=topology_size-1; i--;){
						if(type[i+1]==output_layer){
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > cost_function(output, layer[i].Array[5],cost);
							layer[i].Array[4] += layer[i].Array[1] * layer[i-1].Array[5].transpose() * learning_rate;}
	
						else if(type[i]==hidden_layer){
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > (layer[i+1].Array[4].transpose() * layer[i+1].Array[1]);
							layer[i].Array[4] += layer[i].Array[1] * layer[i-1].Array[5].transpose() * learning_rate;
							layer[i].Array[0] += layer[i].Array[1] * layer[i].Array[3].transpose() * learning_rate;}
	
						else{
							layer[i].Array[1]  = activation(layer[i].Array[5],kernel[i],false) > (layer[i+1].Array[4].transpose() * layer[i+1].Array[1]);
							layer[i].Array[4] += layer[i].Array[1] * input.transpose() * learning_rate;}

						layer[i].Array[2] += layer[i].Array[1] * learning_rate;
				}break;
			}}
				
		void fit(float* X, float* Y, float norm=1){
			output.set_array(Y); input.set_array(X);
			input /= norm; output /= norm;
			forwrd();backwrd();}
					
		void predict(float* X, float norm=1){
			input.set_array(X); input /= norm;
			forwrd(); show_output();}
};

///##########################################################################///
///						  MODEL EXPORTER IMPORTER MLP					 	 ///
///##########################################################################///

void model_export(mlp MODEL){
	int depth = MODEL.i_layer-1,b=0,w=0;

	//Model Initializer
	for(int i=depth; i--;){
		b += MODEL.layer[i].Array[1].size[2];
		w += MODEL.layer[i].Array[2].size[2];}

	//Model Relloc
	T_reg[0].relloc(1,w); 
	T_reg[1].relloc(1,b);

	//Model flatten
	for(int i=depth,sb=0,sw=0; i--;){ 
		for(int j=MODEL.layer[i].Array[2].size[2]; j--;){ //Width Initialization
			T_reg[0].data[sw] = MODEL.layer[i].Array[2].data[j];sw++;}
		for(int j=MODEL.layer[i].Array[1].size[2]; j--;){ //Bias Initialization
			T_reg[1].data[sb] = MODEL.layer[i].Array[1].data[j];sb++;}}
	T_reg[0].show_tensor();
	T_reg[1].show_tensor();
}

void model_import(float* width, float* bias, mlp MODEL){
	int depth = MODEL.i_layer-1;

	//Model Deflatten
	for(int i=depth,sb=0,sw=0; i--;){ 
		for(int j=MODEL.layer[i].Array[1].size[2]; j--;){ //Bias Initialization
			MODEL.layer[i].Array[1].data[j] = bias[sb];sb++;}
		for(int j=MODEL.layer[i].Array[2].size[2]; j--;){ //Width Initialization
			MODEL.layer[i].Array[2].data[j] = width[sw];sw++;}}
}