/**
 * @file 9 yuv_clip.cpp
 * @author luohen
 * @brief yuv image clip
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
 * @brief
 *
 * @param w width of input yuv420p file
 * @param h height of input yuv420p file
 * @param sx clipped initial position x of origin y image
 * @param sy clipped initial position y of origin y image
 * @param sw wdith of clipped image
 * @param sh height of clipped image
 * @param url location of input yuv420p file
 * @return int
 */
int yuv420_clip(int w, int h, int sx, int sy, int sw, int sh, const char *url)
{
	//reading yuv file
	FILE *input_fp;
	//writing yuv file
	FILE *output_fp = fopen("video_result/output_clip.yuv", "wb+");

	if ((input_fp = fopen(url, "rb")) == NULL)
	{
		printf("%s open error!\n", url);
		return -1;
	}
	else
	{
		printf("%s open.\n", url);
	}

	//origin image
	unsigned char *pic = new unsigned char[w * h * 3 / 2];
	//clipped image
	unsigned char *pic_clip = new unsigned char[sw * sh * 3 / 2];

	// y length of origin image
	int size_y = w * h;
	// yu length of origin image
	int size_yu = w * h + w * h / 4;
	// y length of clipped image
	int size_sy = sw * sh;
	// yu length of clipped image
	int size_syu = sw * sh + sw * sh / 4;

	fread(pic, sizeof(unsigned char), w * h * 3 / 2, input_fp);

	//y clip
	for (int j = 0; j < sh; j++)
	{
		for (int k = 0; k < sw; k++)
		{
			pic_clip[j * sw + k] = pic[(sx + j) * w + (sy + k)];
		}
	}

	//sw_uv,sh_uv
	int sw_uv = sw / 2;
	int sh_uv = sh / 2;

	//u clip
	for (int j = 0; j < sh_uv; j++)
	{
		for (int k = 0; k < sw_uv; k++)
		{
			pic_clip[size_sy + j * sw_uv + k] = pic[size_y + (sx / 2 + j) * w / 2 + (sy / 2 + k)];
		}
	}

	//v clip
	for (int j = 0; j < sh_uv; j++)
	{
		for (int k = 0; k < sw_uv; k++)
		{
			pic_clip[size_syu + j * sw_uv + k] = pic[size_yu + (sx / 2 + j) * w / 2 + (sy / 2 + k)];
		}
	}

	fwrite(pic_clip, 1, sw * sh * 3 / 2, output_fp);

	delete[] pic;
	delete[] pic_clip;
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
	int state = yuv420_clip(352, 288, 60, 50, 176, 144, "video/akiyo.yuv");
	return 0;
}