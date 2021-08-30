#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	Point pt1;  // Point ��ü ����
	pt1.x = 5, pt1.y = 10;  // ��ǥ �缳��

	Point pt2(10, 30);  // ��ü�� �����ϸ鼭 �ʱ�ȭ

	Point pt3 = pt1 + pt2;  // ��ü���� ���� ����
	Point pt4 = pt1 * 2;
	bool b1 = (pt1 == pt2);

	Size sz1, sz2(10, 20);  // Size ��ü ����
	sz1.width = 5, sz1.height = 10;  // sz1�� ������� ����

	Size sz3 = sz1 + sz2;  // ��ü���� ���� ����
	Size sz4 = sz1 * 2;
	int area = sz4.area();  // �簢���� ������ ��ȯ
	
	Rect rc1;  // Rect ��ü ����, [0 x 0 from (0, 0)]
	Rect rc2(10, 10, 60, 40);  // [60 x 40 from (10, 10)]
	// ��ü���� ��Ģ���� ����
	Rect rc3 = rc1 + Size(50, 40);  // [50 x 40 from (0, 0)]
	Rect rc4 = rc2 + Point(10, 10);  // [60 x 40 from (20, 20)]

	Rect rc5 = rc3 & rc4;  // �� �簢���� �����ϴ� �簢�� ����, [30 x 20 from (20, 20)]
	Rect rc6 = rc3 | rc4; // �� �簢���� ��� �����ϴ� �ּ� ũ���� �簢��, [80 x 60 from (0, 0)]

	// RotatedRect Ŭ������ ��� ������ float �ڷ����� ���
	RotatedRect rr1(Point2f(40, 30), Size2f(40, 20), 30.f);  // RotatedRect ��ü ����
	Point2f pts[4];  
	rr1.points(pts);  // �簢���� �� �������� ������, ���� �ϴ� ���������� �ð��������

	Rect br = rr1.boundingRect();  // ȸ���� �簢���� �ٿ�� �ڽ�

	Range r1(0, 10);  // 0~9������ ������ ǥ���ϴ� ��ü

	// String Ŭ������ string Ŭ������ ����, �ڵ� ȣȯ���� ���� ������
	String str1 = "Hello";
	String str2 = "world";
	String str3 = str1 + " " + str2;


	return 0;
}