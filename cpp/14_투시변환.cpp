#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat src;  // 입력 영상
Point2f srcQuad[4], dstQuad[4];  // 입력영상과 출력영상의 네 점의 좌표를 저장할 배열

void on_mouse(int event, int x, int y, int flags, void* userdata) {
	static int cnt = 0;  // 마우스 왼쪽 버튼이 눌린 횟수를 static 변수로 저장

	// 마우스 이벤트 중 왼쪽 버튼이 눌려지는 이벤트
	if (event == EVENT_LBUTTONDOWN) {
		if (cnt < 4) {  // 왼쪽 버튼이 눌려진 횟수를 4번 입력받음
			// 왼쪽 버튼이 눌려진 위치는 좌측상단, 우측상단, 우측하단, 좌측하단 순이여야 올바른 출력영상을 얻을 수 있음
			srcQuad[cnt++] = Point2f(x, y);  // 왼쪽 버튼이 눌려진 위치를 저장
			// 왼쪽 버튼을 누른 위치를 빨간색 원으로 표시
			circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
			imshow("src", src);
			// 4개의 위치를 입력받으면
			if (cnt == 4) {
				int w = 200, h = 300;  // 카드의 크기, 보통 가로와 세로 비율이 2:3이므로 
				// 출력영상의 네점
				dstQuad[0] = Point2f(0, 0);
				dstQuad[1] = Point2f(w - 1, 0);
				dstQuad[2] = Point2f(w - 1, h - 1);
				dstQuad[3] = Point2f(0, h - 1);
				// 3X3 투시 변환 행렬
				Mat pers = getPerspectiveTransform(srcQuad, dstQuad);
				// 투시변환을 수행하여 wXh 크기의 결과 영상을 출력
				Mat dst;
				warpPerspective(src, dst, pers, Size(w, h));
				imshow("dst", dst);
			}
		}
	}
}

int main(void) {
	src = imread("./images/card.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return -1;
	}

	namedWindow("src");
	setMouseCallback("src", on_mouse);  // 마우스 콜백 함수 등록

	imshow("src", src);
	waitKey();
	destroyAllWindows();
	return 0;
}