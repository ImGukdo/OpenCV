#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	// 이미지 불러오기
	Mat src = imread("./images/lenna.bmp");
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return -1;
	}
	imshow("src", src);

	// 작업할 이미지 복사
	Mat src_copy = src.clone();

	int num_down = 1;  // 다운캐스팅할 횟수
	int num_bilateral = 5;  // 양방향 필터 횟수
	
	// 다운캐스팅
	for (int i = 0; i < num_down; ++i) {
		pyrDown(src_copy, src_copy);
	}

	// 양방향 필터
	Mat dst;
	for (int i = 0; i < num_bilateral; ++i) {
		bilateralFilter(src_copy, dst, -1, 9, 7);
		src_copy = dst.clone();
	}

	// 업캐스팅
	for (int i = 0; i < num_down; ++i) {
		pyrUp(src_copy, src_copy);
	}

	// 그레이스케일로 변환, 미디언필터
	Mat blur, gray;
	cvtColor(src_copy, gray, COLOR_BGR2GRAY);
	medianBlur(gray, blur, 5);

	// 적응형 이진화, BGR 변환
	Mat edge;
	adaptiveThreshold(blur, edge, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 1.5);
	cvtColor(edge, edge, COLOR_GRAY2BGR);

	// and연산을 통해 만화그림 생성
	Mat cartoon;
	bitwise_and(src_copy, edge, cartoon);

	imshow("cartoon", cartoon);

	waitKey();
	destroyAllWindows();

	return 0;
}