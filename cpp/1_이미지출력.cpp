#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	Mat img;  // Mat 객체 생성
	img = imread("./images/lenna.bmp", IMREAD_COLOR);  // 이미지를 컬러모드로 불러온다

	if (img.empty()) {  // 이미지 제대로 불러오지 못한 경우 에러 출력 후 프로그램 종료
		cerr << "Image load failed" << endl;
		return -1;
	}

	namedWindow("img");  // "img"라는 이름의 출력창을 생성
	imshow("img", img);  // "img"창에 img를 데이터를 출력
	waitKey();  // 지정한 시간 동안 키입력을 대기, imshow()함수 이후에 waitKey()함수를 호출해야 화면 그리기 이벤트가 동작
	destroyAllWindows();  // 영상출력창을 전부 닫는다
	return 0;
}