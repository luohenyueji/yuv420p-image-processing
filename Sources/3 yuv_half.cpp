/**
 * @file 3 yuv_halfy.cpp
 * @author luohen
 * @brief Half of Y value
 * @date 2018-12-07
 *
 */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

/**
 * @brief
 *
 * @param url	location of input yuv420p file
 * @param w		width of input yuv420p file
 * @param h 	height of input yuv420p file
 * @return	int
 */
int yuv420_half(const char *url, int w, int h)
{
	//reading yuv image
	FILE *input_fp;
	if ((input_fp = fopen(url, "rb")) == NULL)
	{
		printf("%s open error!\n", url);
		return -1;
	}
	else
	{
		printf("%s open.\n", url);
	}

	//writing yuv image
	FILE *output_fp = fopen("video_result/output_half.yuv", "wb+");

	unsigned char *pic = new unsigned char[w * h * 3 / 2];

	fread(pic, sizeof(unsigned char), w * h * 3 / 2, input_fp);
	//half of Y
	for (int j = 0; j < w * h; j++)
	{
		unsigned char temp = pic[j] / 2;
		//printf("%d,\n",temp);
		pic[j] = temp;
	}
	fwrite(pic, 1, w * h * 3 / 2, output_fp);

	delete[] pic;
	fclose(input_fp);
	fclose(output_fp);
	return 0;
}

/**
 * @brief main函数
 *
 * @return int
 */
int main()
{
	int state = yuv420_half("video/akiyo.yuv", 352, 288);
	return 0;
}