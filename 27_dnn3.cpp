#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
using namespace cv::dnn;

const String model = "opencv_face_detector_uint8.pb";  // 모델 파일
const String config = "opencv_face_detector.pbtxt";  // 구성파일 

int main(void) {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return -1;
	}
	
	// 모델파일과 구성파일을 이용하여 Net 객체 생성
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

		// frame 영상을 이용하여 네트워크 입력 블롭을 설정하고 네트워크 실행결과를 res행렬에 저장
		Mat blob = blobFromImage(frame, 1, Size(300, 300), Scalar(104, 177, 123));
		net.setInput(blob);
		Mat res = net.forward();

		Mat detect(res.size[2], res.size[3], CV_32FC1, res.ptr<float>());

		for (int i = 0; i < detect.rows; i++) {
			float confidence = detect.at<float>(i, 2);
			if (confidence < 0.5)  // 결과 신뢰도 값이 0.5보다 작으면 무시
				break;
			// 얼굴 검출 사각형 영역을 계산
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