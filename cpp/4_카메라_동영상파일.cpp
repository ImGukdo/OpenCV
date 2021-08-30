#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void camera_in() {
	VideoCapture cap(0);  // VideoCapture 객체를 만들어서 카메라를 오픈한다.

	if (!cap.isOpened()) {  // 카메라가 오픈됐는지 확인
		cerr << "Camera open failed" << endl;
		return;
	}

	cout << "width : " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;  // 프레임 가로 크기 출력
	cout << "height : " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;  // 프레임 세로 크기 출력

	Mat frame, inversed;  // Mat 객체 생성
	while (true) {
		cap >> frame;  // 카메라로 한프레임을 받아와 frame 변수에 저장
		if (frame.empty())  // 받아온 프레임이 없으면 탈출
			break;
		inversed = ~frame;  // 현재 프레임을 반전해서 inversed 변수에 저장

		imshow("frame", frame);  // 출력
		imshow("inversed", inversed);  // 출력

		if (waitKey(10) == 27)  // 10ms 동안 입력대기, esc 입력을 받으면 탈출
			break;
	}
	destroyAllWindows();  // 열려있는 모든 창 닫기
}

void video_in() {
	VideoCapture cap("./images/stopwatch.avi");  // 동영상 파일을 불러와서 VideoCapture 객체를 생성

	if (!cap.isOpened()) {  // 동영상을 제대로 불러오지 못하면 리턴
		cerr << "Video open failed" << endl;
		return;
	}

	cout << "width : " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;  // 가로 크기
	cout << "height : " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;  // 세로 크기
	cout << "frame count : " << cvRound(cap.get(CAP_PROP_FRAME_COUNT)) << endl;  // 총 프레임수

	double fps = cap.get(CAP_PROP_FPS);  // fps 값
	cout << "fps : " << fps << endl;
	int delay = cvRound(1000 / fps);  // delay를 계산

	Mat frame, inversed;
	while (true) {
		cap >> frame;
		if (frame.empty())
			break;
		inversed = ~frame;

		imshow("frame", frame);
		imshow("inversed", inversed);

		if (waitKey(delay) == 27)
			break;
	}
	destroyAllWindows();
}

void camera_in_video_out() {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return;
	}

	int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
	double fps = cap.get(CAP_PROP_FPS);
	int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
	int delay = cvRound(1000 / fps);
	VideoWriter outputVideo("./images/output.avi", fourcc, fps, Size(w, h));  // VideoWriter 객체 생성

	if (!outputVideo.isOpened()) {
		cerr << "File open failed" << endl;
		return;
	}

	Mat frame, inversed;
	while (true) {
		cap >> frame;
		if (frame.empty())
			break;
		inversed = ~frame;
		outputVideo << inversed;  // outputVideo에 inversed 프레임을 저장한다.

		imshow("frame", frame);
		imshow("inversed", inversed);

		if (waitKey(delay) == 27)
			break;
	}
	destroyAllWindows();
}


int main(void) {
	// camera_in();
	// video_in();
	camera_in_video_out();
	return 0;
}