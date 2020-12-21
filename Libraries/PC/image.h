#include "opencv2/opencv.hpp"

using namespace cv;

///##########################################################################///
///									Color Image 						 	 ///
///##########################################################################///

//TODO: get_Image ############################################################//
tensor* img2tensor_color(const char* path){ Mat img_cv = imread(path);
	for(int i=3; i--;){ I_img[i].relloc(img_cv.cols,img_cv.rows);}
	for(int i=3; i--;){ for(int j=I_img[0].size[2]; j--;){ I_img[i].data[j] = img_cv.data[j*3+i];
	} I_img[i] /= 255; }
return I_img;}

tensor* img2tensor_color(Mat img_cv){
	for(int i=3; i--;){ I_img[i].relloc(img_cv.cols,img_cv.rows);}
	for(int i=3; i--;){ for(int j=I_img[0].size[2]; j--;){ I_img[i].data[j] = img_cv.data[j*3+i];
	} I_img[i] /= 255; }
return I_img;}

//TODO: show_Image ###########################################################//
Mat tensor2img_color(tensor* img){
	Mat img_cv(img[0].size[0],img[0].size[1],CV_8UC3);
	for(int i=3; i--;){ for(int j=img[0].size[2]; j--;){
		img_cv.data[j*3+i] = img[i].data[j]*255; }}
return img_cv;}

//TODO: show Image ###########################################################//
void show_image(tensor* img){
	char windowName[10];
	sprintf(windowName,"(%d,%d)\n",img[0].size[0],img[0].size[1]);
	printf("%s\n",windowName);
	namedWindow(windowName, WINDOW_NORMAL); imshow(windowName, tensor2img_color(img));
	waitKey(0); destroyWindow(windowName);}

//TODO: save Image ###########################################################//
void save_image( string PATH,tensor* img ){ imwrite(PATH,tensor2img_color(img)); }

///##########################################################################///
///									gray Image 							 	 ///
///##########################################################################///

//TODO: get Image ############################################################//
tensor img2tensor(const char* path){
	Mat img_cv = imread(path);
	I_reg[0].relloc(img_cv.cols, img_cv.rows);
	for(int i=3; i--;){ for(int j=I_reg[0].size[2]; j--;){ I_reg[0].data[j] += img_cv.data[j*3+i]; }}
return I_reg[0]/(3*255);}

tensor img2tensor(Mat img_cv){
	I_reg[0].relloc(img_cv.cols, img_cv.rows);
	for(int i=3; i--;){ for(int j=I_reg[0].size[2]; j--;){ I_reg[0].data[j] += img_cv.data[j*3+i]; }}
return I_reg[0]/(3*255);}

//TODO: pre_show Image #######################################################//
Mat tensor2img(tensor img){
	Mat img_cv(img.size[0],img.size[1],CV_8UC3);
	for (int i=3; i--;){ for(int j=img.size[2]; j--;){ img_cv.data[j*3+i] = img.data[j]*255; }}
return img_cv;}

Mat tensor2img(tensor img, tensor label){
	Mat img_cv(img.size[0],img.size[1],CV_8UC3);
	for (int i=3; i--;){ for(int j=img.size[2]; j--;){ img_cv.data[j*3+i] = img.data[j]*255; }}

	for(int i=label.size[1]; i--;){
		Rect RectangleToDraw(img.size[0] * label.data[label.get_index(i,0)],
							 img.size[1] * label.data[label.get_index(i,1)],
							 img.size[0] * ( label.data[label.get_index(i,2)] - label.data[label.get_index(i,0)] ), 
							 img.size[1] * ( label.data[label.get_index(i,3)] - label.data[label.get_index(i,1)] ));
		rectangle(img_cv, RectangleToDraw ,Scalar(0,0,255), 2,8,0);}
return img_cv;}

//TODO: show Image ###########################################################//
void show_image(tensor img){
	char windowName[10];
	sprintf(windowName,"(%d,%d)\n",img.size[0],img.size[1]);
	printf("%s\n",windowName);
	namedWindow(windowName,WINDOW_NORMAL); imshow(windowName, tensor2img(img));
	waitKey(0); destroyWindow(windowName);}

void show_image(tensor img, tensor label){
	char windowName[10];
	sprintf(windowName,"(%d,%d)\n",img.size[0],img.size[1]);
	printf("%s\n",windowName);
	namedWindow(windowName,WINDOW_NORMAL); imshow(windowName, tensor2img(img,label));
	waitKey(0); destroyWindow(windowName);}

//TODO: save Image ###########################################################//
void save_image( string PATH,tensor img ){ imwrite(PATH,tensor2img(img)); }
void save_image( string PATH,tensor img, tensor label ){ imwrite(PATH,tensor2img(img,label)); }
