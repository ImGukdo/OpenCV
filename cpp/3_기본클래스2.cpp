#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void MatOp1() {
	// Mat 객체 생성
	Mat img1;
	Mat img2(480, 640, CV_8UC1);
	Mat img3(480, 640, CV_8UC3);
	Mat img4(Size(640, 480), CV_8UC3);
	Mat img5(480, 640, CV_8UC1, Scalar(128));
	Mat img6(480, 640, CV_8UC3, Scalar(0, 0, 255));

	Mat mat1 = Mat::zeros(3, 3, CV_32SC1);  // 모든 원소를 0으로
	Mat mat2 = Mat::ones(3, 3, CV_32FC1);  // 모든 원소를 1으로
	Mat mat3 = Mat::eye(3, 3, CV_32FC1);  // 단위 행렬

	float  data[] = { 1, 2, 3, 4, 5, 6 };  // 이미 할당되어 있는 외부 메모리를 이용해서 생성하기
	Mat mat4(2, 3, CV_32FC1, data);

	Mat_<float> mat5_(2, 3);  // Mat_ 클래스를 사용하여 생성
	mat5_ << 1, 2, 3, 4, 5, 6;
	Mat mat5 = mat5_;
	// Mat mat5 = Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6;
	Mat mat6 = Mat_<float>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });

	mat4.create(256, 256, CV_8UC3);  // 이미 생성된 객체에 새로운 행렬을 할당, 초기화는 불가
	mat5.create(4, 4, CV_32FC1);

	mat4 = Scalar(255, 0, 0);  // 모든 원소값을 설정
	mat5.setTo(1.f);  // 모든 원소값을 설정
}

void MatOp2() {
	Mat img1 = imread("./images/dog.bmp");

	if (img1.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// 얇은 복사
	Mat img2 = img1;
	Mat img3;
	img3 = img1;

	// 깊은 복사
	Mat img4 = img1.clone();
	Mat img5;
	img1.copyTo(img5);

	img1 = Scalar(0, 255, 255);

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("img3", img3);
	imshow("img4", img4);
	imshow("img5", img5);
	
	waitKey();
	destroyAllWindows();
}

void MatOp3() {
	Mat img1 = imread("./images/cat.bmp");
	if (img1.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ROI 특정 영역을 추출
	Mat img2 = img1(Rect(220, 120, 340, 240));  // 얇은 복사로 추출
	Mat img3 = img1(Rect(220, 120, 340, 240)).clone();  // 깊은 복사로 추출
	img2 = ~img2;  // 이미지 반전

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("img3", img3);
	waitKey();
	destroyAllWindows();
}

void MatOp4() {
	Mat img1 = Mat::zeros(3, 4, CV_8UC1);
	// 모든 원소를 방문하여 값을 변경
	for (int j = 0; j < img1.rows; ++j) {
		for (int i = 0; i < img1.cols; ++i) {
			img1.at<uchar>(j, i)++;
		}
	}
	// 행의 주소값을 받아와서 모든 원소의 값을 변경
	for (int j = 0; j < img1.rows; ++j) {
		uchar* p = img1.ptr<uchar>(j);
		for (int i = 0; i < img1.cols; ++i) {
			p[i]++;
		}
	}
	// 반복자를 사용하여 모든 원소의 값을 변경
	for (MatIterator_<uchar> it = img1.begin<uchar>(); it < img1.end<uchar>(); ++it) {
		(*it)++;
	}
	cout << img1 << endl;
}

void MatOp5() {
	Mat img1 = imread("./images/lenna.bmp");

	if (img1.empty()) {
		cerr << "Image load failed" << endl;
			return;
	}
	// 행렬의 가로와 세로의 길이를 출력
	cout << "width : " << img1.cols << endl;;
	cout << "height : " << img1.rows << endl;
	// 행렬의 타입을 출력
	if (img1.type() == CV_8UC1)
		cout << "img1 is a grayscale image" << endl;
	else if (img1.type() == CV_8UC3)
		cout << "img1 is a truecolor image" << endl;
}

void MatOp6() {
	float data[] = { 1, 1, 2, 3 };
	Mat mat1(2, 2, CV_32FC1, data);
	cout << "mat1 :\n" << mat1 << endl;
	// 행렬의 연산
	Mat mat2 = mat1.inv(); // 역행렬
	cout << "mat2 :\n" << mat2 << endl;
	cout << "mat1.t() :\n" << mat1.t() << endl;  // 전치행렬
	cout << "mat1 + 3 :\n" << mat1 + 3 << endl;  // 행렬과 scalar 덧셈
	cout << "mat1 + mat2 :\n" << mat1 + mat2 << endl;  // 행렬과 행렬의 덧셈
	cout << "mat1 * mat2 :\n" << mat1 * mat2 << endl;  // 행렬과 행렬의 곱셈
}

void MatOp7() {
	Mat img1 = imread("./imread/lenna.bmp", IMREAD_GRAYSCALE);
	// 행렬의 타입을 변경
	Mat img1f;
	img1.convertTo(img1f, CV_32FC1);

	// 행렬의 크기를 변경
	uchar data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	Mat mat1(3, 4, CV_8UC1, data1);
	Mat mat2 = mat1.reshape(0, 1);  // 채널수, 행의 수, 기존 그대로면 0

	cout << "mat1 :\n" << mat1 << endl;
	cout << "mat2 :\n" << mat2 << endl;

	mat1.resize(5, 100);  // 행의 갯수를 5로 증가하고 새로 추가된 행은 100으로 초기화

	Mat mat3 = Mat::ones(1, 4, CV_8UC1) * 255;
	mat1.push_back(mat3);  // mat1의 맨 아래에 추가
}

int main(void) {
	// MatOp2();
	// MatOp3();
	// MatOp4();
	// MatOp5();
	// MatOp6();
	MatOp7();

	return 0;
}