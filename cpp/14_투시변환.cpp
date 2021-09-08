#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat src;  // �Է� ����
Point2f srcQuad[4], dstQuad[4];  // �Է¿���� ��¿����� �� ���� ��ǥ�� ������ �迭

void on_mouse(int event, int x, int y, int flags, void* userdata) {
	static int cnt = 0;  // ���콺 ���� ��ư�� ���� Ƚ���� static ������ ����

	// ���콺 �̺�Ʈ �� ���� ��ư�� �������� �̺�Ʈ
	if (event == EVENT_LBUTTONDOWN) {
		if (cnt < 4) {  // ���� ��ư�� ������ Ƚ���� 4�� �Է¹���
			// ���� ��ư�� ������ ��ġ�� �������, �������, �����ϴ�, �����ϴ� ���̿��� �ùٸ� ��¿����� ���� �� ����
			srcQuad[cnt++] = Point2f(x, y);  // ���� ��ư�� ������ ��ġ�� ����
			// ���� ��ư�� ���� ��ġ�� ������ ������ ǥ��
			circle(src, Point(x, y), 5, Scalar(0, 0, 255), -1);
			imshow("src", src);
			// 4���� ��ġ�� �Է¹�����
			if (cnt == 4) {
				int w = 200, h = 300;  // ī���� ũ��, ���� ���ο� ���� ������ 2:3�̹Ƿ� 
				// ��¿����� ����
				dstQuad[0] = Point2f(0, 0);
				dstQuad[1] = Point2f(w - 1, 0);
				dstQuad[2] = Point2f(w - 1, h - 1);
				dstQuad[3] = Point2f(0, h - 1);
				// 3X3 ���� ��ȯ ���
				Mat pers = getPerspectiveTransform(srcQuad, dstQuad);
				// ���ú�ȯ�� �����Ͽ� wXh ũ���� ��� ������ ���
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
	setMouseCallback("src", on_mouse);  // ���콺 �ݹ� �Լ� ���

	imshow("src", src);
	waitKey();
	destroyAllWindows();
	return 0;
}