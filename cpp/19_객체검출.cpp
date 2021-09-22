#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// 템플릿 매칭
void template_matching() {
	Mat img = imread("./images/circuit.bmp");
	Mat templ = imread("./images/crystal.bmp");

	if (img.empty() || templ.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 영상에 조명과 잡음을 추가
	img += Scalar(50, 50, 50);  // 밝기 50 증가
	Mat noise(img.size(), CV_32SC3);
	randn(noise, 0, 10);
	add(img, noise, img, Mat(), CV_8UC3);  // 표준편차가 10인 가우시안 잡음 추가

	Mat res, res_norm;  
	// 정규화된 상관계수 매칭 방법으 사용하여 템플릿 매칭 수행
	matchTemplate(img, templ, res, TM_CCOEFF_NORMED);
	// 템플릿 매칭 결과 행렬의 모든 원소값을 0~255 사이로 정규화 하고 CV_8U 타입으로 변환
	normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

	// 최댓값과 최대값의 위치 찾기
	double maxv;
	Point maxloc;
	minMaxLoc(res, 0, &maxv, 0, &maxloc);
	cout << "maxv : " << maxv << endl;

	// img 영상에서 템플릿 매칭으로 찾은 위치를 빨간색 사각형으로 표시
	rectangle(img, Rect(maxloc, templ.size()), Scalar(0, 0, 255), 2);
	imshow("img", img);
	imshow("templ", templ);
	waitKey();
	destroyAllWindows();
}


// 얼굴 검출
void detect_face() {
	Mat src = imread("./images/kids.png");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	
	// CascadeClassifier 객체를 생성하고 haarcascade_frontalface_default.xml 파일을 불러옴
	CascadeClassifier classifier("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml");
	if (classifier.empty()) {
		cerr << "XML load failed" << endl;
		return;
	}
	// src 영상에서 얼굴을 검출하여 사각형 정보를 저장
	vector<Rect> faces;
	classifier.detectMultiScale(src, faces);
	// 검출된 얼굴을 보라색 사각형으로 그림
	for (Rect rc : faces) {
		rectangle(src, rc, Scalar(255, 0, 255), 2);
	}

	imshow("src", src);
	waitKey();
	destroyAllWindows();
}


// 눈 검출
void detect_eyes() {
	Mat src = imread("./images/kids.png");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// CascadeClassifier 객체를 생성하고 xml 파일을 불러온다
	CascadeClassifier face_classifier("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml");
	CascadeClassifier eye_classifier("C:/opencv/build/etc/haarcascades/haarcascade_eye.xml");

	if (face_classifier.empty() || eye_classifier.empty()) {
		cerr << "xml load failed" << endl;
		return;
	}
	// src 영상에서 얼굴을 검출
	vector<Rect> faces;
	face_classifier.detectMultiScale(src, faces);
	// 검출된 얼굴영역에서 눈 검출
	for (Rect face : faces) {
		rectangle(src, face, Scalar(255, 0, 255), 2);

		Mat faceRoi = src(face);
		vector<Rect> eyes;
		eye_classifier.detectMultiScale(faceRoi, eyes);

		for (Rect eye : eyes) {
			Point center(eye.x + eye.width / 2, eye.y + eye.height / 2);
			circle(faceRoi, center, eye.width / 2, Scalar(255, 0, 0), 2, LINE_AA);
		}
	}
	imshow("src", src);
	waitKey();
	destroyAllWindows();
}


// 보행자 검출
void detect_people() {
	// vtest.avi. 파일을 불러옴
	VideoCapture cap("./images/vtest.avi");

	if (!cap.isOpened()) {
		cerr << "Video open failed" << endl;
		return;
	}
	
	HOGDescriptor hog;  // HOGDescriptor 객체 생성
	// 보행자 검출을 위한 분류기 계수를 SVM 분류기 함수에 등록
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty())
			break;
		// 매 프레임마다 보행자 검출 수행
		vector<Rect> detected;
		hog.detectMultiScale(frame, detected);
		// 검출된 사각형 정보를 그림
		for (Rect r : detected) {
			Scalar c = Scalar(rand() % 256, rand() % 256, rand() % 256);
			rectangle(frame, r, c, 3);
		}
		imshow("frame", frame);

		if (waitKey(10) == 27)
			break;
	}
}


// QR코드 검출 및 해석
void decode_qrcode() {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return;
	}

	QRCodeDetector detector;  // QRCodeDetector 객체 생성

	Mat frame;

	while (true) {
		cap >> frame;
		if (frame.empty())
			break;

		vector<Point> points;

		// QR코드 검출 및 해석
		String info = detector.detectAndDecode(frame, points);

		 // QR코드를 빨간색 사각형으로 그리고 해석된 문자열을 출력 
		if (!info.empty()) {
			polylines(frame, points, true, Scalar(0, 0, 255), 2);
			putText(frame, info, Point(10, 30), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));
		}

		imshow("frame", frame);
		if (waitKey(1) == 27)
			break;
	}
}



int main(void) {
	// template_matching();
	// detect_face();
	// detect_eyes();
	// detect_people();
	decode_qrcode();
	return 0;
}
