#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void filter_embossing() {
	Mat src = imread("./images/rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ������ ���� ����ũ ��� ����
	float data[] = { -1, -1, 0, -1, 0, 1, 0, 1, 1 };
	Mat emboss(3, 3, CV_32FC1, data);

	Mat dst;
	// filter2D �Լ��� �Է¿����� ���̿� ���� ������ ��¿����� ���̰� �����Ǿ� ����
	filter2D(src, dst, -1, emboss, Point(-1, -1), 128);

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}

void blurring_mean() {
	Mat src = imread("./images/rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);

	Mat dst;
	int count = 1;
	// ������ ũ�Ⱑ Ŭ���� ���귮�� �����ϰ� ���� �ε巯�� ������ ��� ������ ����
	for (int ksize = 3; ksize <= 7; ksize += 2) {
		blur(src, dst, Size(ksize, ksize));

		String desc = format("Mean: %dx%d", ksize, ksize);
		putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);

		String namedWindow = format("dst%d", count++);
		imshow(namedWindow, dst);
	}
	waitKey();
	destroyAllWindows();
}

void blurring_gaussian() {
	Mat src = imread("./images/rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);
	Mat dst;

	int count = 1;
	// �ñ׸��� ���� Ŭ���� ���� �ε巯�� ������ ��� ������ ����
	for (int sigma = 1; sigma <= 5; ++sigma) {
		// Ŀ�� ����� Size()�� �ϸ� ǥ�������κ��� ũ�⸦ �ڵ� ����
		// ysigma ���� ������ xsigma���� ���� ���� ���
		GaussianBlur(src, dst, Size(), (double)sigma);

		String text = format("sigma = %d", sigma);
		putText(dst, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
		String namedWindow = format("dst%d", count++);
		imshow(namedWindow, dst);
	}
	waitKey();
	destroyAllWindows();
}

void unsharp_mask() {
	Mat src = imread("./images/rose.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);
	Mat blurred, dst;
	int count = 1;

	for (int sigma = 1; sigma <= 5; ++sigma) {
		GaussianBlur(src, blurred, Size(), sigma);  // ���� ������ ����
		float alpha = 2.f;  // ����ġ��
	
		dst = (1 + alpha) * src - alpha * blurred;  // ����� ����ũ ���͸� ����

		String text = format("sigma : %d", sigma);
		String namedWindow = format("dst%d", count++);

		putText(dst, text, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);

		imshow(namedWindow, dst);
	}
	waitKey();
	destroyAllWindows();
}

void noise_gaussian() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	imshow("src", src);
	Mat dst, noise(src.size(), CV_32SC1);  // ������ ����� ��ȣ�ִ� ����������
	int count = 1;

	for (int sigma = 10; sigma <= 30; sigma += 10) {
		randn(noise, 0, sigma);

		add(src, noise, dst, Mat(), CV_8U);  // ������ ������ ���ϰ� ������ ���̴� CV_U8�� ����

		String text = format("sigma : %d", sigma);
		String namedWindow = format("dst%d", count++);

		putText(dst, text, Point(10, 30), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255), 1, LINE_AA);
		imshow(namedWindow, dst);
	}
	waitKey();
	destroyAllWindows();
}

// ����� ����
void filter_bilateral() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
			return;
	}

	// ���� ���� ������ �����ش�, ��� 0, ǥ�������� 5�� ����þ� ����
	Mat noise(src.size(), CV_32SC1);
	randn(noise, 0, 5);
	add(src, noise, src, Mat(), CV_8U);

	// ǥ�������� 5�� ����þ� ���͸�
	Mat dst1;
	GaussianBlur(src, dst1, Size(), 5);

	// ������ ǥ������ 10, ��ǥ���� ǥ������ 5�� ����� ���͸�
	Mat dst2;
	bilateralFilter(src, dst2, -1, 10, 5);

	imshow("src", src);
	imshow("dst1", dst1);
	imshow("dst2", dst2);

	waitKey();
	destroyAllWindows();
}

void filter_median() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;;
		return;
	}

	// ���� ������ 10% ������ 0 �Ǵ� 255 ���� ���� �߰�
	int num = (int)(src.total() * 0.1);
	for (int i = 0; i < num; ++i) {
		int x = rand() % src.cols;
		int y = rand() % src.rows;
		src.at<uchar>(y, x) = (i % 2) * 255;
	}

	// ǥ�������� 1�� ����þ� ���͸�
	Mat dst1;
	GaussianBlur(src, dst1, Size(), 1);

	// ũ�Ⱑ 3�� �̵�� ���͸�
	Mat dst2;
	medianBlur(src, dst2, 3);

	imshow("src", src);
	imshow("dst1", dst1);
	imshow("dst2", dst2);

	waitKey();
	destroyAllWindows();
}


int main(void) {
	// filter_embossing();
	// blurring_mean();
	// blurring_gaussian();
	// unsharp_mask();
	// noise_gaussian();
	// filter_bilateral();
	filter_median();

	return 0;
}