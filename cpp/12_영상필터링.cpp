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

	// 엠보싱 필터 마스크 행렬 생성
	float data[] = { -1, -1, 0, -1, 0, 1, 0, 1, 1 };
	Mat emboss(3, 3, CV_32FC1, data);

	Mat dst;
	// filter2D 함수는 입력영상의 깊이에 따라 가능한 출력영상의 깊이가 지정되어 있음
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
	// 필터의 크기가 클수록 연산량이 증가하고 더욱 부드러운 느낌의 결과 영상을 생성
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
	// 시그마의 값이 클수록 더욱 부드러운 느낌의 결과 영상을 생성
	for (int sigma = 1; sigma <= 5; ++sigma) {
		// 커널 사이즈를 Size()로 하면 표준편차로부터 크기를 자동 결정
		// ysigma 값이 없으면 xsigma값과 같은 값을 사용
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
		GaussianBlur(src, blurred, Size(), sigma);  // 블러링 영상을 만듦
		float alpha = 2.f;  // 가중치값
	
		dst = (1 + alpha) * src - alpha * blurred;  // 언샤프 마스크 필터링 수행

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
	Mat dst, noise(src.size(), CV_32SC1);  // 노이즈 행렬은 부호있는 정수형으로
	int count = 1;

	for (int sigma = 10; sigma <= 30; sigma += 10) {
		randn(noise, 0, sigma);

		add(src, noise, dst, Mat(), CV_8U);  // 원본과 잡음을 더하고 영상의 깊이는 CV_U8로 설정

		String text = format("sigma : %d", sigma);
		String namedWindow = format("dst%d", count++);

		putText(dst, text, Point(10, 30), FONT_HERSHEY_COMPLEX, 1.0, Scalar(255), 1, LINE_AA);
		imshow(namedWindow, dst);
	}
	waitKey();
	destroyAllWindows();
}

// 양방향 필터
void filter_bilateral() {
	Mat src = imread("./images/lenna.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
			return;
	}

	// 원본 영상에 잡음을 더해준다, 평균 0, 표준편차가 5인 가우시안 잡음
	Mat noise(src.size(), CV_32SC1);
	randn(noise, 0, 5);
	add(src, noise, src, Mat(), CV_8U);

	// 표준편차가 5인 가우시안 필터링
	Mat dst1;
	GaussianBlur(src, dst1, Size(), 5);

	// 색공간 표준편차 10, 좌표공간 표준편차 5인 양방향 필터링
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

	// 원본 영상의 10% 영역에 0 또는 255 값의 잡음 추가
	int num = (int)(src.total() * 0.1);
	for (int i = 0; i < num; ++i) {
		int x = rand() % src.cols;
		int y = rand() % src.rows;
		src.at<uchar>(y, x) = (i % 2) * 255;
	}

	// 표준편차가 1인 가우시안 필터링
	Mat dst1;
	GaussianBlur(src, dst1, Size(), 1);

	// 크기가 3인 미디언 필터링
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