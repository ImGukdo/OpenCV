#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void camera_in() {
	VideoCapture cap(0);  // VideoCapture ��ü�� ���� ī�޶� �����Ѵ�.

	if (!cap.isOpened()) {  // ī�޶� ���µƴ��� Ȯ��
		cerr << "Camera open failed" << endl;
		return;
	}

	cout << "width : " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;  // ������ ���� ũ�� ���
	cout << "height : " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;  // ������ ���� ũ�� ���

	Mat frame, inversed;  // Mat ��ü ����
	while (true) {
		cap >> frame;  // ī�޶�� ���������� �޾ƿ� frame ������ ����
		if (frame.empty())  // �޾ƿ� �������� ������ Ż��
			break;
		inversed = ~frame;  // ���� �������� �����ؼ� inversed ������ ����

		imshow("frame", frame);  // ���
		imshow("inversed", inversed);  // ���

		if (waitKey(10) == 27)  // 10ms ���� �Է´��, esc �Է��� ������ Ż��
			break;
	}
	destroyAllWindows();  // �����ִ� ��� â �ݱ�
}

void video_in() {
	VideoCapture cap("./images/stopwatch.avi");  // ������ ������ �ҷ��ͼ� VideoCapture ��ü�� ����

	if (!cap.isOpened()) {  // �������� ����� �ҷ����� ���ϸ� ����
		cerr << "Video open failed" << endl;
		return;
	}

	cout << "width : " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;  // ���� ũ��
	cout << "height : " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;  // ���� ũ��
	cout << "frame count : " << cvRound(cap.get(CAP_PROP_FRAME_COUNT)) << endl;  // �� �����Ӽ�

	double fps = cap.get(CAP_PROP_FPS);  // fps ��
	cout << "fps : " << fps << endl;
	int delay = cvRound(1000 / fps);  // delay�� ���

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
	VideoWriter outputVideo("./images/output.avi", fourcc, fps, Size(w, h));  // VideoWriter ��ü ����

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
		outputVideo << inversed;  // outputVideo�� inversed �������� �����Ѵ�.

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