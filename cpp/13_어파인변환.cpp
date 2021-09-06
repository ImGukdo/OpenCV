#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


void affine_transform() {
	Mat src = imread("./images/tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
			return;
	}

	// 입력 영상의 세 점, 출력영상의 세 점을 배열로 선언
	Point2f srcPts[3], dstPts[3];
	srcPts[0] = Point2f(0, 0);
	srcPts[1] = Point2f(src.cols - 1, 0); 
	srcPts[2] = Point2f(src.cols - 1, src.rows - 1); 
	dstPts[0] = Point2f(50, 50); 
	dstPts[1] = Point2f(src.cols - 100, 100); 
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50); 

	// 2X3 어파인 변환행렬을 M에 저장
	Mat M = getAffineTransform(srcPts, dstPts);

	// warpAffine 함수를 이용해 src 영상을 어파인 변환하여 dst에 저장
	Mat dst;
	warpAffine(src, dst, M, Size());

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void affine_translation() {
	Mat src = imread("./images/tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 가로로 150, 세로로 100 이도아는 어파인 변환 행렬
	Mat M = Mat_<double>({ 2, 3 }, { 1, 0, 150, 0, 1, 100 });

	Mat dst;
	warpAffine(src, dst, M, Size());

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void affine_shear() {
	Mat src = imread("./images/tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 가로 방향으로 밀림 정도를 0.3으로 설정한 전단 변환 행렬을 생성
	double mx = 0.3;
	Mat M = Mat_<double>({ 2, 3 }, { 1, mx, 0, 0, 1, 0 });
	Mat dst;
	// 전단변환에 의해 영상의 일부가 잘리지 않도록 결과 영상의 가로크기를 늘려준다.
	warpAffine(src, dst, M, Size(cvRound(src.cols + src.rows * mx), src.rows));

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}

void affine_scale() {
	Mat src = imread("./images/rose.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 영상 확대 변환
	Mat dst1, dst2, dst3, dst4;
	resize(src, dst1, Size(), 4, 4, INTER_NEAREST); // 최근방 이웃 보간법, 픽셀 자체가 커진거처럼 보임
	resize(src, dst2, Size(1920, 1280));  // 양선형 보간법, 기본값
	resize(src, dst3, Size(1920, 1280), 0, 0, INTER_CUBIC);  // 3차 회선 보간법, 확대영상에서 화질이 좋음, 연산 속도가 느림
	resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LANCZOS4);  // 란초스 보간법, 확대영상에서 화질이 좋음, 연산 속도가 느림

	imshow("src", src);
	imshow("dst1", dst1(Rect(400, 500, 400, 400)));
	imshow("dst2", dst2(Rect(400, 500, 400, 400)));
	imshow("dst3", dst3(Rect(400, 500, 400, 400)));
	imshow("dst4", dst4(Rect(400, 500, 400, 400)));
	waitKey();
	destroyAllWindows();
}

void affine_rotation() {
	Mat src = imread("./images/tekapo.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	imshow("src", src);
	Point2f cp(src.cols / 2.f, src.rows / 2.f); // 영상의 중심 좌표
	// cp 좌표를 기준으로 반시계반향으로 20도 회전, 스케일은 1인 변환 행렬
	Mat M = getRotationMatrix2D(cp, 20, 1);
	Mat dst1;
	warpAffine(src, dst1, M, Size());

	// 영상이 잘리지 않게 출력하기
	RotatedRect rr(cp, Size2f(src.cols, src.rows), -20.f); // 반시계방향으로 회전한 사각형
	Rect br = rr.boundingRect2f();  // 그 사각형은 바운딩하는 사각형
	
	// src 영상을 바운딩사각형 크기의 출력창에 중심으로 이동한다.
	M = Mat_<float>({ 2, 3 }, { 1, 0, br.width / 2.f - cp.x, 0, 1, br.height / 2.f - cp.y });
	warpAffine(src, src, M, br.size());
	// 출력창의 중심을 구하여 회전 변환
	cp += Point2f(br.width / 2.f - cp.x, br.height / 2.f - cp.y);
	M = getRotationMatrix2D(cp, 20, 1);  
	Mat dst2;
	warpAffine(src, dst2, M, Size());
	
	imshow("dst1", dst1);
	imshow("dst2", dst2);
	waitKey();
	destroyAllWindows();
}

void affine_flip() {
	Mat src = imread("./images/eastsea.bmp");

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	imshow("src", src);
	Mat dst;
	// flip()함수, 세번째인자의 값이 양수면 좌우대칭, 0이면 상하대칭, 음수이면 상하, 좌우 대칭 모두 수행
	int flipCode[] = { 1, 0, -1 };
	for (int i = 0; i < 3; ++i) {
		flip(src, dst, flipCode[i]);
		putText(dst, format("flipCode : %d", flipCode[i]), Point(10, 30), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0), 1, LINE_AA);
		imshow(format("dst%d", i + 1), dst);
	}
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// affine_transform();
	// affine_translation();
	// affine_shear();
	// affine_scale();
	// affine_rotation();
	affine_flip();
	return 0;
}