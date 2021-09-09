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

	// x축 방향과 y축 방향으로 편미분하는 마스크
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
	Sobel(src, dx, CV_32FC1, 1, 0);  // x축 방향 1차 편미분
	Sobel(src, dy, CV_32FC1, 0, 1);  // y축 방향 1차 편미분
	
	Mat fmag, mag;
	magnitude(dx, dy, fmag);  // 그래디언트 크기를 계산
	fmag.convertTo(mag, CV_8UC1);  // 그레이스케일 형식으로 변경

	Mat edge = mag > 150;  // 임계값을 150으로 설정, 150보다 크면 255, 작으면 0

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

	// 임계값을 낮출수록 더 많은 에지 검출
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
	// 캐니 에지 검출기를 이용해 에지 영상을 구함
	Mat edge;
	Canny(src, edge, 50, 150);
	// 직선의 방정식 파라미터를 저장할 백터, 원점에서부터 거리, x축과의 각도
	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI / 180, 250);  // 1픽셀단위, 1도단위, 임계값

	// 에지영상을 3채널 컬러영상으로 변환, 직선을 빨간선으로 표현하기 위해서
	Mat dst;
	cvtColor(edge, dst, COLOR_GRAY2BGR);

	for (size_t i = 0; i < lines.size(); ++i) {
		float r = lines[i][0], t = lines[i][1];  // 원점에서부터 거리, x축과의 각도
		double cos_t = cos(t), sin_t = sin(t);  
		double x0 = r * cos_t, y0 = r * sin_t;  // 원점에서 직선까지의 수직점
		double alpha = 1000;  // 알파값이 충분히 커야 끊김이 없는 직선이 그려진다
		
							  // 직선위의 두점을 구해 빨간색 선을 그려준다
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
	// 캐니 에지 검출기를 이용해 에지 영상을 구함
	Mat edge;
	Canny(src, edge, 50, 150);

	// 직선의 두점의 좌표를 저장할 백터
	vector<Vec4i> lines;
	HoughLinesP(edge, lines, 1, CV_PI / 180, 160, 50, 5);

	// 에지영상을 3채널 컬러영상으로 변환, 직선을 빨간선으로 표현하기 위해서
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