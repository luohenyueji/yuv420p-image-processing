/**
 * @file 4 yuv_border.cpp
 * @author luohen
 * @brief  add a border to yuv
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
int yuv420_border(const char *url, int w, int h)
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
	FILE *output_fp = fopen("video_result/output_border.yuv", "wb+");

	//border width
	int border = 30;
	unsigned char *pic = new unsigned char[w * h * 3 / 2];

	//reading y
	fread(pic, 1, w * h * 3 / 2, input_fp);
	//y
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			if (k < border || k >(w - border) || j < border || j >(h - border))
			{
				//0最暗，255最亮
				pic[j * w + k] = 0;
				//pic[j*w+k]=255;
			}
		}
	}
	//u
	for (int j = 0; j < h / 2; j++)
	{
		for (int k = 0; k < w / 2; k++)
		{
			if (k < border / 2 || k >(w / 2 - border / 2) || j < border / 2 || j >(h / 2 - border / 2))
			{
				pic[w*h + j * w / 2 + k] = 128;
				//pic[j*w+k]=255;
			}
		}
	}
	//v
	for (int j = 0; j < h / 2; j++)
	{
		for (int k = 0; k < w / 2; k++)
		{
			if (k < border / 2 || k >(w / 2 - border / 2) || j < border / 2 || j >(h / 2 - border / 2))
			{
				pic[w*h + w / 2 * h / 2 + j * w / 2 + k] = 128;
				//pic[j*w+k]=255;
			}
		}
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
	int state = yuv420_border("video/akiyo.yuv", 352, 288);
	return 0;
}