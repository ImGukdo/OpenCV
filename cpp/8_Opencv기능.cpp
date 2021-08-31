#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void mask_setTo() {
	Mat src = imread("./images/lenna.bmp", IMREAD_COLOR);
	Mat mask = imread("./images/mask_smile.bmp", IMREAD_GRAYSCALE);

	if (src.empty() || mask.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 마스크는 대상행렬과 크기가 같아야한다.
	// 마스크의 0이 아닌부분에만 value값이 설정된다.
	src.setTo(Scalar(0, 255, 255), mask);

	imshow("src", src);
	imshow("mask", mask);

	waitKey();
	destroyAllWindows();
}

void mask_copyTo() {
	Mat src = imread("./images/airplane.bmp", IMREAD_COLOR);
	Mat mask = imread("./images/mask_plane.bmp", IMREAD_GRAYSCALE);
	Mat dst = imread("./images/field.bmp", IMREAD_COLOR);

	if (src.empty() || mask.empty() || dst.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// mask 영상에서 0이 아닌 부분에서만 scr 영상 픽셀값을 dst로 복사
	src.copyTo(dst, mask);

	imshow("dst", dst);
	waitKey();
}

void time_inverse() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst(src.rows, src.cols, src.type());

	TickMeter tm; 
	tm.start();  // 시간 측정 시작

	// 영상의 모든 픽셀을 반전
	for (int j = 0; j < src.rows; ++j) {
		for (int i = 0; i < src.cols; ++i) {
			dst.at<uchar>(j, i) = 255 - src.at<uchar>(j, i);
		}
	}

	tm.stop();  // 시간 측정 종료
	cout << "Image inverse took " << tm.getTimeMilli() << "ms" << endl;
}

void func() {
	Mat img = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	cout << "sum : " << (int)sum(img)[0] << endl;  // 각 채널의 합
	cout << "mean : " << (int)mean(img)[0] << endl;  // 각 채널의 평균

	// 단일 채널 영상만 가능, 최솟값과 위치, 최댓값과 위치 구하기
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
	cout << "minVal : " << minVal << " at " << minLoc << endl;
	cout << "maxVal : " << maxVal << " at " << maxLoc << endl;

	// 반올림, 소수점 아래가 0.5인 경우 가까운 짝수로 반올림 수행
	cout << "cvRound(2.5) : " << cvRound(2.5) << endl;
	cout << "cvRound(2.51) : " << cvRound(2.51) << endl;
	cout << "cvRound(3.4999) : " << cvRound(3.4999) << endl;
	cout << "cvRound(3.5) : " << cvRound(3.5) << endl;
	
	// 컬러변환 함수
	Mat img2 = imread("./images/lenna.bmp", IMREAD_COLOR);
	Mat img3;
	cvtColor(img2, img3, COLOR_BGR2GRAY);  // 3채널 컬러영상을 그레이스케일로 변환
}

int main(void) {
	// mask_setTo();
	// mask_copyTo();
	// time_inverse();
	func();
	return 0;
}
