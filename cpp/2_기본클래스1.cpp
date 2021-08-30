#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(void) {
	Point pt1;  // Point 객체 생성
	pt1.x = 5, pt1.y = 10;  // 좌표 재설정

	Point pt2(10, 30);  // 객체를 생성하면서 초기화

	Point pt3 = pt1 + pt2;  // 객체끼리 연산 가능
	Point pt4 = pt1 * 2;
	bool b1 = (pt1 == pt2);

	Size sz1, sz2(10, 20);  // Size 객체 생성
	sz1.width = 5, sz1.height = 10;  // sz1의 멤버값을 변경

	Size sz3 = sz1 + sz2;  // 객체끼리 연산 가능
	Size sz4 = sz1 * 2;
	int area = sz4.area();  // 사각형의 면적을 반환
	
	Rect rc1;  // Rect 객체 생성, [0 x 0 from (0, 0)]
	Rect rc2(10, 10, 60, 40);  // [60 x 40 from (10, 10)]
	// 객체끼리 사칙연산 가능
	Rect rc3 = rc1 + Size(50, 40);  // [50 x 40 from (0, 0)]
	Rect rc4 = rc2 + Point(10, 10);  // [60 x 40 from (20, 20)]

	Rect rc5 = rc3 & rc4;  // 두 사각형이 교차하는 사각형 영역, [30 x 20 from (20, 20)]
	Rect rc6 = rc3 | rc4; // 두 사각형을 모두 포함하는 최소 크기의 사각형, [80 x 60 from (0, 0)]

	// RotatedRect 클래스는 모든 정보를 float 자료형을 사용
	RotatedRect rr1(Point2f(40, 30), Size2f(40, 20), 30.f);  // RotatedRect 객체 생성
	Point2f pts[4];  
	rr1.points(pts);  // 사각형의 네 꼭지점을 가져옴, 좌측 하단 꼭지점부터 시계방향으로

	Rect br = rr1.boundingRect();  // 회전된 사각형의 바운딩 박스

	Range r1(0, 10);  // 0~9까지의 범위를 표현하는 객체

	// String 클래스는 string 클래스와 같음, 코드 호환성을 위해 재정의
	String str1 = "Hello";
	String str2 = "world";
	String str3 = str1 + " " + str2;


	return 0;
}