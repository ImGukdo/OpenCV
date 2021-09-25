#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::ml;

Point ptPrev(-1, -1);

Ptr<KNearest> train_knn();
void on_mouse(int event, int x, int y, int flags, void* userdata);

// KNearest Ŭ������ �̿��� �ʱ�ü �����ν�
int main(void) {
	// �ʱ�ü ���ڸ� �н��� ����� knn�� ����
	Ptr<KNearest> knn = train_knn();

	if (knn.empty()) {
		cerr << "Training failed" << endl;
		return -1;
	}
	// ���콺�� �۾� ����
	Mat img = Mat::zeros(Size(400, 400), CV_8U);
	imshow("img", img);
	setMouseCallback("img", on_mouse, (void*)&img);

	while (true) {
		int c = waitKey();
		// esc ������ ����
		if (c == 27)
			break;
		// space ������
		else if (c == ' ') {
			Mat img_resize, img_float, img_flatten, res;
			// ������ �۾��� 20 X 20 ũ��� ��ȯ�Ͽ� float�ڷ����� 400 X 1 ũ��� ��ȯ
			resize(img, img_resize, Size(20, 20), 0, 0, INTER_AREA);
			img_resize.convertTo(img_float, CV_32F);
			img_flatten = img_float.reshape(1, 1);
			// knn �˰������� �з��Ͽ� ���
			knn->findNearest(img_flatten, 10, res);
			cout << cvRound(res.at<float>(0, 0)) << endl;
			// img ������ ���������� �ٽ� �ʱ�ȭ
			img.setTo(0);
			imshow("img", img);
		}
	}
	return 0;
}


// kNN �˰����� �̿��� �ʱ�ü ���� �н�
Ptr<KNearest> train_knn() {
	Mat digits = imread("./images/digits.png", IMREAD_GRAYSCALE);

	if (digits.empty()) {
		cerr << "Image load failed" << endl;
		return 0;
	}

	Mat train_images, train_labels;

	// ���� 100��, ���� 50���� �ʱ�ü ����
	for (int j = 0; j < 50; ++j) {
		for (int i = 0; i < 100; ++i) {
			Mat roi, roi_float, roi_flatten;
			// ���� i��°, ���� j��° �ʱ�ü ���� ������ roi�� ����
			roi = digits(Rect(i * 20, j * 20, 20, 20));
			// roi ������ float �ڷ������� ��ȯ 
			roi.convertTo(roi_float, CV_32F);
			// 400 X 1 ũ���� �������� ��ȯ
			roi_flatten = roi_float.reshape(1, 1);
			// �Ʒõ����Ϳ� ���䵥���͸� �߰�
			train_images.push_back(roi_flatten);
			train_labels.push_back(j / 5);
		}
	}
	// KNearest ��ü�� �����ϰ� kNN �н��� ����
	Ptr<KNearest> knn = KNearest::create();
	knn->train(train_images, ROW_SAMPLE, train_labels);
	return knn;
}


// ���콺�� ���� �׸���
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
