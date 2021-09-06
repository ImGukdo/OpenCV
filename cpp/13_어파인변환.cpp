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

	// �Է� ������ �� ��, ��¿����� �� ���� �迭�� ����
	Point2f srcPts[3], dstPts[3];
	srcPts[0] = Point2f(0, 0);
	srcPts[1] = Point2f(src.cols - 1, 0); 
	srcPts[2] = Point2f(src.cols - 1, src.rows - 1); 
	dstPts[0] = Point2f(50, 50); 
	dstPts[1] = Point2f(src.cols - 100, 100); 
	dstPts[2] = Point2f(src.cols - 50, src.rows - 50); 

	// 2X3 ������ ��ȯ����� M�� ����
	Mat M = getAffineTransform(srcPts, dstPts);

	// warpAffine �Լ��� �̿��� src ������ ������ ��ȯ�Ͽ� dst�� ����
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
	// ���η� 150, ���η� 100 �̵��ƴ� ������ ��ȯ ���
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
	// ���� �������� �и� ������ 0.3���� ������ ���� ��ȯ ����� ����
	double mx = 0.3;
	Mat M = Mat_<double>({ 2, 3 }, { 1, mx, 0, 0, 1, 0 });
	Mat dst;
	// ���ܺ�ȯ�� ���� ������ �Ϻΰ� �߸��� �ʵ��� ��� ������ ����ũ�⸦ �÷��ش�.
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
	// ���� Ȯ�� ��ȯ
	Mat dst1, dst2, dst3, dst4;
	resize(src, dst1, Size(), 4, 4, INTER_NEAREST); // �ֱٹ� �̿� ������, �ȼ� ��ü�� Ŀ����ó�� ����
	resize(src, dst2, Size(1920, 1280));  // �缱�� ������, �⺻��
	resize(src, dst3, Size(1920, 1280), 0, 0, INTER_CUBIC);  // 3�� ȸ�� ������, Ȯ�뿵�󿡼� ȭ���� ����, ���� �ӵ��� ����
	resize(src, dst4, Size(1920, 1280), 0, 0, INTER_LANCZOS4);  // ���ʽ� ������, Ȯ�뿵�󿡼� ȭ���� ����, ���� �ӵ��� ����

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
	Point2f cp(src.cols / 2.f, src.rows / 2.f); // ������ �߽� ��ǥ
	// cp ��ǥ�� �������� �ݽð�������� 20�� ȸ��, �������� 1�� ��ȯ ���
	Mat M = getRotationMatrix2D(cp, 20, 1);
	Mat dst1;
	warpAffine(src, dst1, M, Size());

	// ������ �߸��� �ʰ� ����ϱ�
	RotatedRect rr(cp, Size2f(src.cols, src.rows), -20.f); // �ݽð�������� ȸ���� �簢��
	Rect br = rr.boundingRect2f();  // �� �簢���� �ٿ���ϴ� �簢��
	
	// src ������ �ٿ���簢�� ũ���� ���â�� �߽����� �̵��Ѵ�.
	M = Mat_<float>({ 2, 3 }, { 1, 0, br.width / 2.f - cp.x, 0, 1, br.height / 2.f - cp.y });
	warpAffine(src, src, M, br.size());
	// ���â�� �߽��� ���Ͽ� ȸ�� ��ȯ
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
	// flip()�Լ�, ����°������ ���� ����� �¿��Ī, 0�̸� ���ϴ�Ī, �����̸� ����, �¿� ��Ī ��� ����
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