#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat img;
Point ptOld;

void on_mouse(int event, int x, int y, int flags, void*) {
	switch (event) {
	case EVENT_LBUTTONDOWN :  // 마우스 왼쪽 버튼을 누르면
		ptOld = Point(x, y);
		cout << "EVENT_LBUTTONDOWN : " << x << ", " << y << endl;
		break;

	case EVENT_LBUTTONUP :  // 마우스 왼쪽 버튼을 떼면
		cout << "EVENT_LBUTTONUP : " << x << ", " << y << endl;
		break;

	case EVENT_MOUSEMOVE :  // 마우스가 움직이면
		if (flags & EVENT_FLAG_LBUTTON) {  // 왼쪽버튼이 눌려져 있는 상태이면
			line(img, ptOld, Point(x, y), Scalar(0, 255, 255), 2);  // 선을 그린다
			imshow("img", img);
			ptOld = Point(x, y);  // 현재위치를 저장
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
	// setMouseCallback함수를 출력하기 전에 창이 생성되어 있어야함
	setMouseCallback("img", on_mouse); 
	
	imshow("img", img);
	waitKey();

	return 0;
}