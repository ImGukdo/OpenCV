#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// ���� ����ȭ
void on_threshold(int pos, void* userData) {
	Mat src = *(Mat*)userData;

	Mat dst;
	// Ʈ������ ��ġ�� �޾Ƽ� ����ȭ ����
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
	// Ʈ���� ����
	createTrackbar("Threshold", "dst", 0, 255, on_threshold, (void*)&src);
	// ó�� ����ɶ� Ʈ���� ��ġ �����Ͽ� �Լ��� �ѹ� ���� �ǵ��� ��
	setTrackbarPos("Threshold", "dst", 128);  

	waitKey();
	destroyAllWindows();
}

// ������ ����ȭ
void on_trackbar(int pos, void* userdata) {
	Mat src = *(Mat*)userdata;

	int bsize = pos;
	// ��ϻ���� ¦���̸� 1�� ���� Ȧ���� �����
	if (bsize % 2 == 0)
		--bsize;
	// ��ϻ���� 3���� ������ �ּҰ� 3���� ����
	if (bsize < 3)
		bsize = 3;
	// Ʈ���ٷ� ������ ���ũ��� ������ ����ȭ ����, ����þ� ������� ���, ��տ��� 5�� ������ �Ӱ谪���� ���
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


// ���� ������ ħ�İ� ��â
void erode_dilate() {
	Mat src = imread("./images/milkdrop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ���� �˰������� �ڵ� ����ȭ
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	Mat dst1, dst2;
	erode(bin, dst1, Mat());  // 3X3 ������ ������Ҹ� �̿��Ͽ� ħ�� ���� ����
	dilate(bin, dst2, Mat()); // 3X3 ������ ������Ҹ� �̿��Ͽ� ��â ���� ����

	imshow("src", src);
	imshow("bin", bin);
	imshow("erode", dst1);
	imshow("dilate", dst2);

	waitKey();
	destroyAllWindows();
}


// ���������� ����� �ݱ� ����
void open_close() {
	Mat src = imread("./images/milkdrop.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ���� �˰������� �ڵ� ����ȭ�� ����
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);

	// 3X3 ������ ���� ��Ҹ� �̿��Ͽ� ���⿬��� �ݱ⿬�� ����
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