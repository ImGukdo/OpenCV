#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

// 마우스를 이용해 숫자 그리기
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

// 학습된 모델파일을 이용한 필기체 숫자 인식
int main(void) {
	// mnist_cnn.pb(텐서플로우 모델 파일)을 이용하여 Net 객체 생성
	Net net = readNet("mnist_cnn.pb");
	// Net 객체가 정상적으로 생성되지 않으면 종료
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
			// 4차원 1X1X28X28크기의 블롭 생성
			Mat inputBlob = blobFromImages(img, 1 / 255.f, Size(28, 28));
			// 네트워크 입력으로 설정
			net.setInput(inputBlob);
			// 네트워크를 순방향으로 실행
			Mat prob = net.forward();
			// prob 행렬에서 최댓값과 최댓값 위치를 찾음
			double maxVal;  // 최대 확률값
			Point maxLoc;  // maxLoc.x 가장확률이 높은 숫자
			minMaxLoc(prob, 0, &maxVal, 0, &maxLoc);
			int digit = maxLoc.x;

			cout << digit << " (" << maxVal * 100 << "%)" << endl;

			img.setTo(Scalar(0));
			imshow("img", img);
		}
	}
	return 0;
}