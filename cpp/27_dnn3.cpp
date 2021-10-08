#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

const String model = "opencv_face_detector_uint8.pb";  // �� ����
const String config = "opencv_face_detector.pbtxt";  // �������� 

int main(void) {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return -1;
	}
	
	// �����ϰ� ���������� �̿��Ͽ� Net ��ü ����
	Net net = readNet(model, config);
	if (net.empty()) {
		cerr << "Net open failed" << endl;
		return -1;
	}

	Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty())
			break;

		// frame ������ �̿��Ͽ� ��Ʈ��ũ �Է� ����� �����ϰ� ��Ʈ��ũ �������� res��Ŀ� ����
		Mat blob = blobFromImage(frame, 1, Size(300, 300), Scalar(104, 177, 123));
		net.setInput(blob);
		Mat res = net.forward();

		Mat detect(res.size[2], res.size[3], CV_32FC1, res.ptr<float>());

		for (int i = 0; i < detect.rows; i++) {
			float confidence = detect.at<float>(i, 2);
			if (confidence < 0.5)  // ��� �ŷڵ� ���� 0.5���� ������ ����
				break;
			// �� ���� �簢�� ������ ���
			int x1 = cvRound(detect.at<float>(i, 3) * frame.cols);
			int y1 = cvRound(detect.at<float>(i, 4) * frame.rows);
			int x2 = cvRound(detect.at<float>(i, 5) * frame.cols);
			int y2 = cvRound(detect.at<float>(i, 6) * frame.rows);

			rectangle(frame, Rect(Point(x1, y1), Point(x2, y2)), Scalar(0, 255, 0));

			String label = format("Face: %4.3f", confidence);
			putText(frame, label, Point(x1, y1 - 1), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0));
		}

		imshow("frame", frame);
		if (waitKey(10) == 27)
			break;
	}
	destroyAllWindows();

	return 0;
}