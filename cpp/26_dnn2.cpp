#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::dnn;

int main(void) {
	Mat img = imread("./images/space_shuttle.jpg");

	if (img.empty()) {
		cerr << "Image load failed" << endl;
		return -1;
	}
	// bvlc_googlenet.caffemodel 파일과 deploy.prototxt 파일을 이용하여 Net 객체 생성
	Net net = readNet("bvlc_googlenet.caffemodel", "deploy.prototxt");

	if(net.empty()){
		cerr << "Network load failed" << endl;
		return -1;
	}
	
	// classification_classes_ILSVRC2012.txt 파일을 한줄씩 읽어서 패당 문자열을 백터에 저장
	ifstream fp("classification_classes_ILSVRC2012.txt");
	if (!fp.is_open()) {
		cerr << "Class file load failed" << endl;
		return -1;
	}
	vector<String> classNames;
	string name;
	while (!fp.eof()) {
		getline(fp, name);
		if (name.length())
			classNames.push_back(name);
	}
	fp.close();

	// 입력영상 img를 이용하여 입력 블롭을 생성하고 네트워크를 실행
	Mat inputBlob = blobFromImage(img, 1, Size(224, 224), Scalar(104, 117, 123));
	net.setInput(inputBlob);
	Mat prob = net.forward();

	double maxVal;  // 최대 확률값 
	Point maxLoc;  // 인식된 영상 클래스 번호
	minMaxLoc(prob, 0, &maxVal, 0, &maxLoc);

	String str = format("%s (%4.2lf%%)", classNames[maxLoc.x].c_str(), maxVal * 100);
	putText(img, str, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255));
	imshow("img", img);
	waitKey();

	return 0;
}