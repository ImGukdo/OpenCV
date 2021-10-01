#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

// ���콺�� �̿��� ���� �׸���
void on_mouse(int event, int x, int y, int flags, void* userdata) {
	Mat img = *(Mat*)userdata;
	static Point ptPrev(-1, -1);

	if (event == EVENT_LBUTTONDOWN) {
		ptPrev = Point(x, y);
	}
	else if (event == EVENT_LBUTTONUP) {
		ptPrev = Point(-1, -1);
	}
	else if (event == EVENT_MOUSEMOVE && flags & EVENT_FLAG_LBUTTON) {
		line(img, ptPrev, Point(x, y), Scalar(255), 40, LINE_AA);
		ptPrev = Point(x, y);
		imshow("img", img);
	}
}

// �н��� �������� �̿��� �ʱ�ü ���� �ν�
int main(void) {
	// mnist_cnn.pb(�ټ��÷ο� �� ����)�� �̿��Ͽ� Net ��ü ����
	Net net = readNet("mnist_cnn.pb");
	// Net ��ü�� ���������� �������� ������ ����
	if (net.empty()) {
		cerr << "Network load failed" << endl;
		return -1;
	}

	Mat img = Mat::zeros(400, 400, CV_8UC1);
	imshow("img", img);
	setMouseCallback("img", on_mouse, (void*)&img);

	while (true) {
		int c = waitKey();
		if (c == 27)
			break;
		else if (c == ' ') {
			// 4���� 1X1X28X28ũ���� ��� ����
			Mat inputBlob = blobFromImages(img, 1 / 255.f, Size(28, 28));
			// ��Ʈ��ũ �Է����� ����
			net.setInput(inputBlob);
			// ��Ʈ��ũ�� ���������� ����
			Mat prob = net.forward();
			// prob ��Ŀ��� �ִ񰪰� �ִ� ��ġ�� ã��
			double maxVal;  // �ִ� Ȯ����
			Point maxLoc;  // maxLoc.x ����Ȯ���� ���� ����
			minMaxLoc(prob, 0, &maxVal, 0, &maxLoc);
			int digit = maxLoc.x;

			cout << digit << " (" << maxVal * 100 << "%)" << endl;

			img.setTo(Scalar(0));
			imshow("img", img);
		}
	}
	return 0;
}