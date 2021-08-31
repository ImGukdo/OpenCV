#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void on_level_change(int pos, void* userdata) {
	Mat img = *(Mat*)userdata;  // userdata�� ���� �����͸� MatŸ������ ��ȯ

	img.setTo(pos * 16);  // Ʈ���� ��ġ���� 16�� ���� ���� ���� �־��ش�.
	imshow("img", img);
}

int main(void) {
	Mat img = Mat::zeros(400, 400, CV_8UC1);

	namedWindow("img");
	createTrackbar("level", "img", 0, 16, on_level_change, (void*)&img);  // Ʈ���� ����

	imshow("img", img);
	waitKey();

	return 0;
}