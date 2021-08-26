#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	Mat img;  // Mat ��ü ����
	img = imread("./images/lenna.bmp", IMREAD_COLOR);  // �̹����� �÷����� �ҷ��´�

	if (img.empty()) {  // �̹��� ����� �ҷ����� ���� ��� ���� ��� �� ���α׷� ����
		cerr << "Image load failed" << endl;
		return -1;
	}

	namedWindow("img");  // "img"��� �̸��� ���â�� ����
	imshow("img", img);  // "img"â�� img�� �����͸� ���
	waitKey();  // ������ �ð� ���� Ű�Է��� ���, imshow()�Լ� ���Ŀ� waitKey()�Լ��� ȣ���ؾ� ȭ�� �׸��� �̺�Ʈ�� ����
	destroyAllWindows();  // �������â�� ���� �ݴ´�
	return 0;
}