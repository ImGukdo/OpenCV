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
	// ����ũ�� �����İ� ũ�Ⱑ ���ƾ��Ѵ�.
	// ����ũ�� 0�� �ƴѺκп��� value���� �����ȴ�.
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
	// mask ���󿡼� 0�� �ƴ� �κп����� scr ���� �ȼ����� dst�� ����
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
	tm.start();  // �ð� ���� ����

	// ������ ��� �ȼ��� ����
	for (int j = 0; j < src.rows; ++j) {
		for (int i = 0; i < src.cols; ++i) {
			dst.at<uchar>(j, i) = 255 - src.at<uchar>(j, i);
		}
	}

	tm.stop();  // �ð� ���� ����
	cout << "Image inverse took " << tm.getTimeMilli() << "ms" << endl;
}

void func() {
	Mat img = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	cout << "sum : " << (int)sum(img)[0] << endl;  // �� ä���� ��
	cout << "mean : " << (int)mean(img)[0] << endl;  // �� ä���� ���

	// ���� ä�� ���� ����, �ּڰ��� ��ġ, �ִ񰪰� ��ġ ���ϱ�
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);
	cout << "minVal : " << minVal << " at " << minLoc << endl;
	cout << "maxVal : " << maxVal << " at " << maxLoc << endl;

	// �ݿø�, �Ҽ��� �Ʒ��� 0.5�� ��� ����� ¦���� �ݿø� ����
	cout << "cvRound(2.5) : " << cvRound(2.5) << endl;
	cout << "cvRound(2.51) : " << cvRound(2.51) << endl;
	cout << "cvRound(3.4999) : " << cvRound(3.4999) << endl;
	cout << "cvRound(3.5) : " << cvRound(3.5) << endl;
}

int main(void) {
	// mask_setTo();
	// mask_copyTo();
	// time_inverse();
	func();
	return 0;
}