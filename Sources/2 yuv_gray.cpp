/**
 * @file 2 yuv_gray.cpp
 * @author luohen
 * @brief gray scale of yuv
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
int yuv420_gray(const char *url, int w, int h)
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
	FILE *outputGray_fp = fopen("video_result/output_gray.yuv", "wb+");

	unsigned char *pic = new unsigned char[w * h * 3 / 2];

	fread(pic, sizeof(unsigned char), w * h * 3 / 2, input_fp);
	//Gray
	//把pic+w*h开始所有的数据置为128，色度分量取值范围是-128至127，量化后范围为0至255
	//uv=128，实现灰度化
	memset(pic + w * h, 128, w * h / 2);
	fwrite(pic, sizeof(unsigned char), w * h * 3 / 2, outputGray_fp);

	delete[] pic;
	fclose(input_fp);
	fclose(outputGray_fp);
	return 0;
}

/**
 * @brief main函数
 *
 * @return int
 */
int main()
{
	int state = yuv420_gray("video/akiyo.yuv", 352, 288);
	return 0;
}