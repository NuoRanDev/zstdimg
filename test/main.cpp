#define _CRT_SECURE_NO_WARNINGS
#include "opencv4/opencv2/opencv.hpp"

#include "zstdImgWrite.h"
#include "zstdImgInit.h"
#include "zstdImgRead.h"

#include <stdio.h>

int main()
{
	/*
	Zimg zimg;
	cv::Mat pimg = cv::imread("C:/Users/root/Desktop/max_big.png");

	zimg = zLoadImgData(pimg.cols, pimg.rows, pimg.channels(), 1, pimg.data);
	pimg.release();

	FILE* zwfs = fopen("C:/Users/root/Desktop/max_big.zimg", "w");

	zWriteImgToCFile(zimg, zwfs);

	zReleaseImgData(zimg);
	fclose(zwfs);*/

	FILE* zrfs = fopen("C:/Users/root/Desktop/max_big.zimg", "r");
	Zimg nzimg;
	if (zReadImgFromCFile(zrfs, &nzimg))exit(-1);
	fclose(zrfs);


	cv::Mat cvnzimg = cv::Mat(cv::Size(nzimg.x, nzimg.y), CV_8UC4, nzimg.data);
	cv::imshow("zimg", cvnzimg);
	while(1)
	{
		if (cv::waitKey(100) == 'a');
		{
			break;
		}
	}
	zReleaseImgData(nzimg);

	return 0;
}