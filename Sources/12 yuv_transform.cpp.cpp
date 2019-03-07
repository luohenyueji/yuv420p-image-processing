/**
 * @file 12 yuv_transform.cpp
 * @author luohen
 * @brief YUV image transform to opencv rgb image
 * @date 2018-12-11
 * 
 */

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>

extern "C"
{
#include "include\libavcodec\avcodec.h"
#include "include\libavformat\avformat.h"
#include "include\libavutil\channel_layout.h"
#include "include\libavutil\common.h"
#include "include\libavutil\imgutils.h"
#include "include\libswscale\swscale.h"
#include "include\libavutil\imgutils.h"
#include "include\libavutil\opt.h"
#include "include\libavutil\mathematics.h"
#include "include\libavutil\samplefmt.h"
//libyuv
#include "include\libyuv\libyuv.h"
};
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "postproc.lib")
#pragma comment(lib, "swresample.lib")
#pragma comment(lib, "swscale.lib")
//libyuv
#pragma comment(lib, "yuv.lib")

using namespace std;
using namespace cv;

/**
 * @brief
 *
 * @param pYUV		input yuv420 image
 * @param pBGR24 	output bgr24 image
 * @param width		width of input yuv420p image
 * @param height	height of input yuv420p image
 * @return
 */
bool ffmpeg_yuv2bgr(unsigned char *pYUV, unsigned char *pBGR24, int width, int height)
{
	AVPicture pFrameYUV, pFrameBGR;

	avpicture_fill(&pFrameYUV, pYUV, AV_PIX_FMT_YUV420P, width, height);
	avpicture_fill(&pFrameBGR, pBGR24, AV_PIX_FMT_BGR24, width, height);

	struct SwsContext *imgCtx = NULL;
	//初始化函数
	//原图高，宽，图像类型；输出图高，宽，图像类型；算法种类；其他
	imgCtx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_BGR24, SWS_BILINEAR, 0, 0, 0);

	if (imgCtx != NULL)
	{
		//执行函数
		//函数返回值；输入图像指针数组，图像颜色通道数组；扫描起点；扫描行数；输出图像指针数组，图像颜色通道数组；
		sws_scale(imgCtx, pFrameYUV.data, pFrameYUV.linesize, 0, height, pFrameBGR.data, pFrameBGR.linesize);
		//end
		if (imgCtx)
		{
			sws_freeContext(imgCtx);
			imgCtx = NULL;
		}
		return true;
	}
	else
	{
		sws_freeContext(imgCtx);
		imgCtx = NULL;
		return false;
	}
}

/**
 * @brief		transform function of ffmpeg
 *
 * @param w		width of input yuv420p image
 * @param h		height of input yuv420p image
 * @param pic	input yuv image
 * @return Mat	output rgb image(opencv mat)
 */
Mat yuv420_ffmpeg(int w, int h, unsigned char *pic)
{
	Mat bgrImg(h, w, CV_8UC3);
	unsigned char *pBGR24 = new unsigned char[w * h * 3];
	ffmpeg_yuv2bgr(pic, bgrImg.data, w, h);

	return bgrImg;
}

/**
 * @brief		transform function of libyuv
 *
 * @param w		width of input yuv420p image
 * @param h		height of input yuv420p image
 * @param pic	input yuv image
 * @return Mat	output rgb image(opencv mat)
 */
Mat yuv420_libyuv(int w, int h, unsigned char *pic)
{
	int size_src = w * h * 3 / 2;
	int size_dest = w * h * 4;

	//BGRA, A:Alpha(transparency，透明度)
	Mat matI420 = cv::Mat(h, w, CV_8UC4);

	libyuv::I420ToARGB((const uint8 *)pic, w,
					   (const uint8 *)(pic + w * h), w / 2,
					   (const uint8 *)(pic + w * h * 5 / 4), w / 2,
					   matI420.data, w * 4, w, h);
	//bgr
	Mat bgrImg;
	cvtColor(matI420, bgrImg, COLOR_BGRA2BGR);
	return bgrImg;
}

/**
 * @brief
 *
 * @param w
 * @param h
 * @param pic
 * @return Mat
 */
Mat yuv420_opencv(int w, int h, unsigned char *pic)
{
	//创建YUV mat
	cv::Mat yuvImg;
	yuvImg.create(h * 3 / 2, w, CV_8UC1);
	//数据保存为yuvImg.data
	memcpy(yuvImg.data, pic, w * h * 3 / 2 * sizeof(unsigned char));

	//转化为RGB图像
	cv::Mat bgrImg;
	cv::cvtColor(yuvImg, bgrImg, CV_YUV2BGR_I420);

	return bgrImg;
}

/**
 * @brief main
 *
 * @return int
 */
int main()
{
	clock_t start, end;
	double endtime;
	//Frequency of reading image
	int count_frame = 300;
	//视频路径
	char *url = (char *)"video/akiyo.yuv";
	int w = 352, h = 288;
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

	unsigned char *pYuvBuf = new unsigned char[w * h * 3 / 2];

	fseek(input_fp, 0, SEEK_SET);
	//Timing starts
	start = clock();
	Mat ffmpeg_mat;
	for (int i = 0; i < count_frame; i++)
	{
		fread(pYuvBuf, sizeof(unsigned char), w * h * 3 / 2, input_fp);
		ffmpeg_mat = yuv420_ffmpeg(w, h, pYuvBuf);
	}

	//Timing end
	end = clock();
	endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "ffmpeg Total time:" << endtime << "s" << endl;
	cout << "ffmpeg Total time:" << endtime * 1000 << "ms" << endl;

	fseek(input_fp, 0, SEEK_SET);
	start = clock();
	Mat libyuv_mat;
	for (int i = 0; i < count_frame; i++)
	{
		fread(pYuvBuf, sizeof(unsigned char), w * h * 3 / 2, input_fp);
		libyuv_mat = yuv420_libyuv(w, h, pYuvBuf);
	}
	end = clock();
	endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "libyuv Total time:" << endtime << "s" << endl;			//s为单位
	cout << "libyuv Total time:" << endtime * 1000 << "ms" << endl; //ms为单位

	fseek(input_fp, 0, SEEK_SET);
	start = clock();
	Mat opencv_mat;
	for (int i = 0; i < count_frame; i++)
	{
		fread(pYuvBuf, sizeof(unsigned char), w * h * 3 / 2, input_fp);
		opencv_mat = yuv420_opencv(w, h, pYuvBuf);
	}
	end = clock();
	endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "opencv Total time:" << endtime << "s" << endl;
	cout << "opencv Total time:" << endtime * 1000 << "ms" << endl;

	system("pause");
	return 0;
}