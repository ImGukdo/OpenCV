#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	// �̹��� �ҷ�����
	Mat src = imread("./images/lenna.bmp");
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return -1;
	}
	imshow("src", src);

	// �۾��� �̹��� ����
	Mat src_copy = src.clone();

	int num_down = 1;  // �ٿ�ĳ������ Ƚ��
	int num_bilateral = 5;  // ����� ���� Ƚ��
	
	// �ٿ�ĳ����
	for (int i = 0; i < num_down; ++i) {
		pyrDown(src_copy, src_copy);
	}

	// ����� ����
	Mat dst;
	for (int i = 0; i < num_bilateral; ++i) {
		bilateralFilter(src_copy, dst, -1, 9, 7);
		src_copy = dst.clone();
	}

	// ��ĳ����
	for (int i = 0; i < num_down; ++i) {
		pyrUp(src_copy, src_copy);
	}

	// �׷��̽����Ϸ� ��ȯ, �̵������
	Mat blur, gray;
	cvtColor(src_copy, gray, COLOR_BGR2GRAY);
	medianBlur(gray, blur, 5);

	// ������ ����ȭ, BGR ��ȯ
	Mat edge;
	adaptiveThreshold(blur, edge, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 1.5);
	cvtColor(edge, edge, COLOR_GRAY2BGR);

	// and������ ���� ��ȭ�׸� ����
	Mat cartoon;
	bitwise_and(src_copy, edge, cartoon);

	imshow("cartoon", cartoon);

	waitKey();
	destroyAllWindows();

	return 0;
}