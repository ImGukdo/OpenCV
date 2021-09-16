#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


// 기본 레이블링
void labeling_basic() {
	// 이진영상 생성
	uchar data[] = {
		0, 0, 1, 1, 0, 0, 0, 0,
		1, 1, 1, 1, 0, 0, 1, 0,
		1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 0, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	Mat src = Mat(8, 8, CV_8UC1, data) * 255;

	// 레이블맵 생성, 레이블 개수 반환
	Mat labels;
	int cnt = connectedComponents(src, labels);

	cout << "src:\n" << src << endl;
	cout << "labels:\n" << labels << endl;
	cout << "number of labels: " << cnt << endl;
}


// 레이블링을 이용하여 객체의 바운딩박스 그리기
void labeling_stats() {
	Mat src = imread("./images/keyboard.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 오츠 알고리즘으로 영상 이진화
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
	// 레이블링 수행, 각 객체의 통계정보 추출
	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	// 3채널 컬러 영상형식으로 변환
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);
	// 배경영역을 제외하고 흰색 객체 영역에 대해서
	for (int i = 1; i < cnt; ++i) {
		int* p = stats.ptr<int>(i);  // 1행부터 

		if (p[4] < 20) continue;  // 객체 픽셀 개수가 2보다 작으면 잡음으로 간주
		// 검출된 객체를 노란색 사각형으로 감쌈
		rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 255), 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}


// 외곽선 검출과 그리기
void contours_basic() {
	Mat src = imread("./images/contours.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// 모든 외곽선 검출
	vector<vector<Point> > contours;
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);

	// 3채널 영상으로 변환
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// 모든 외곽선을 임의의 색상으로 그림
	for (int i = 0; i < contours.size(); ++i) {
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, c, 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();

}

// 계층구조를 사용하는 외곽선 검출과 그리기
void contours_hier() {
	Mat src = imread("./images/contours.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// 외곽선 검출
	vector<vector<Point> > contours;  // 외곽선 픽셀을 받아올 벡터
	vector<Vec4i> hierarchy;  // 계층정보를 받아올 벡터
	findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// 0번 외곽선부터 다음 외각선으로 반복하면서 다음 외각선 번호가 -1이 될때까지
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, idx, c, -1, LINE_8, hierarchy);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}


// 다각형 검출 및 인식
void setLabel(Mat& img, const vector<Point>& pts, const String& label) {
	// 외곽선 주변에 바운딩박스를 그림
	Rect rc = boundingRect(pts);
	rectangle(img, rc, Scalar(0, 0, 255), 1);
	// 사각형의 왼쪽위의 점을 기준으로 텍스트를 출력
	putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
}

void boudingPoly() {
	Mat img = imread("./images/polygon.bmp", IMREAD_COLOR);

	if (img.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// 오츠 알고리즘으로 자동 이진화
	Mat bin;
	threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	// 외곽선 검출
	vector<vector<Point> > contours;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// 모든 외곽선 좌표를 탐색하면서
	for (vector<Point>& pts : contours) {
		// 면적이 400보다 작으면 잡음으로 간주
		if (contourArea(pts) < 400)
			continue;
		// 외곽선을 근사화 하여 저장
		vector<Point> approx;
		approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);
		// approx 에 저장된 점의 개수
		int vtc = (int)approx.size();
		// 점의 개수가 3이면 TRI
		if (vtc == 3) {
			setLabel(img, pts, "TRI");
		}
		// 점의 개수가 4이면 RECT
		else if (vtc == 4) {
			setLabel(img, pts, "RECT");
		}
		// 점의 개수가 4보다 크면 
		else if (vtc > 4) {
			// 객체의 면적 대 길이 비율 계산
			double len = arcLength(pts, true);
			double area = contourArea(pts);
			double ratio = 4 * CV_PI * area / (len * len);
			// 비율이 0.8보다 크면 CIR로 판단
			if (ratio > 0.8) {
				setLabel(img, pts, "CIR");
			}
		}
	}
	
	imshow("img", img);
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// labeling_basic();
	// labeling_stats();
	// contours_basic();
	// contours_hier();
	boudingPoly();

	return 0;
}