#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

Mat img;
Mat train, label;
Ptr<KNearest> knn;
int k_value = 1;

void on_k_changed(int, void*);
void addPoint(const Point& pt, int cls);
void trainAndDisplay();

// kNN 알고리즘을 이용한 2차원 점 분류
int main(void) {
	img = Mat::zeros(Size(500, 500), CV_8UC3);
	knn = KNearest::create();

	namedWindow("knn");
	createTrackbar("k", "knn", &k_value, 5, on_k_changed);
	
	// 점을 각각 30개씩 생성하여 훈련 데이터에 추가
	const int NUM = 30;
	Mat rn(NUM, 2, CV_32SC1);
	// (150, 150) 좌표를 중심으로 하는 0번 클래스 점
	randn(rn, 0, 50);
	for (int i = 0; i < NUM; ++i) {
		addPoint(Point(rn.at<int>(i, 0) + 150, rn.at<int>(i, 1) + 150), 0);
	}
	// (350, 150) 좌표를 중심으로 하는 0번 클래스 점
	randn(rn, 0, 50);
	for (int i = 0; i < NUM; ++i) {
		addPoint(Point(rn.at<int>(i, 0) + 350, rn.at<int>(i, 1) + 150), 1);
	}
	// (250, 400) 좌표를 중심으로 하는 0번 클래스 점
	randn(rn, 0, 70);
	for (int i = 0; i < NUM; ++i) {
		addPoint(Point(rn.at<int>(i, 0) + 250, rn.at<int>(i, 1) + 400), 2);
	}
	// 프로그램이 시작되자마자 분류결과를 보여줌
	trainAndDisplay();

	waitKey();
	destroyAllWindows();
	return 0;
}

// k값에 따라 knn 알고리즘을 학습시키고 결과를 화면에 나타냄
void on_k_changed(int, void*) {
	// k값의 최솟값은 1
	if (k_value < 1) k_value = 1;
	trainAndDisplay();
}

// 좌표를 cls클래스로 등록
void addPoint(const Point& pt, int cls) {
	Mat new_sample = (Mat_<float>(1, 2) << pt.x, pt.y);
	train.push_back(new_sample);

	Mat new_label = (Mat_<int>(1, 1) << cls);
	label.push_back(new_label);
}

// kNN 알고리즘을 학습시키고 결과를 보여줌
void trainAndDisplay() {
	// kNN 알고리즘 학습
	knn->train(train, ROW_SAMPLE, label);
	// img 영상 전체 좌표에 대해 kNN 분류기 응답을 조사하여 색상을 표시
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			Mat sample = (Mat_<float>(1, 2) << j, i);

			Mat res;
			knn->findNearest(sample, k_value, res);

			int response = cvRound(res.at<float>(0, 0));
			if (response == 0)
				img.at<Vec3b>(i, j) = Vec3b(128, 128, 255);
			else if (response == 1)
				img.at<Vec3b>(i, j) = Vec3b(128, 255, 128);
			else if (response == 2)
				img.at<Vec3b>(i, j) = Vec3b(255, 128, 128);
		}
	}
	// 추가한 훈련 데이터 점 좌표를 원으로 표시
	for (int i = 0; i < train.rows; ++i) {
		int x = cvRound(train.at<float>(i, 0));
		int y = cvRound(train.at<float>(i, 1));
		int l = label.at<int>(i, 0);

		if (l == 0)
			circle(img, Point(x, y), 5, Scalar(0, 0, 128), -1, LINE_AA);
		else if(l == 1)
			circle(img, Point(x, y), 5, Scalar(0, 128, 0), -1, LINE_AA);
		else if (l == 2)
			circle(img, Point(x, y), 5, Scalar(128, 0, 0), -1, LINE_AA);
	}
	imshow("knn", img);
}