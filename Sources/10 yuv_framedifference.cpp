/**
 * @file 10 yuv_framedifference.cpp
 * @author luohen
 * @brief Frame difference method of y
 * @date 2018-12-10
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

/**
 * @brief
 *
 * @param pFrame1		the first frame
 * @param pFrame2		the second frame
 * @param pResult		the result image
 * @param w				width of input yuv420p file
 * @param h				height of input yuv420p file
 * @param yThreshold	threshold value
 */
void yuv420_Framedifference(unsigned char *pFrame1, unsigned char *pFrame2, unsigned char *pResult, int w, int h, int yThreshold)
{
	//the first frame
	unsigned char *Y1 = new unsigned char[w * h];
	//the second frame
	unsigned char *Y2 = new unsigned char[w * h];

	//copy y
	memcpy(Y1, pFrame1, w * h);
	memcpy(Y2, pFrame2, w * h);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = y * w + x;
			//diff
			int temp = abs((int)Y1[i] - (int)Y2[i]);
			if (temp > yThreshold)
			{
				pResult[i] = 255;
			}
			else
			{
				pResult[i] = 0;
			}
		}
	}
	delete[] Y1;
	delete[] Y2;
}

/**
 * @brief main function
 *
 * @return int
 */
int main()
{
	//the yuv image size
	int w = 352, h = 288;

	//reading yuv file
	FILE *input_fp;
	//writing yuv file
	//the first yuv image
	FILE *fp1 = fopen("video_result/akiyo1.yuv", "wb+");
	//the second yuv image
	FILE *fp2 = fopen("video_result/akiyo2.yuv", "wb+");
	//the binary image of frame difference
	FILE *fp3 = fopen("video_result/output_diff.y", "wb+");

	const char * url = "video/akiyo.yuv";
	if ((input_fp = fopen(url, "rb")) == NULL)
	{
		printf("%s open error!\n", url);
		return 0;
	}
	else
	{
		printf("%s open.\n", url);
	}

	//result
	unsigned char *pResult = new unsigned char[w * h];
	//the first image
	unsigned char *pFrame1 = new unsigned char[w * h * 3 / 2];
	//the second image
	unsigned char *pFrame2 = new unsigned char[w * h * 3 / 2];

	//used for read frames
	unsigned char originalFrame[352 * 288 * 3 / 2];
	//reading image for a loop
	for (int i = 0; i < 200; i++)
	{
		//fread function automatically moves the pointer
		//take the first frame
		if (i == 0)
		{
			fread(pFrame1, w * h * 3 / 2, 1, input_fp);
		}
		//take the second frame
		if (i == 199)
		{
			fread(pFrame2, w * h * 3 / 2, 1, input_fp);
		}
		//Skip intermediate frame
		else
		{
			fread(originalFrame, w * h * 3 / 2, 1, input_fp);
		}
	}

	/* another way to read images
	fread(pFrame1, w * h * 3 / 2, 1, input_fp);
	int p = 199 * w*h * 3 / 2;
	//move the pointer
	fseek(input_fp, p, SEEK_SET);
	fread(pFrame2, w * h * 3 / 2, 1, input_fp);
	*/

	//the threshold is 30
	yuv420_Framedifference(pFrame1, pFrame2, pResult, w, h, 30);

	fwrite(pFrame1, 1, w * h * 3 / 2, fp1);
	fwrite(pFrame2, 1, w * h * 3 / 2, fp2);
	fwrite(pResult, 1, w * h, fp3);

	delete[] pResult;
	delete[] pFrame1;
	delete[] pFrame2;

	fclose(input_fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}