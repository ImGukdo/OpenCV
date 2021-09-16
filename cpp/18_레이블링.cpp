#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


// �⺻ ���̺�
void labeling_basic() {
	// �������� ����
	uchar data[] = {
		0, 0, 1, 1, 0, 0, 0, 0,
		1, 1, 1, 1, 0, 0, 1, 0,
		1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 0, 0, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
	Mat src = Mat(8, 8, CV_8UC1, data) * 255;

	// ���̺�� ����, ���̺� ���� ��ȯ
	Mat labels;
	int cnt = connectedComponents(src, labels);

	cout << "src:\n" << src << endl;
	cout << "labels:\n" << labels << endl;
	cout << "number of labels: " << cnt << endl;
}


// ���̺��� �̿��Ͽ� ��ü�� �ٿ���ڽ� �׸���
void labeling_stats() {
	Mat src = imread("./images/keyboard.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ���� �˰������� ���� ����ȭ
	Mat bin;
	threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
	// ���̺� ����, �� ��ü�� ������� ����
	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	// 3ä�� �÷� ������������ ��ȯ
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);
	// ��濵���� �����ϰ� ��� ��ü ������ ���ؼ�
	for (int i = 1; i < cnt; ++i) {
		int* p = stats.ptr<int>(i);  // 1����� 

		if (p[4] < 20) continue;  // ��ü �ȼ� ������ 2���� ������ �������� ����
		// ����� ��ü�� ����� �簢������ ����
		rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 255, 255), 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}


// �ܰ��� ����� �׸���
void contours_basic() {
	Mat src = imread("./images/contours.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ��� �ܰ��� ����
	vector<vector<Point> > contours;
	findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);

	// 3ä�� �������� ��ȯ
	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// ��� �ܰ����� ������ �������� �׸�
	for (int i = 0; i < contours.size(); ++i) {
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, c, 2);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();

}

// ���������� ����ϴ� �ܰ��� ����� �׸���
void contours_hier() {
	Mat src = imread("./images/contours.bmp", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// �ܰ��� ����
	vector<vector<Point> > contours;  // �ܰ��� �ȼ��� �޾ƿ� ����
	vector<Vec4i> hierarchy;  // ���������� �޾ƿ� ����
	findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	// 0�� �ܰ������� ���� �ܰ������� �ݺ��ϸ鼭 ���� �ܰ��� ��ȣ�� -1�� �ɶ�����
	for (int idx = 0; idx >= 0; idx = hierarchy[idx][0]) {
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, idx, c, -1, LINE_8, hierarchy);
	}

	imshow("src", src);
	imshow("dst", dst);

	waitKey();
	destroyAllWindows();
}


// �ٰ��� ���� �� �ν�
void setLabel(Mat& img, const vector<Point>& pts, const String& label) {
	// �ܰ��� �ֺ��� �ٿ���ڽ��� �׸�
	Rect rc = boundingRect(pts);
	rectangle(img, rc, Scalar(0, 0, 255), 1);
	// �簢���� �������� ���� �������� �ؽ�Ʈ�� ���
	putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
}

void boudingPoly() {
	Mat img = imread("./images/polygon.bmp", IMREAD_COLOR);

	if (img.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);

	// ���� �˰������� �ڵ� ����ȭ
	Mat bin;
	threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	// �ܰ��� ����
	vector<vector<Point> > contours;
	findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// ��� �ܰ��� ��ǥ�� Ž���ϸ鼭
	for (vector<Point>& pts : contours) {
		// ������ 400���� ������ �������� ����
		if (contourArea(pts) < 400)
			continue;
		// �ܰ����� �ٻ�ȭ �Ͽ� ����
		vector<Point> approx;
		approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);
		// approx �� ����� ���� ����
		int vtc = (int)approx.size();
		// ���� ������ 3�̸� TRI
		if (vtc == 3) {
			setLabel(img, pts, "TRI");
		}
		// ���� ������ 4�̸� RECT
		else if (vtc == 4) {
			setLabel(img, pts, "RECT");
		}
		// ���� ������ 4���� ũ�� 
		else if (vtc > 4) {
			// ��ü�� ���� �� ���� ���� ���
			double len = arcLength(pts, true);
			double area = contourArea(pts);
			double ratio = 4 * CV_PI * area / (len * len);
			// ������ 0.8���� ũ�� CIR�� �Ǵ�
			if (ratio > 0.8) {
				setLabel(img, pts, "CIR");
			}
		}
	}
	
	imshow("img", img);
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// labeling_basic();
	// labeling_stats();
	// contours_basic();
	// contours_hier();
	boudingPoly();

	return 0;
}