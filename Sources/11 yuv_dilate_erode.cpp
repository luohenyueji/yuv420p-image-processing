/**
 * @file 11 yuv_dilate_erode.cpp
 * @author luohen
 * @brief dilate and erode of yuv image
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
 * @brief erode
 * 
 * @param pFrame		the input binary image
 * @param pdilateResult	the output binary image
 * @param kernel		kernel of erode
 * @param w				width of image
 * @param h				height of image
 */
void yuv420_erode(unsigned char *pFrame, unsigned char *pErodeResult, int kernel, int w, int h)
{
	//copy data
	unsigned char *srcImg = new unsigned char[w * h];
	memcpy((char *)srcImg, (char *)pFrame, w * h);

	//kernel除中心之外像素点个数
	int nErodeThreshold = kernel * kernel - 1;
	//对于中点要erode像素区域步长
	int erodeDist = (kernel - 1) / 2;
	//ignore edge point
	for (int i = erodeDist; i < h - erodeDist; i++)
	{
		for (int j = erodeDist; j < w - erodeDist; j++)
		{
			// 如果值不为0才进行处理
			if (srcImg[i * w + j] != 0)
			{
				int iPointCount = 0;
				// 根据此点的邻域判断此点是否需要删除
				for (int r = -erodeDist; r <= erodeDist; r++)
				{
					for (int c = -erodeDist; c <= erodeDist; c++)
					{
						//统计不为0的个数
						if (srcImg[(i + r) * w + j + c] != 0)
						{
							iPointCount++;
						}
					}
				}
				// 如果邻域中不为0的个数小于阈值，则中心点像素值设置为0
				if (iPointCount < nErodeThreshold)
				{
					pErodeResult[i * w + j] = 0;
				}
				else
				{
					pErodeResult[i * w + j] = 255;
				}
			}
			else
			{
				pErodeResult[i * w + j] = 0;
			}
		}
	}
	delete[] srcImg;
	return;
}

/**
 * @brief dilate
 * 
 * @param pFrame		the input binary image
 * @param pdilateResult	the output binary image
 * @param kernel		kernel of dilate
 * @param w				width of image
 * @param h				height of image
 */
void yuv420_dilate(unsigned char *pFrame, unsigned char *pdilateResult, int kernel, int w, int h)
{
	//copy data
	unsigned char *srcImg = new unsigned char[w * h];
	memcpy((char *)srcImg, (char *)pFrame, w * h);

	//对于中点要erode像素区域步长
	int erodeDist = (kernel - 1) / 2;
	//ignore edge point
	for (int i = erodeDist; i < h - erodeDist; i++)
	{
		for (int j = erodeDist; j < w - erodeDist; j++)
		{
			//对所有点进行判断
			int iPointCount = 0;
			// 根据此点的邻域判断此点是否需要删除
			for (int r = -erodeDist; r <= erodeDist; r++)
			{
				for (int c = -erodeDist; c <= erodeDist; c++)
				{
					//统计不为0的个数
					if (srcImg[(i + r) * w + j + c] != 0)
					{
						iPointCount++;
					}
				}
			}
			// 如果邻域中各像素点值都为0，则中心点像素值设置为0
			if (iPointCount == 0)
			{
				pdilateResult[i * w + j] = 0;
			}
			else
			{
				pdilateResult[i * w + j] = 255;
			}
		}
	}
	delete[] srcImg;
	return;
}

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
	;
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
	FILE *fp3 = fopen("video_result/akiyo_erode.y", "wb+");

	const char *url = "video/akiyo.yuv";
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

	//kernel size is 3 X 3.
	yuv420_erode(pResult, pResult, 3, w, h);
	//yuv420_dilate(pResult, pResult, 3, w, h);

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