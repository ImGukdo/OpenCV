#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void MatOp1() {
	// Mat ��ü ����
	Mat img1;
	Mat img2(480, 640, CV_8UC1);
	Mat img3(480, 640, CV_8UC3);
	Mat img4(Size(640, 480), CV_8UC3);
	Mat img5(480, 640, CV_8UC1, Scalar(128));
	Mat img6(480, 640, CV_8UC3, Scalar(0, 0, 255));

	Mat mat1 = Mat::zeros(3, 3, CV_32SC1);  // ��� ���Ҹ� 0����
	Mat mat2 = Mat::ones(3, 3, CV_32FC1);  // ��� ���Ҹ� 1����
	Mat mat3 = Mat::eye(3, 3, CV_32FC1);  // ���� ���

	float  data[] = { 1, 2, 3, 4, 5, 6 };  // �̹� �Ҵ�Ǿ� �ִ� �ܺ� �޸𸮸� �̿��ؼ� �����ϱ�
	Mat mat4(2, 3, CV_32FC1, data);

	Mat_<float> mat5_(2, 3);  // Mat_ Ŭ������ ����Ͽ� ����
	mat5_ << 1, 2, 3, 4, 5, 6;
	Mat mat5 = mat5_;
	// Mat mat5 = Mat_<float>(2, 3) << 1, 2, 3, 4, 5, 6;
	Mat mat6 = Mat_<float>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });

	mat4.create(256, 256, CV_8UC3);  // �̹� ������ ��ü�� ���ο� ����� �Ҵ�, �ʱ�ȭ�� �Ұ�
	mat5.create(4, 4, CV_32FC1);

	mat4 = Scalar(255, 0, 0);  // ��� ���Ұ��� ����
	mat5.setTo(1.f);  // ��� ���Ұ��� ����
}

void MatOp2() {
	Mat img1 = imread("./images/dog.bmp");

	if (img1.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ���� ����
	Mat img2 = img1;
	Mat img3;
	img3 = img1;

	// ���� ����
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
	// ROI Ư�� ������ ����
	Mat img2 = img1(Rect(220, 120, 340, 240));  // ���� ����� ����
	Mat img3 = img1(Rect(220, 120, 340, 240)).clone();  // ���� ����� ����
	img2 = ~img2;  // �̹��� ����

	imshow("img1", img1);
	imshow("img2", img2);
	imshow("img3", img3);
	waitKey();
	destroyAllWindows();
}

void MatOp4() {
	Mat img1 = Mat::zeros(3, 4, CV_8UC1);
	// ��� ���Ҹ� �湮�Ͽ� ���� ����
	for (int j = 0; j < img1.rows; ++j) {
		for (int i = 0; i < img1.cols; ++i) {
			img1.at<uchar>(j, i)++;
		}
	}
	// ���� �ּҰ��� �޾ƿͼ� ��� ������ ���� ����
	for (int j = 0; j < img1.rows; ++j) {
		uchar* p = img1.ptr<uchar>(j);
		for (int i = 0; i < img1.cols; ++i) {
			p[i]++;
		}
	}
	// �ݺ��ڸ� ����Ͽ� ��� ������ ���� ����
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
	// ����� ���ο� ������ ���̸� ���
	cout << "width : " << img1.cols << endl;;
	cout << "height : " << img1.rows << endl;
	// ����� Ÿ���� ���
	if (img1.type() == CV_8UC1)
		cout << "img1 is a grayscale image" << endl;
	else if (img1.type() == CV_8UC3)
		cout << "img1 is a truecolor image" << endl;
}

void MatOp6() {
	float data[] = { 1, 1, 2, 3 };
	Mat mat1(2, 2, CV_32FC1, data);
	cout << "mat1 :\n" << mat1 << endl;
	// ����� ����
	Mat mat2 = mat1.inv(); // �����
	cout << "mat2 :\n" << mat2 << endl;
	cout << "mat1.t() :\n" << mat1.t() << endl;  // ��ġ���
	cout << "mat1 + 3 :\n" << mat1 + 3 << endl;  // ��İ� scalar ����
	cout << "mat1 + mat2 :\n" << mat1 + mat2 << endl;  // ��İ� ����� ����
	cout << "mat1 * mat2 :\n" << mat1 * mat2 << endl;  // ��İ� ����� ����
}

void MatOp7() {
	Mat img1 = imread("./imread/lenna.bmp", IMREAD_GRAYSCALE);
	// ����� Ÿ���� ����
	Mat img1f;
	img1.convertTo(img1f, CV_32FC1);

	// ����� ũ�⸦ ����
	uchar data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	Mat mat1(3, 4, CV_8UC1, data1);
	Mat mat2 = mat1.reshape(0, 1);  // ä�μ�, ���� ��, ���� �״�θ� 0

	cout << "mat1 :\n" << mat1 << endl;
	cout << "mat2 :\n" << mat2 << endl;

	mat1.resize(5, 100);  // ���� ������ 5�� �����ϰ� ���� �߰��� ���� 100���� �ʱ�ȭ

	Mat mat3 = Mat::ones(1, 4, CV_8UC1) * 255;
	mat1.push_back(mat3);  // mat1�� �� �Ʒ��� �߰�
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