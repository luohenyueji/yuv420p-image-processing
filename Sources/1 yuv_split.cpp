/**
 * @file	1 yuv_split.cpp
 * @author	luohen
 * @brief	split of yuv
 * @date	2018-12-07
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
int yuv420_split(const char *url, int w, int h)
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
	FILE *outputY_fp = fopen("video_result/output_420_y.y", "wb+");
	FILE *outputU_fp = fopen("video_result/output_420_u.y", "wb+");
	FILE *outputV_fp = fopen("video_result/output_420_v.y", "wb+");

	unsigned char *pic = new unsigned char[w * h * 3 / 2];

	//读数据，每次读取的字节数为sizeof(unsigned char)=1，共读取w*h*3/2次
	//reading data
	fread(pic, sizeof(unsigned char), w * h * 3 / 2, input_fp);
	//writing data
	//Y
	fwrite(pic, sizeof(unsigned char), w * h, outputY_fp);
	//U
	fwrite(pic + w * h, sizeof(unsigned char), w * h / 4, outputU_fp);
	//V
	fwrite(pic + w * h * 5 / 4, sizeof(unsigned char), w * h / 4, outputV_fp);

	//memory release and files closing
	delete[] pic;
	fclose(input_fp);
	fclose(outputY_fp);
	fclose(outputU_fp);
	fclose(outputV_fp);

	return 0;
}

/**
 * @brief main
 *
 * @return int
 */
int main()
{
	//Setting YUV information
	int state = yuv420_split("video/akiyo.yuv", 352, 288);
	return 0;
}