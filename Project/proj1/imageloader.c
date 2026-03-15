/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				LZY
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	Image* new = (Image *)malloc(sizeof(Image));
	if(new == NULL){
		printf("read Data failure!\n");
		exit(0);
	}

	FILE* fp  = fopen(filename, "r");
	if(fp == NULL){
		perror("文件无法打开");
		free(new);
		return NULL;
	}
	char magic[3];
	fscanf(fp, "%2s", magic);

	//读取宽高
	uint32_t row,col;
	fscanf(fp, "%u %u",&row,&col);
	new->cols = col;
	new->rows = row;
	
	int maxval;
	fscanf(fp, "%d", &maxval);



	//分配行
	new->image =  (Color **)malloc(sizeof(Color *) * row);
	if(new->image == NULL){
		free(new);
		return NULL;
	}

	//分配列
	for(uint32_t i = 0; i < row; i++){
		new->image[i] = (Color*)malloc(col * sizeof(Color));
		if(new->image[i] == NULL){
			for(uint32_t j = 0; j < i; j++){
				free(new->image[j]);
			}
			free(new->image);
			free(new);
			return NULL;
		}
	}
	
	//读取像素
	for(int r = 0; r < new->rows; r++){
		for(int c = 0; c < new->cols; c++){
			int r1,g,b;
			int count = fscanf(fp,"%d %d %d",&r1,&g,&b);
			if(count != 3 ){
				printf("readData: Failed to read pixel at [%u,%u], read %d values\n",r,c,count);
				freeImage(new);
				fclose(fp);
				return NULL;
			}
			new->image[r][c].R = (uint8_t)r1;
			new->image[r][c].G = (uint8_t)g;
			new->image[r][c].B = (uint8_t)b;
		}
	}

	fclose(fp);
	return new;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%u %u\n",image->cols, image->rows);
	printf("255\n");
	for(int r = 0; r < image->rows; r++){
		for(int c = 0; c < image->cols; c++){
			Color pixel = image->image[r][c];
			printf("%u %u %u", pixel.R,pixel.G,pixel.B);
			if(c == image->cols - 1){
				printf("\n");
			}else{
				printf(" ");
			}
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for(int r = 0; r < image->rows; r++){
		free(image->image[r]);
	}
	free(image->image);
	free(image);
}