#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void brightness1() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst = src + 100;  // 원본 영상의 모든 픽셀값을 100 증가

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void brightness2() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst(src.rows, src.cols, src.type());

	for (int j = 0; j < src.rows; ++j) {
		for (int i = 0; i < src.cols; ++i) {
			int v = src.at<uchar>(j, i) + 100;
			//dst.at<uchar>(j, i) = v > 255 ? 255 : v < 0 ? 0 : v;
			dst.at<uchar>(j, i) = saturate_cast<uchar>(v);
		}
	}
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

// 트랙바 콜백함수
void on_brightness(int pos, void* userdata) {
	Mat img = *(Mat*)userdata + pos;
	imshow("dst", img);
}
void brightness3() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	namedWindow("dst");
	// 트랙바 생성
	createTrackbar("Brightness", "dst", 0, 100, on_brightness, (void*)&src);

	imshow("dst", src);
	waitKey();
	destroyAllWindows();
}

int main(void) {
	// brightness1();
	// brightness2();
	brightness3();
	return 0;
}