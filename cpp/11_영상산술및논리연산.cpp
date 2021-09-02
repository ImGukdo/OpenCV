#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


void addSubtract() {
	Mat src1 = imread("./images/lenna256.bmp", IMREAD_GRAYSCALE);
	Mat src2 = imread("./images/square.bmp", IMREAD_GRAYSCALE);

	if (src1.empty() || src2.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src1", src1);
	imshow("scr2", src2);

	Mat dst1, dst2, dst3, dst4;

	add(src1, src2, dst1);  // 두 영상의 덧셈 연산
	addWeighted(src1, 0.5, src2, 0.5, 0, dst2);  // 두 영상의 가중치 합
	subtract(src1, src2, dst3);  // 두 영상의 뺄셈 연산
	absdiff(src1, src2, dst4);  // 두 영상의 차이 연산

	imshow("dst1", dst1);
	imshow("dst2", dst2);
	imshow("dst3", dst3);
	imshow("dst4", dst4);

	waitKey();
	destroyAllWindows();
}

// 비트연산자 또한 연산자 재정의가 되어있어 함수없이 사용 가능
void bitwize() {
	Mat src1 = imread("./images/lenna256.bmp", IMREAD_GRAYSCALE);
	Mat src2 = imread("./images/square.bmp", IMREAD_GRAYSCALE);

	if (src1.empty() || src2.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src1", src1);
	imshow("src2", src2);

	Mat dst1, dst2, dst3, dst4;

	bitwise_and(src1, src2, dst1);  // 논리곱 연산
	bitwise_or(src1, src2, dst2);  // 논리합 연산
	bitwise_xor(src1, src2, dst3);  // 배타적 논리합 연산
	bitwise_not(src1, dst4);  // 부정 연산

	imshow("dst1", dst1);
	imshow("dst2", dst2);
	imshow("dst3", dst3);
	imshow("dst4", dst4);

	waitKey();
	destroyAllWindows();
}

int main(void) {
	// addSubtract();
	bitwize();

	return 0;
}