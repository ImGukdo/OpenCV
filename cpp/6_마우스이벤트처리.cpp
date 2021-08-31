#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat img;
Point ptOld;

void on_mouse(int event, int x, int y, int flags, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN :  // ���콺 ���� ��ư�� ������
		ptOld = Point(x, y);
		cout << "EVENT_LBUTTONDOWN : " << x << ", " << y << endl;
		break;

	case EVENT_LBUTTONUP :  // ���콺 ���� ��ư�� ����
		cout << "EVENT_LBUTTONUP : " << x << ", " << y << endl;
		break;

	case EVENT_MOUSEMOVE :  // ���콺�� �����̸�
		if (flags & EVENT_FLAG_LBUTTON) {  // ���ʹ�ư�� ������ �ִ� �����̸�
			line(img, ptOld, Point(x, y), Scalar(0, 255, 255), 2);  // ���� �׸���
			imshow("img", img);
			ptOld = Point(x, y);  // ������ġ�� ����
		}
		break;
	default :
		break;
	}
}


int main(void) {
	img = imread("./images/lenna.bmp");

	if (img.empty()) {
		cerr << "Image load failed" << endl;
		return -1;
	}

	namedWindow("img");
	// setMouseCallback�Լ��� ����ϱ� ���� â�� �����Ǿ� �־����
	setMouseCallback("img", on_mouse); 
	
	imshow("img", img);
	waitKey();

	return 0;
}