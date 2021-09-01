#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void contrast1() {
	// 기본적인 명암비 조절, 너무 쉽게 포화되는 문제가 있음
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
	// 픽셀값의 절반인 128을 기준으로 명암비 조절
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
	CV_Assert(img.type() == CV_8UC1);  // 영상이 그레이스케일인지 확인

	Mat hist;  // 출력 히스토그램을 저장할 객체
	int channels[] = {0};  // 히스토그램을 구할 채널들
	int dims = 1;  //  출력 히스토그램 차원수
	const int histSize[] = { 256 };  // 히스토그램의 빈 개수
	float grayLevel[] = { 0, 256 };  // 히스토그램범위
	const float* ranges[] = { grayLevel }; // 각차원의 히스토그램범위값을 저장한 배열의 주소
	calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);  // 히스토그램 구하는 함수

	return hist;
}

Mat getGrayHistImage(const Mat& hist) {
	CV_Assert(hist.type() == CV_32FC1);
	CV_Assert(hist.size() == Size(1, 256));

	double histMax;  // 히스토그램의 값중 가장 큰값을 저장
	minMaxLoc(hist, 0, &histMax);

	Mat imgHist(100, 256, CV_8UC1, Scalar(255));  // 히스토그램을 그릴 영상 생성
	// 히스토그램의 그래프값을 계산하여 넣어준다
	for (int i = 0; i < 256; ++i) {
		line(imgHist, Point(i, 100),
			Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), Scalar(0));
	}
	return imgHist;
}

// 히스토그램 스트레칭
void histogram_stretching() {
	Mat src = imread("./images/hawkes.bmp", IMREAD_GRAYSCALE);
	
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	
	Mat dst(src.rows, src.cols, src.type());
	double gmin, gmax;  // src 영상 픽셀값의 최소값과 최대값을 구한다
	minMaxLoc(src, &gmin, &gmax);

	dst = (src - gmin) * 255 / (gmax - gmin);  // 스트레칭 계산
	
	imshow("src", src);
	imshow("srcHist", getGrayHistImage(calcGrayHist(src)));

	imshow("dst", dst);
	imshow("dstHist", getGrayHistImage(calcGrayHist(dst)));

	waitKey();
	destroyAllWindows();
}

// 히스토그램 평활화
void histogram_equalization() {
	Mat src = imread("./images/hawkes.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst;
	equalizeHist(src, dst);  // 히스토그램 평활화 함수, 그레이스케일 영상만 가능

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
