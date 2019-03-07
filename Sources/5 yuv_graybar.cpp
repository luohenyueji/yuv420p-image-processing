/**
 * @file 5 yuv_graybar.cpp
 * @author luohen
 * @brief gray scale bar of yuv
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
 * @param width		width of input yuv420p file
 * @param height	height of input yuv420p file
 * @param ymin		minimum value of y
 * @param ymax		maximum value of y
 * @param barnum 	Number of bars
 * @param url		location of input yuv420p file
 * @return int
 */
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, const char *url)
{
	//每个灰度条的宽度
	int barwidth;
	//每个灰度阶次范围
	float lum_inc;
	//计算Y值
	unsigned char lum_temp;
	//uv分量宽高
	int uv_width, uv_height;
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
	FILE *output_fp = fopen("video_result/gray_test.yuv", "wb+");

	int t = 0, i = 0, j = 0;

	//每个灰度条的宽度
	barwidth = width / barnum;
	//每个灰度阶次范围
	lum_inc = ((float)(ymax - ymin)) / ((float)(barnum - 1));
	//uv分量宽高
	uv_width = width / 2;
	uv_height = height / 2;

	unsigned char *data_y = new unsigned char[width * height];
	unsigned char *data_u = new unsigned char[uv_width * uv_height];
	unsigned char *data_v = new unsigned char[uv_width * uv_height];

	//Output Info
	//输出信息
	printf("Y, U, V value from picture's left to right:\n");
	for (t = 0; t < (width / barwidth); t++)
	{
		//计算Y值
		lum_temp = ymin + (char)(t * lum_inc);
		printf("%3d, 128, 128\n", lum_temp);
	}
	//保存数据
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			t = i / barwidth;
			lum_temp = ymin + (char)(t * lum_inc);
			data_y[j * width + i] = lum_temp;
		}
	}
	for (j = 0; j < uv_height; j++)
	{
		for (i = 0; i < uv_width; i++)
		{
			data_u[j * uv_width + i] = 128;
		}
	}
	for (j = 0; j < uv_height; j++)
	{
		for (i = 0; i < uv_width; i++)
		{
			data_v[j * uv_width + i] = 128;
		}
	}

	fwrite(data_y, width * height, sizeof(unsigned char), output_fp);
	fwrite(data_u, uv_width * uv_height, sizeof(unsigned char), output_fp);
	fwrite(data_v, uv_width * uv_height, sizeof(unsigned char), output_fp);
	fclose(input_fp);
	fclose(output_fp);

	delete[] data_y;
	delete[] data_u;
	delete[] data_v;
	return 0;
}

/**
 * @brief main
 *
 * @return int
 */
int main()
{
	int state = yuv420_graybar(640, 360, 0, 255, 10, "video/graybar.yuv");
	return 0;
}