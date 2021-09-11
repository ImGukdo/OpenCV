#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// 전역 이진화
void on_threshold(int pos, void* userData) {
	Mat src = *(Mat*)userData;

	Mat dst;
	// 트랙바의 위치를 받아서 이진화 수행
	threshold(src, dst, pos, 255, THRESH_BINARY);
	imshow("dst", dst);
}

void threshold_binary() {
	Mat src = imread("./images/neutrophils.png", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);

	namedWindow("dst");  
	// 트랙바 생성
	createTrackbar("Threshold", "dst", 0, 255, on_threshold, (void*)&src);
	// 처음 실행될때 트랙바 위치 설정하여 함수가 한번 실행 되도록 함
	setTrackbarPos("Threshold", "dst", 128);  

	waitKey();
	destroyAllWindows();
}

// 적응형 이진화
void on_trackbar(int pos, void* userdata) {
	Mat src = *(Mat*)userdata;

	int bsize = pos;
	// 블록사이즈가 짝수이면 1을 빼서 홀수로 만든다
	if (bsize % 2 == 0)
		--bsize;
	// 블록사이즈가 3보다 작으면 최소값 3으로 설정
	if (bsize < 3)
		bsize = 3;
	// 트랙바로 설정한 블록크기로 적응형 이진화 수행, 가우시안 가중평균 사용, 평균에서 5를 뺀값을 임계값으로 사용
	Mat dst;
	adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, 5);

	imshow("dst", dst);
}

void adapThreshold() {
	Mat src = imread("./images/sudoku.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);

	namedWindow("dst");
	createTrackbar("BlockSize", "dst", 0, 200, on_trackbar, (void*)&src);
	setTrackbarPos("BlockSize", "dst", 11);

	waitKey();
	destroyAllWindows();
}


// 이진 영상의 침식과 팽창
void erode_dilate() {
	Mat src = imread("./images/milkdrop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// 오츠 알고리즘으로 자동 이진화
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat dst1, dst2;
	erode(bin, dst1, Mat());  // 3X3 정방형 구조요소를 이용하여 침식 연산 수행
	dilate(bin, dst2, Mat()); // 3X3 정방형 구조요소를 이용하여 팽창 연산 수행

	imshow("src", src);
	imshow("bin", bin);
	imshow("erode", dst1);
	imshow("dilate", dst2);

	waitKey();
	destroyAllWindows();
}


// 이진영상의 열기와 닫기 연산
void open_close() {
	Mat src = imread("./images/milkdrop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// 오츠 알고리즘으로 자동 이진화를 수행
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// 3X3 정방형 구조 요소를 이용하여 열기연산과 닫기연산 수행
	Mat dst1, dst2;
	morphologyEx(bin, dst1, MORPH_OPEN, Mat());
	morphologyEx(bin, dst2, MORPH_CLOSE, Mat());

	imshow("src", src);
	imshow("bin", bin);
	imshow("opening", dst1);
	imshow("closing", dst2);

	waitKey();
	destroyAllWindows();
}



int main(void) {
	// threshold_binary();
	// adapThreshold();
	// erode_dilate();
	open_close();
	return 0;
}