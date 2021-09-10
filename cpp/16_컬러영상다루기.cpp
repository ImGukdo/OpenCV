#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// 컬러영상 색반전
void color_inverse() {
	Mat src = imread("./images/butterfly.jpg", IMREAD_COLOR);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst(src.rows, src.cols, src.type());

	// 이미지 반전
	for (int j = 0; j < src.rows; ++j) {
		for (int i = 0; i < src.cols; ++i) {
			Vec3b& p1 = src.at<Vec3b>(j, i);
			Vec3b& p2 = dst.at<Vec3b>(j, i);

			p2[0] = 255 - p1[0];  // B
			p2[1] = 255 - p1[1];  // G
			p2[2] = 255 - p1[2];  // R
		}
	}

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

// 컬러영상 그레이르케일 영상으로 변환
void color_grayscale() {
	Mat src = imread("./images/butterfly.jpg");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat dst;
	// BGR 컬러 영상을 GRAYSCALE 영상으로 변환
	cvtColor(src, dst, COLOR_BGR2GRAY);  

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

// 컬러영상 채널 나누기
void color_split() {
	Mat src = imread("./images/candies.png");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	vector<Mat> bgr_planes;  // B, G, R 행렬을 저장할 벡터
	split(src, bgr_planes);  // 영상 분리

	imshow("src", src);
	imshow("B_plane", bgr_planes[0]);
	imshow("G_plane", bgr_planes[1]);
	imshow("R_plane", bgr_planes[2]);
	waitKey();
	destroyAllWindows();
}

// 컬러영상 히스토그램 평활화
void color_equalizeHist() {
	Mat src = imread("./images/pepper.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// BGR색 공간을 YCrCb색 공간으로 변경
	Mat src_ycrcb;
	cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);

	// 영상의 채널을 분리
	vector<Mat> ycrcb_planes;
	split(src_ycrcb, ycrcb_planes);

	// Y 성분만 평활화 수행
	equalizeHist(ycrcb_planes[0], ycrcb_planes[0]);

	// 다시 세 영상을 합쳐서 영상 생성
	Mat dst_ycrcb;
	merge(ycrcb_planes, dst_ycrcb);

	// YCrCb색 공간을 BGR색 공간으로 변경
	Mat dst;
	cvtColor(dst_ycrcb, dst, COLOR_YCrCb2BGR);

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

// 특정 생상으로 영역 분할
int lower_hue = 40, upper_hue = 80;  // 트랙바의 위치를 저장할 변수
Mat src1, src_hsv, mask;
void on_hue_changed(int, void*) {
	// 색상의 하한값과 상한값
	Scalar lowerb(lower_hue, 100, 0); 
	Scalar upperb(upper_hue, 255, 255);
	inRange(src_hsv, lowerb, upperb, mask);
	imshow("mask", mask);
}

void inrange() {
	src1 = imread("./images/candies.png", IMREAD_COLOR);

	if (src1.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	//  BGR색 공간을  HSV색 공간으로 변환
	cvtColor(src1, src_hsv, COLOR_BGR2HSV);

	imshow("src1", src1);

	// 트랙바 생성, 하한값과 상한값을 조절하기 위해
	namedWindow("mask");
	createTrackbar("Lower Hue", "mask", &lower_hue, 179, on_hue_changed);
	createTrackbar("upper Hue", "mask", &upper_hue, 179, on_hue_changed);
	on_hue_changed(0, 0);

	waitKey();
	destroyAllWindows();
}

// 히스토그램 역투영
void crcb_histogram() {
	Mat ref, ref_ycrcb, mask;
	ref = imread("./images/ref.png", IMREAD_COLOR);
	mask = imread("./images/mask.bmp", IMREAD_GRAYSCALE);

	if (ref.empty() || mask.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	cvtColor(ref, ref_ycrcb, COLOR_BGR2YCrCb);  // YCrCb 색 공간으로 변환
	
	// 피부색 영역의 히스토그램 생성
	Mat hist;
	int channels[] = { 1 , 2 };
	int cr_bins = 128, cb_bins = 128;
	int histSize[] = { cr_bins, cb_bins };
	float cb_range[] = { 0, 256 };
	float cr_range[] = { 0, 256 };
	const float* ranges[] = { cr_range, cb_range };
	calcHist(&ref_ycrcb, 1, channels, mask, hist, 2, histSize, ranges);

	Mat src, src_ycrcb;
	src = imread("./images/kids.png", IMREAD_COLOR);
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);

	// 히스토그램 역투영
	Mat backproj;
	calcBackProject(&src_ycrcb, 1, channels, hist, backproj, ranges, 1, true);

	imshow("src", src);
	imshow("backproj", backproj);
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// color_inverse();
	// color_grayscale();
	// color_split();
	// color_equalizeHist();
	// inrange();
	crcb_histogram();
	return 0;
}