#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

// SVM 알고리즘을 이용한 2차원 점 분류
int main(void) {
	// 8개의 점 좌표를 포함하는 train 행렬
	Mat train = Mat_<float>({ 8, 2 }, { 150, 200, 200, 250, 100, 250, 150, 300,
		350, 100, 400, 200, 400, 300, 350, 400 });
	// 훈련 데이터 점들의 클래스를 정의한 label 행령
	Mat label = Mat_<int>({ 8, 1 }, { 0, 0, 0, 0, 1, 1, 1, 1 });
	// SVM 객체 생성, 타입, 커널함수 설정
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::Types::C_SVC);
	svm->setKernel(SVM::KernelTypes::RBF);
	// 자동으로 최적의 파라미터를 찾아 학습
	svm->trainAuto(train, ROW_SAMPLE, label);

	// img 영상의 모든 픽셀 좌표에 대해 SVM 응답을 구하여 빨간색 또는 녹색으로 표현
	Mat img = Mat::zeros(Size(500, 500), CV_8UC3);
	for (int j = 0; j < img.rows; ++j) {
		for (int i = 0; i < img.cols; ++i) {
			Mat test = Mat_<float>({ 1, 2 }, { (float)i, (float)j });
			int res = cvRound(svm->predict(test));

			if (res == 0)
				img.at<Vec3b>(j, i) = Vec3b(128, 128, 255);
			else
				img.at<Vec3b>(j, i) = Vec3b(128, 255, 128);
		}
	}

	// train 행렬에 저장된 훈련 데이터 점을 반지름 5인 원으로 표시
	for (int i = 0; i < train.rows; ++i) {
		int x = cvRound(train.at<float>(i, 0));
		int y = cvRound(train.at<float>(i, 1));
		int l = label.at<int>(i, 0);

		if (l == 0)
			circle(img, Point(x, y), 5, Scalar(0, 0, 128), -1, LINE_AA);
		else
			circle(img, Point(x, y), 5, Scalar(0, 128, 0), -1, LINE_AA);
	}

	imshow("img", img);
	waitKey();
	destroyAllWindows();
	return 0;
}