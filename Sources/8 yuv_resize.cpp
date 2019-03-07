/**
 * @file 8 yuv_resize.cpp
 * @author luohen
 * @brief adjusting yuv image size
 * @date 2018-12-08
 *
 */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <iostream>

using namespace std;

#define HEIGHT 288
#define WIDTH 352

/**
 * @brief 
 * 
 * @param url			location of input yuv420p file
 * @param out_width		output image width
 * @param out_height	output image height
 * @return int 
 */
int yuv420_resize(const char *url, int out_width, int out_height)
{
	//input array
	unsigned char yin[HEIGHT][WIDTH];
	unsigned char uin[HEIGHT / 2][WIDTH / 2];
	unsigned char vin[HEIGHT / 2][WIDTH / 2];
	//output array
	unsigned char *yout = new unsigned char[out_width * out_height];
	unsigned char *uout = new unsigned char[out_width / 2 * out_height / 2];
	unsigned char *vout = new unsigned char[out_width / 2 * out_height / 2];
	///reading yuv file
	FILE *input_fp;
	//writing yuv file
	FILE *output_fp = fopen("video_result/output_resize.yuv", "wb+");

	if ((input_fp = fopen(url, "rb")) == NULL)
	{
		printf("%s open error!\n", url);
		return -1;
	}
	else
	{
		printf("%s open.\n", url);
	}

	fread(yin, sizeof(unsigned char), HEIGHT * WIDTH, input_fp);
	fread(uin, sizeof(unsigned char), HEIGHT * WIDTH / 4, input_fp);
	fread(vin, sizeof(unsigned char), HEIGHT * WIDTH / 4, input_fp);

	//Y
	for (int i = 0; i < out_height; i++)
	{
		for (int j = 0; j < out_width; j++)
		{
			int i_in = round(i * HEIGHT / out_height);
			int j_in = round(j * WIDTH / out_width);
			yout[i * out_width + j] = yin[i_in][j_in];
		}
	}

	//U
	for (int i = 0; i < out_height / 2; i++)
	{
		for (int j = 0; j < out_width / 2; j++)
		{
			int i_in = round(i * (HEIGHT / 2) / (out_height / 2));
			int j_in = round(j * (WIDTH / 2) / (out_width / 2));
			uout[i * out_width / 2 + j] = uin[i_in][j_in];
		}
	}

	//V
	for (int i = 0; i < out_height / 2; i++)
	{
		for (int j = 0; j < out_width / 2; j++)
		{
			int i_in = round(i * (HEIGHT / 2) / (out_height / 2));
			int j_in = round(j * (WIDTH / 2) / (out_width / 2));
			vout[i * out_width / 2 + j] = vin[i_in][j_in];
		}
	}

	fwrite(yout, sizeof(unsigned char), out_width * out_height, output_fp);
	fwrite(uout, sizeof(unsigned char), out_width * out_height / 4, output_fp);
	fwrite(vout, sizeof(unsigned char), out_width * out_height / 4, output_fp);

	delete[] yout;
	delete[] uout;
	delete[] vout;
	fclose(input_fp);
	fclose(output_fp);

	return 0;
}

/**
 * @brief main
 *
 * @return int
 */
int main()
{
	int state = yuv420_resize("video/akiyo.yuv", 288, 352);
	return 0;
}