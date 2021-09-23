#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


// 해리스 코너 검출
void corner_harris() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 해리스 코너 응답 함수 구하기
	Mat harris;
	cornerHarris(src, harris, 3, 3, 0.04);
	// harris 행렬 원소값 범위를 0~255로 정규화하고 CV_8U 타입으로 변환
	Mat harris_norm;
	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);
	// harris_norm 값이 120보다 큰 픽셀에서 지역 최대값을 코너로 판별
	for (int j = 1; j < harris.rows - 1; ++j) {
		for (int i = 1; i < harris.cols - 1; ++i) {
			if (harris_norm.at<uchar>(j, i) > 120) {
				if (harris.at<float>(j, i) > harris.at<float>(j - 1, i) &&
					harris.at<float>(j, i) > harris.at<float>(j + 1, i) &&
					harris.at<float>(j, i) > harris.at<float>(j, i - 1) &&
					harris.at<float>(j, i) > harris.at<float>(j, i + 1)) {
					circle(dst, Point(i, j), 5, Scalar(0, 0, 255), 2);
				}
			}
		}
	}

	imshow("src", src);
	imshow("harris_norm", harris_norm);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// FAST 코너 검출
void corner_fast() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// 코너점 검출, 밝기 차이 임계값 60, 비최대 억제 수행
	vector<KeyPoint> keypoints;
	FAST(src, keypoints, 60, true);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);

	for (KeyPoint& kp : keypoints) {
		Point pt(cvRound(kp.pt.x), cvRound(kp.pt.y));
		circle(dst, pt, 5, Scalar(0, 0, 255), 2);
	}
	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// 키포인트 검출
void detect_keypoints() {
	Mat src = imread("./images/box_in_scene.png", IMREAD_GRAYSCALE);
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ORB 클래스 객체를 생성하여  feature 스마트 포인터에 저장
	Ptr<Feature2D> feature = ORB::create(); 
	// 키포인트 검출
	vector<KeyPoint> keypoints;
	feature->detect(src, keypoints);
	// 기술자 계산
	Mat desc;
	feature->compute(src, keypoints, desc);
	// 키포인트 개수와 desc 행렬의 크기, 500, 32X500
	cout << "keypoints.size() : " << keypoints.size() << endl;
	cout << "desc.size() : " << desc.size() << endl;
	// 키포인트 위치, 크기, 방향 정보를 그림
	Mat dst;
	drawKeypoints(src, keypoints, dst, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


void keypoint_good_matching() {
	Mat src1 = imread("./images/box.png", IMREAD_GRAYSCALE);
	Mat src2 = imread("./images/box_in_scene.png", IMREAD_GRAYSCALE);

	if (src1.empty() || src2.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// ORB 클래스 객체 생성
	Ptr<Feature2D> feature = ORB::create();
	// 특징점 검출 및 기술자 계산
	vector<KeyPoint> keypoints1, keypoints2;
	Mat desc1, desc2;
	feature->detectAndCompute(src1, Mat(), keypoints1, desc1);
	feature->detectAndCompute(src2, Mat(), keypoints2, desc2);
	// BFMatcher 클래스 객체 생성, 기술자 거리 계산 방식은 해밍 거리 사용
	Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);
	// 기술자를 서로 매칭
	vector<DMatch> matches;
	matcher->match(desc1, desc2, matches);
	// 두영상의 특징점 매칭 결과를 정렬, 상위 50개의 결과만 추출
	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 50);
	// 매칭 결과 영상을 생성, NOT_DRAW_SINGLE_POINTS - 매칭되지 않은 특징점은 화면에 표시하지 않음
	Mat dst;
	drawMatches(src1, keypoints1, src2, keypoints2, good_matches, dst,
		Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// 키포인트 매칭 및 호모그래피 계산
void find_homography() {
	Mat src1 = imread("./images/box.png", IMREAD_GRAYSCALE);
	Mat src2 = imread("./images/box_in_scene.png", IMREAD_GRAYSCALE);

	if (src1.empty() || src2.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	Ptr<Feature2D> orb = ORB::create();
	vector<KeyPoint> keypoints1, keypoints2;
	Mat desc1, desc2;
	orb->detectAndCompute(src1, Mat(), keypoints1, desc1);
	orb->detectAndCompute(src2, Mat(), keypoints2, desc2);

	Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);

	vector<DMatch> matches;
	matcher->match(desc1, desc2, matches);

	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 50);

	Mat dst;
	drawMatches(src1, keypoints1, src2, keypoints2, good_matches, dst, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	// good_matches로 추출된 질의영상과 훈련영상의 특징점 좌표를 저장
	vector<Point2f> pts1, pts2;
	for (int i = 0; i < good_matches.size(); ++i) {
		pts1.push_back(keypoints1[good_matches[i].queryIdx].pt);
		pts2.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	// 호모그래피 행렬 구하기, RANSAC 알고리즘을 사용
	Mat H = findHomography(pts1, pts2, RANSAC);
	// 호모그래피 행렬로 src1의 네모서리점이 이동하는 좌표 구하기
	vector<Point2f> corners1, corners2;
	corners1.push_back(Point2f(0, 0));
	corners1.push_back(Point2f(src1.cols - 1, 0));
	corners1.push_back(Point2f(src1.cols - 1, src1.rows - 1));
	corners1.push_back(Point2f(0, src1.rows - 1));
	perspectiveTransform(corners1, corners2, H);
	// dst 결과창에 위치할 좌표를 저장
	vector<Point> corners_dst;
	for (Point2f pt : corners2) {
		corners_dst.push_back(Point(cvRound(pt.x + src1.cols), cvRound(pt.y)));
	}
	// 찾은 객체를 녹색 사각형으로 그림
	polylines(dst, corners_dst, true, Scalar(0, 255, 0), 2, LINE_AA);

	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// 영상 이어 붙이기
void img_stitching() {
	Mat src1 = imread("./images/img1.jpg");
	Mat src2 = imread("./images/img2.jpg");
	Mat src3 = imread("./images/img3.jpg");

	if (src1.empty() || src2.empty() || src3.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	vector<Mat> imgs;
	imgs.push_back(src1);
	imgs.push_back(src2);
	imgs.push_back(src3);

	// Stitcher 객체 생성
	Ptr<Stitcher> stitcher = Stitcher::create();
	Mat dst;
	// 영상 이어 붙이기, 성공하면 Stitcher::Status::OK 반환
	Stitcher::Status status = stitcher->stitch(imgs, dst);
	
	if (status != Stitcher::Status::OK) {
		cerr << "Error on stitching" << endl;
		return;
	}
	imwrite("./images/result.jpg", dst);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


int main(void) {
	// corner_harris();
	// corner_fast();
	// detect_keypoints();
	// keypoint_good_matching();
	// find_homography();
	img_stitching();

	return 0;
}