#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void contrast1() {
	// �⺻���� ��Ϻ� ����, �ʹ� ���� ��ȭ�Ǵ� ������ ����
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	float s = 2.f;
	Mat dst = s * src;
	
	imshow("src", src);
	imshow("dst", dst);
	waitKey();

	destroyAllWindows();
}

void contrast2() {
	// �ȼ����� ������ 128�� �������� ��Ϻ� ����
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	float alpha = 1.f;
	Mat dst = src + (src - 128) * alpha;

	imshow("src", src);
	imshow("dst", dst);
	waitKey();

	destroyAllWindows();
}

Mat calcGrayHist(const Mat& img) {
	CV_Assert(img.type() == CV_8UC1);  // ������ �׷��̽��������� Ȯ��

	Mat hist;  // ��� ������׷��� ������ ��ü
	int channels[] = {0};  // ������׷��� ���� ä�ε�
	int dims = 1;  //  ��� ������׷� ������
	const int histSize[] = { 256 };  // ������׷��� �� ����
	float grayLevel[] = { 0, 256 };  // ������׷�����
	const float* ranges[] = { grayLevel }; // �������� ������׷��������� ������ �迭�� �ּ�
	calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);  // ������׷� ���ϴ� �Լ�

	return hist;
}

Mat getGrayHistImage(const Mat& hist) {
	CV_Assert(hist.type() == CV_32FC1);
	CV_Assert(hist.size() == Size(1, 256));

	double histMax;  // ������׷��� ���� ���� ū���� ����
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8UC1, Scalar(255));  // ������׷��� �׸� ���� ����
	// ������׷��� �׷������� ����Ͽ� �־��ش�
	for (int i = 0; i < 256; ++i) {
		line(imgHist, Point(i, 100),
			Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
	}
	return imgHist;
}

// ������׷� ��Ʈ��Ī
void histogram_stretching() {
	Mat src = imread("./images/hawkes.bmp", IMREAD_GRAYSCALE);
	
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	
	Mat dst(src.rows, src.cols, src.type());
	double gmin, gmax;  // src ���� �ȼ����� �ּҰ��� �ִ밪�� ���Ѵ�
	minMaxLoc(src, &gmin, &gmax);

	dst = (src - gmin) * 255 / (gmax - gmin);  // ��Ʈ��Ī ���
	
	imshow("src", src);
	imshow("srcHist", getGrayHistImage(calcGrayHist(src)));

	imshow("dst", dst);
	imshow("dstHist", getGrayHistImage(calcGrayHist(dst)));

	waitKey();
	destroyAllWindows();
}

// ������׷� ��Ȱȭ
void histogram_equalization() {
	Mat src = imread("./images/hawkes.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst;
	equalizeHist(src, dst);  // ������׷� ��Ȱȭ �Լ�, �׷��̽����� ���� ����

	imshow("src", src);
	imshow("srcHist", getGrayHistImage(calcGrayHist(src)));

	imshow("dst", dst);
	imshow("dstHist", getGrayHistImage(calcGrayHist(dst)));

	waitKey();
	destroyAllWindows();

}

int main(void) {
	// contrast1();
	// contrast2();
	// histogram_stretching();
	histogram_equalization();

	return 0;
}
