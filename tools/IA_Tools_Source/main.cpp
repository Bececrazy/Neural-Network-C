#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"

#include "../IA/tensor.h"
#include "../IA/image.h"

using namespace std;

class TOOL_CLASS{
	public:

		tensor* sect_img = new tensor[3];
		tensor* image = new tensor[3];

		int section[4];
		int size = 100;
		
		char path[100];
		char cmd[30];
		
		int color;
}tool;

void show_menu(){

	printf("####################################################################\n");
	printf("#                              IA Tools                            #\n");
	printf("####################################################################\n\n");
	
	printf("#                          list of commands                        #\n");
	printf("#                          ################                        #\n\n");
	
	printf("\t|Show image: show_image\n\n");
		
	printf("\t|ends the program: exit\n\n");
		
	printf("\t|clear command line: clear\n\n");

	printf("\t|Get image: get_image <path>\n\n");
			
	printf("\t|Decrease image: decrease_image <size>\n\n");
		
	printf("\t|Get image section: get_section <X1> <Y1> <X2> <Y2>\n\n");

	printf("####################################################################\n");
	printf("#                                                                  #\n");	
	printf("####################################################################\n\n");}

void get_command(){ printf(":> "); scanf("%s",tool.cmd);
	
	/*	####### IMAGE COMMANDS ####### */

	if( strcmp(tool.cmd,"get_image")==0 ){ scanf("%s",tool.path);
		switch( tool.color ){
			case 1: 
				tool.image = img2tensor_color(tool.path),tool.size;
				for(int i=3; i--;){ tool.image[i] = decrease_image(tool.image[i],tool.size); }
			break;
			default: tool.image[0] = decrease_image( img2tensor(tool.path),tool.size ); break;
		}}

	if( strcmp(tool.cmd,"get_cam_image")==0 ){
		Mat frame;
		VideoCapture cap(0); cap.read(frame);
		switch( tool.color ){
			case 1:
				tool.image = img2tensor_color(frame),tool.size;
				for(int i=3; i--;){ tool.image[i] = decrease_image(tool.image[i],tool.size); }
			break;
			default: tool.image[0] = decrease_image( img2tensor(frame),tool.size ); break;
		}}

	else if( strcmp(tool.cmd,"get_image_list")==0 ){}
		
	else if( strcmp(tool.cmd,"show_image")==0 ){ 
		switch(tool.color){
			case 1: show_image(tool.image); break;
			default: show_image(tool.image[0]); break;
		}}

	else if( strcmp(tool.cmd,"save_image")==0 ){ char save[100]; scanf("%s",save);
		switch(tool.color){
			case 1: save_image( save,tool.image ); break;
			default: save_image( save,tool.image[0] ); break;
		}}

	/*	####### PRE PROCESSING COMMANDS ####### */

	else if( strcmp(tool.cmd,"set_color_image")==0 ){tool.color = true;}
	else if( strcmp(tool.cmd,"set_gray_image")==0 ){tool.color = false;}
	
	/*	####### SECTION & LABELLING COMMANDS ####### */

	else if( strcmp(tool.cmd,"get_section")==0 ){
		for(int i=4; i--;){ scanf("%d",&tool.section[i]); }

		if( tool.section[3]>=tool.section[1]
		or 	tool.section[2]>=tool.section[0] ){

			tool.section[0] = 0; tool.section[1] = 0;
			tool.section[2] = 0; tool.section[3] = 0;
			puts("wrong section values"); }

		else if( tool.section[3] >tool.image[0].size[0]
			 or  tool.section[2] >tool.image[0].size[1] 
			 or  tool.section[1] >tool.image[0].size[0]
			 or  tool.section[0] >tool.image[0].size[1] ){

			tool.section[0] = 0; tool.section[1] = 0;
			tool.section[2] = 0; tool.section[3] = 0;
			puts("the section is bigger than the image"); }

		switch(tool.color){
			case 1: for(int l=3; l--;){
				tool.sect_img[l].relloc( tool.section[0]-tool.section[2],tool.section[1]-tool.section[3] );
				for(int i=tool.section[2],k=0; i<tool.section[0] ;i++){ for( int j=tool.section[3]; j<tool.section[1]; j++ ){ 
					tool.sect_img[l].data[k] = tool.image[l].data[tool.image[0].get_index(i,j)];
				k++; }}
			}break;

			default:
				tool.sect_img[0].relloc( tool.section[0]-tool.section[2],
										 tool.section[1]-tool.section[3] );
				for(int i=tool.section[2],k=0; i<tool.section[0] ;i++){
					for( int j=tool.section[3]; j<tool.section[1]; j++ ){ 
						tool.sect_img[0].data[k] = tool.image[0].data[tool.image[0].get_index(i,j)];
				k++; }}
			break;
	}}
	
	else if( strcmp(tool.cmd,"show_section")==0 ){
		switch(tool.color){
			case 1: show_image(tool.sect_img); break;
			default: show_image(tool.sect_img[0]); break;
		}}

	else if( strcmp(tool.cmd,"save_section")==0 ){ char save[100]; scanf("%s",save);
		switch(tool.color){
			case 1: save_image( save,tool.sect_img ); break;
			default: save_image( save,tool.sect_img[0] ); break;
		}}

	/*	####### PROCESSING COMMANDS ####### */
	
	else if( strcmp(tool.cmd,"decrease_image")==0 ){ scanf("%d",&tool.size);
		switch(tool.color){
			case 1: for(int i=3; i--;){ tool.image[i] = decrease_image( tool.image[i],tool.size ); } break;
			default: tool.image[0] = decrease_image( tool.image[0],tool.size ); break;
		}}
		
	/*	####### MENU COMMANDS ####### */

	else if( strcmp(tool.cmd,"tool_status")==0 ){
		printf(" Path: %s \n Section: %d,%d,%d,%d \n Size: %d \n",
		tool.path, tool.section[0],tool.section[1],tool.section[2],tool.section[3], tool.size); }

	else if( strcmp(tool.cmd,"clear")==0 ){ system("clear"); show_menu(); }

	else if( strcmp(tool.cmd,"exit")==0 ){ system("clear"); exit(1); }
	
	else{ printf("wrong command: %s\n",tool.cmd); }

	fflush(stdin);
}

int main(int argc, char **argv){ show_menu(); while(1) get_command(); return 0;}