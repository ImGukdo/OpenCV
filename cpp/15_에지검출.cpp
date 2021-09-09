#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void sobel_derivative() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// x�� ����� y�� �������� ��̺��ϴ� ����ũ
	Mat mx = Mat_<float>({ 1, 3 }, { -1 / 2.f, 0, 1 / 2.f });
	Mat my = Mat_<float>({ 3, 1 }, { -1 / 2.f, 0, 1 / 2.f });

	Mat dst1, dst2;
	filter2D(src, dst1, -1, mx, Point(-1, -1), 128);
	filter2D(src, dst2, -1, my, Point(-1, -1), 128);

	imshow("src", src);
	imshow("dst1", dst1);
	imshow("dst2", dst2);
	waitKey();
	destroyAllWindows();
}

void sobel_edge() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dx, dy;
	Sobel(src, dx, CV_32FC1, 1, 0);  // x�� ���� 1�� ��̺�
	Sobel(src, dy, CV_32FC1, 0, 1);  // y�� ���� 1�� ��̺�
	
	Mat fmag, mag;
	magnitude(dx, dy, fmag);  // �׷����Ʈ ũ�⸦ ���
	fmag.convertTo(mag, CV_8UC1);  // �׷��̽����� �������� ����

	Mat edge = mag > 150;  // �Ӱ谪�� 150���� ����, 150���� ũ�� 255, ������ 0

	imshow("src", src);
	imshow("mag", mag);
	imshow("edge", edge);

	waitKey();
	destroyAllWindows();
}

void canny_edge() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// �Ӱ谪�� ������� �� ���� ���� ����
	Mat dst1, dst2;
	Canny(src, dst1, 50, 100);
	Canny(src, dst2, 50, 150);

	imshow("src", src);
	imshow("dst1", dst1);
	imshow("dst2", dst2);

	waitKey();
	destroyAllWindows();
}

void hough_lines() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ĳ�� ���� ����⸦ �̿��� ���� ������ ����
	Mat edge;
	Canny(src, edge, 50, 150);
	// ������ ������ �Ķ���͸� ������ ����, ������������ �Ÿ�, x����� ����
	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI / 180, 250);  // 1�ȼ�����, 1������, �Ӱ谪

	// ���������� 3ä�� �÷��������� ��ȯ, ������ ���������� ǥ���ϱ� ���ؼ�
	Mat dst;
	cvtColor(edge, dst, COLOR_GRAY2BGR);

	for (size_t i = 0; i < lines.size(); ++i) {
		float r = lines[i][0], t = lines[i][1];  // ������������ �Ÿ�, x����� ����
		double cos_t = cos(t), sin_t = sin(t);  
		double x0 = r * cos_t, y0 = r * sin_t;  // �������� ���������� ������
		double alpha = 1000;  // ���İ��� ����� Ŀ�� ������ ���� ������ �׷�����
		
							  // �������� ������ ���� ������ ���� �׷��ش�
		Point pt1(cvRound(x0 + alpha * sin_t), cvRound(y0 - alpha * cos_t));
		Point pt2(cvRound(x0 - alpha * sin_t), cvRound(y0 + alpha * cos_t));
		line(dst, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	}
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void hough_line_segments() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ĳ�� ���� ����⸦ �̿��� ���� ������ ����
	Mat edge;
	Canny(src, edge, 50, 150);

	// ������ ������ ��ǥ�� ������ ����
	vector<Vec4i> lines;
	HoughLinesP(edge, lines, 1, CV_PI / 180, 160, 50, 5);

	// ���������� 3ä�� �÷��������� ��ȯ, ������ ���������� ǥ���ϱ� ���ؼ�
	Mat dst;
	cvtColor(edge, dst, COLOR_GRAY2BGR);

	for (Vec4i l : lines) {
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, LINE_AA);
	}

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void hough_circles() {
	Mat src = imread("./images/coins.png", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat blurred;
	blur(src, blurred, Size(3, 3));

	vector<Vec3f> circles;
	HoughCircles(blurred, circles, HOUGH_GRADIENT, 1, 50, 150, 30);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (Vec3f c : circles) {
		Point center(cvRound(c[0]), cvRound(c[1]));
		int radius = cvRound(c[2]);
		circle(dst, center, radius, Scalar(0, 0, 255), 2, LINE_AA);
	}
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// sobel_derivative();
	// sobel_edge();
	// canny_edge();
	// hough_lines();
	// hough_line_segments();
	hough_circles();

	return 0;
}