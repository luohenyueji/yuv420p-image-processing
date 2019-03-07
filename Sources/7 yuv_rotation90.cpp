/**
 * @file 7 yuv_Rotation90.cpp
 * @author luohen
 * @brief 90 degree rotation of yuv420 images
 * @date 2018-12-08
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
 * @brief Pre-defined image size
 *
 */
#define image_h 288
#define image_w 352

/**
  * @brief
  *
  * @param url location of input yuv420p file
  * @return int
  */
int yuv420_Rotation90(const char *url)
{
	//reading yuv files
	FILE *input_fp;
	//writingyuv files
	FILE *output_fp = fopen("video_result/output_rotation.yuv", "wb+");

	//reading yuv datas
	if ((input_fp = fopen(url, "rb")) == NULL)
	{
		printf("%s open error!\n", url);
		return -1;
	}
	else
	{
		printf("%s open.\n", url);
	}

	//Input image array definition
	unsigned char input_Y[image_h][image_w];
	unsigned char input_U[image_h / 2][image_w / 2];
	unsigned char input_V[image_h / 2][image_w / 2];

	//Output image array definition
	unsigned char output_Y[image_w][image_h];
	unsigned char output_U[image_w / 2][image_h / 2];
	unsigned char output_V[image_w / 2][image_h / 2];

	int w = image_w;
	int h = image_h;

	fread(input_Y, sizeof(unsigned char), w * h, input_fp);
	fread(input_U, sizeof(unsigned char), w / 2 * h / 2, input_fp);
	fread(input_V, sizeof(unsigned char), w / 2 * h / 2, input_fp);

	//Y 90 degree rotation
	for (int x = 0; x < h; x++)
	{
		for (int y = 0; y < w; y++)
		{
			//旋转之后，输出的x值等于输入的y坐标值
			//y值等于输入列高-输入x坐标值-1
			output_Y[y][h - x - 1] = input_Y[x][y];
		}
	}

	//u 90 degree rotation
	for (int x = 0; x < h / 2; x++)
	{
		for (int y = 0; y < w / 2; y++)
		{
			//旋转之后，输出的x值等于输入的y坐标值
			//y值等于输入列高-输入x坐标值-1
			output_U[y][h / 2 - x - 1] = input_U[x][y];
		}
	}

	//v 90 degree rotation
	for (int x = 0; x < h / 2; x++)
	{
		for (int y = 0; y < w / 2; y++)
		{
			//旋转之后，输出的x值等于输入的y坐标值
			//y值等于输入列高-输入x坐标值-1
			output_V[y][h / 2 - x - 1] = input_V[x][y];
		}
	}

	fwrite(output_Y, sizeof(unsigned char), w * h, output_fp);
	fwrite(output_U, sizeof(unsigned char), w / 2 * h / 2, output_fp);
	fwrite(output_V, sizeof(unsigned char), w / 2 * h / 2, output_fp);

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
	int state = yuv420_Rotation90("video/akiyo.yuv");
	return 0;
}