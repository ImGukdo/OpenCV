#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// ���ø� ��Ī
void template_matching() {
	Mat img = imread("./images/circuit.bmp");
	Mat templ = imread("./images/crystal.bmp");

	if (img.empty() || templ.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ���� ����� ������ �߰�
	img += Scalar(50, 50, 50);  // ��� 50 ����
	Mat noise(img.size(), CV_32SC3);
	randn(noise, 0, 10);
	add(img, noise, img, Mat(), CV_8UC3);  // ǥ�������� 10�� ����þ� ���� �߰�

	Mat res, res_norm;  
	// ����ȭ�� ������ ��Ī ����� ����Ͽ� ���ø� ��Ī ����
	matchTemplate(img, templ, res, TM_CCOEFF_NORMED);
	// ���ø� ��Ī ��� ����� ��� ���Ұ��� 0~255 ���̷� ����ȭ �ϰ� CV_8U Ÿ������ ��ȯ
	normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U);

	// �ִ񰪰� �ִ밪�� ��ġ ã��
	double maxv;
	Point maxloc;
	minMaxLoc(res, 0, &maxv, 0, &maxloc);
	cout << "maxv : " << maxv << endl;

	// img ���󿡼� ���ø� ��Ī���� ã�� ��ġ�� ������ �簢������ ǥ��
	rectangle(img, Rect(maxloc, templ.size()), Scalar(0, 0, 255), 2);
	imshow("img", img);
	imshow("templ", templ);
	waitKey();
	destroyAllWindows();
}


// �� ����
void detect_face() {
	Mat src = imread("./images/kids.png");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	
	// CascadeClassifier ��ü�� �����ϰ� haarcascade_frontalface_default.xml ������ �ҷ���
	CascadeClassifier classifier("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml");
	if (classifier.empty()) {
		cerr << "XML load failed" << endl;
		return;
	}
	// src ���󿡼� ���� �����Ͽ� �簢�� ������ ����
	vector<Rect> faces;
	classifier.detectMultiScale(src, faces);
	// ����� ���� ����� �簢������ �׸�
	for (Rect rc : faces) {
		rectangle(src, rc, Scalar(255, 0, 255), 2);
	}

	imshow("src", src);
	waitKey();
	destroyAllWindows();
}


// �� ����
void detect_eyes() {
	Mat src = imread("./images/kids.png");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// CascadeClassifier ��ü�� �����ϰ� xml ������ �ҷ��´�
	CascadeClassifier face_classifier("C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml");
	CascadeClassifier eye_classifier("C:/opencv/build/etc/haarcascades/haarcascade_eye.xml");

	if (face_classifier.empty() || eye_classifier.empty()) {
		cerr << "xml load failed" << endl;
		return;
	}
	// src ���󿡼� ���� ����
	vector<Rect> faces;
	face_classifier.detectMultiScale(src, faces);
	// ����� �󱼿������� �� ����
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


// ������ ����
void detect_people() {
	// vtest.avi. ������ �ҷ���
	VideoCapture cap("./images/vtest.avi");

	if (!cap.isOpened()) {
		cerr << "Video open failed" << endl;
		return;
	}
	
	HOGDescriptor hog;  // HOGDescriptor ��ü ����
	// ������ ������ ���� �з��� ����� SVM �з��� �Լ��� ���
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty())
			break;
		// �� �����Ӹ��� ������ ���� ����
		vector<Rect> detected;
		hog.detectMultiScale(frame, detected);
		// ����� �簢�� ������ �׸�
		for (Rect r : detected) {
			Scalar c = Scalar(rand() % 256, rand() % 256, rand() % 256);
			rectangle(frame, r, c, 3);
		}
		imshow("frame", frame);

		if (waitKey(10) == 27)
			break;
	}
}


void decode_qrcode() {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return;
	}

	QRCodeDetector detector;  // QRCodeDetector ��ü ����

	Mat frame;

	while (true) {
		cap >> frame;
		if (frame.empty())
			break;

		vector<Point> points;

		// QR�ڵ� ���� �� �ؼ�
		String info = detector.detectAndDecode(frame, points);

		 // QR�ڵ带 ������ �簢������ �׸��� �ؼ��� ���ڿ��� ��� 
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