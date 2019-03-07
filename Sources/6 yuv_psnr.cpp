/**
 * @file 6 yuv420_psnr.cpp
 * @author luohen
 * @brief Compute the PSNR values of two yuv files
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
 * @param url1	location of input yuv420p file1
 * @param url2	location of input yuv420p file2
 * @param w		width of input yuv420p file
 * @param h		height of input yuv420p file
 * @return int
 */
int yuv420_psnr(const char *url1, const char *url2, int w, int h)
{
	//reading yuv iamges
	FILE *fp1 = fopen(url1, "rb+");
	FILE *fp2 = fopen(url2, "rb+");

	unsigned char *pic1 = new unsigned char[w * h];
	unsigned char *pic2 = new unsigned char[w * h];

	fread(pic1, 1, w * h, fp1);
	fread(pic2, 1, w * h, fp2);

	double mse_sum = 0, mse = 0, psnr = 0;
	//computing mse
	for (int j = 0; j < w * h; j++)
	{
		mse_sum += pow((double)(pic1[j] - pic2[j]), 2);
	}
	mse = mse_sum / (w * h);
	//computing psnr
	psnr = 10 * log10(255.0 * 255.0 / mse);
	printf("%5.3f\n", psnr);

	delete[] pic1;
	delete[] pic2;
	fclose(fp1);
	fclose(fp2);
	return 0;
}

/**
 * @brief main
 *
 * @return int
 */
int main()
{
	int state = yuv420_psnr("video/akiyo.yuv", "video/distort_akiyo.yuv", 352, 288);
	return 0;
}