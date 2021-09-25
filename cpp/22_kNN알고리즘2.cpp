#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

Point ptPrev(-1, -1);

Ptr<KNearest> train_knn();
void on_mouse(int event, int x, int y, int flags, void* userdata);

// KNearest 클래스를 이용한 필기체 숫자인식
int main(void) {
	// 필기체 숫자를 학습한 결과를 knn에 저장
	Ptr<KNearest> knn = train_knn();

	if (knn.empty()) {
		cerr << "Training failed" << endl;
		return -1;
	}
	// 마우스로 글씨 쓰기
	Mat img = Mat::zeros(Size(400, 400), CV_8U);
	imshow("img", img);
	setMouseCallback("img", on_mouse, (void*)&img);

	while (true) {
		int c = waitKey();
		// esc 누르면 종료
		if (c == 27)
			break;
		// space 누르면
		else if (c == ' ') {
			Mat img_resize, img_float, img_flatten, res;
			// 쓰여진 글씨를 20 X 20 크기로 변환하여 float자료형의 400 X 1 크기로 변환
			resize(img, img_resize, Size(20, 20), 0, 0, INTER_AREA);
			img_resize.convertTo(img_float, CV_32F);
			img_flatten = img_float.reshape(1, 1);
			// knn 알고리즘으로 분류하여 출력
			knn->findNearest(img_flatten, 10, res);
			cout << cvRound(res.at<float>(0, 0)) << endl;
			// img 영상을 검은색으로 다시 초기화
			img.setTo(0);
			imshow("img", img);
		}
	}
	return 0;
}


// kNN 알고리즘을 이용한 필기체 숫자 학습
Ptr<KNearest> train_knn() {
	Mat digits = imread("./images/digits.png", IMREAD_GRAYSCALE);

	if (digits.empty()) {
		cerr << "Image load failed" << endl;
		return 0;
	}

	Mat train_images, train_labels;

	// 가로 100개, 세로 50개의 필기체 숫자
	for (int j = 0; j < 50; ++j) {
		for (int i = 0; i < 100; ++i) {
			Mat roi, roi_float, roi_flatten;
			// 가로 i번째, 세로 j번째 필기체 숫자 영상을 roi에 저장
			roi = digits(Rect(i * 20, j * 20, 20, 20));
			// roi 영상을 float 자료형으로 변환 
			roi.convertTo(roi_float, CV_32F);
			// 400 X 1 크기의 영상으로 변환
			roi_flatten = roi_float.reshape(1, 1);
			// 훈련데이터와 정답데이터를 추가
			train_images.push_back(roi_flatten);
			train_labels.push_back(j / 5);
		}
	}
	// KNearest 객체를 생성하고 kNN 학습을 수행
	Ptr<KNearest> knn = KNearest::create();
	knn->train(train_images, ROW_SAMPLE, train_labels);
	return knn;
}


// 마우스로 숫자 그리기
void on_mouse(int event, int x, int y, int flags, void* userdata) {
	Mat img = *(Mat*)userdata;

	if (event == EVENT_LBUTTONDOWN) {
		ptPrev = Point(x, y);
	}
	else if (event == EVENT_LBUTTONUP) {
		ptPrev = Point(-1, -1);
	}
	else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
		line(img, ptPrev, Point(x, y), Scalar::all(255), 30, LINE_AA);
		ptPrev = Point(x, y);

		imshow("img", img);
	}
}
