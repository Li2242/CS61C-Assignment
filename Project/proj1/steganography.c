/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				LZY
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	if(image == NULL){
		printf("传入函数evaluateOnePixel的值为NULL\n");
		return NULL;
	}
	Color* color = (Color *)malloc(sizeof(Color));
	if(color == NULL){
		return NULL;
	}

	Color pixel = image->image[row][col];
	int lsb = pixel.B & 1;
	if(lsb == 1){
		color->R = 255;
		color->G = 255;
		color->B = 255;
	}else{
		color->R = 0;
		color->G = 0;
		color->B = 0;
	}
	
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	if(image == NULL){
		return NULL;
	}
	//创建一个新的Image结构体
	Image *new_image = (Image *)malloc(sizeof(Image));
	if(new_image == NULL){
		return NULL;
	}

	// 复制图像尺寸
	new_image->rows = image->rows;
	new_image->cols = image->cols;
	

	new_image->image = (Color **)malloc(sizeof(Color*)*image->rows);
	if(new_image->image == NULL){
		free(new_image);
	}
	for(int i = 0;i < image->rows; i++){
		new_image->image[i] = (Color *)malloc(sizeof(Color)*image->cols);
		if(new_image->image[i] == NULL){
			for(int j =0; j < i; j++){
				free(new_image->image[j]);
			}
			free(new_image->image);
			free(new_image);
			return NULL;
		}
	}
	//遍历元素,提取LSB
	for(int r = 0; r<image->rows; r++){
		for(int c = 0; c<image->cols; c++){
			
			Color *newColor = evaluateOnePixel(image,r,c);
			new_image->image[r][c] = *newColor;
			free(newColor);
		}
	}
	return new_image;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	// 检查参数数量
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(-1);
    }
    
    // 读取图像
    Image *original = readData(argv[1]);
    if (original == NULL) {
        exit(-1);
    }
    
    // 提取LSB
    Image *result = steganography(original);
    if (result == NULL) {
        freeImage(original);  // 假设有freeImage函数
        exit(-1);
    }
    
    // 输出结果
    writeData(result);
    
    // 释放内存
    freeImage(original);
    freeImage(result);
    
    return 0;
}
