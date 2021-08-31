#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void on_level_change(int pos, void* userdata) {
	Mat img = *(Mat*)userdata;  // userdata로 받은 데이터를 Mat타입으로 변환

	img.setTo(pos * 16);  // 트랙바 위치값에 16을 곱한 값을 밝기로 넣어준다.
	imshow("img", img);
}

int main(void) {
	Mat img = Mat::zeros(400, 400, CV_8UC1);

	namedWindow("img");
	createTrackbar("level", "img", 0, 16, on_level_change, (void*)&img);  // 트랙바 생성

	imshow("img", img);
	waitKey();

	return 0;
}