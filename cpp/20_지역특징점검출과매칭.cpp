#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


// �ظ��� �ڳ� ����
void corner_harris() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// �ظ��� �ڳ� ���� �Լ� ���ϱ�
	Mat harris;
	cornerHarris(src, harris, 3, 3, 0.04);
	// harris ��� ���Ұ� ������ 0~255�� ����ȭ�ϰ� CV_8U Ÿ������ ��ȯ
	Mat harris_norm;
	normalize(harris, harris_norm, 0, 255, NORM_MINMAX, CV_8U);

	Mat dst;
	cvtColor(src, dst, COLOR_GRAY2BGR);
	// harris_norm ���� 120���� ū �ȼ����� ���� �ִ밪�� �ڳʷ� �Ǻ�
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


// FAST �ڳ� ����
void corner_fast() {
	Mat src = imread("./images/building.jpg", IMREAD_GRAYSCALE);

	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}
	// �ڳ��� ����, ��� ���� �Ӱ谪 60, ���ִ� ���� ����
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


// Ű����Ʈ ����
void detect_keypoints() {
	Mat src = imread("./images/box_in_scene.png", IMREAD_GRAYSCALE);
	if (src.empty()) {
		cerr << "Image load failed" << endl;
		return;
	}

	// ORB Ŭ���� ��ü�� �����Ͽ�  feature ����Ʈ �����Ϳ� ����
	Ptr<Feature2D> feature = ORB::create(); 
	// Ű����Ʈ ����
	vector<KeyPoint> keypoints;
	feature->detect(src, keypoints);
	// ����� ���
	Mat desc;
	feature->compute(src, keypoints, desc);
	// Ű����Ʈ ������ desc ����� ũ��, 500, 32X500
	cout << "keypoints.size() : " << keypoints.size() << endl;
	cout << "desc.size() : " << desc.size() << endl;
	// Ű����Ʈ ��ġ, ũ��, ���� ������ �׸�
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
	// ORB Ŭ���� ��ü ����
	Ptr<Feature2D> feature = ORB::create();
	// Ư¡�� ���� �� ����� ���
	vector<KeyPoint> keypoints1, keypoints2;
	Mat desc1, desc2;
	feature->detectAndCompute(src1, Mat(), keypoints1, desc1);
	feature->detectAndCompute(src2, Mat(), keypoints2, desc2);
	// BFMatcher Ŭ���� ��ü ����, ����� �Ÿ� ��� ����� �ع� �Ÿ� ���
	Ptr<DescriptorMatcher> matcher = BFMatcher::create(NORM_HAMMING);
	// ����ڸ� ���� ��Ī
	vector<DMatch> matches;
	matcher->match(desc1, desc2, matches);
	// �ο����� Ư¡�� ��Ī ����� ����, ���� 50���� ����� ����
	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 50);
	// ��Ī ��� ������ ����, NOT_DRAW_SINGLE_POINTS - ��Ī���� ���� Ư¡���� ȭ�鿡 ǥ������ ����
	Mat dst;
	drawMatches(src1, keypoints1, src2, keypoints2, good_matches, dst,
		Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// Ű����Ʈ ��Ī �� ȣ��׷��� ���
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

	// good_matches�� ����� ���ǿ���� �Ʒÿ����� Ư¡�� ��ǥ�� ����
	vector<Point2f> pts1, pts2;
	for (int i = 0; i < good_matches.size(); ++i) {
		pts1.push_back(keypoints1[good_matches[i].queryIdx].pt);
		pts2.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	// ȣ��׷��� ��� ���ϱ�, RANSAC �˰����� ���
	Mat H = findHomography(pts1, pts2, RANSAC);
	// ȣ��׷��� ��ķ� src1�� �׸𼭸����� �̵��ϴ� ��ǥ ���ϱ�
	vector<Point2f> corners1, corners2;
	corners1.push_back(Point2f(0, 0));
	corners1.push_back(Point2f(src1.cols - 1, 0));
	corners1.push_back(Point2f(src1.cols - 1, src1.rows - 1));
	corners1.push_back(Point2f(0, src1.rows - 1));
	perspectiveTransform(corners1, corners2, H);
	// dst ���â�� ��ġ�� ��ǥ�� ����
	vector<Point> corners_dst;
	for (Point2f pt : corners2) {
		corners_dst.push_back(Point(cvRound(pt.x + src1.cols), cvRound(pt.y)));
	}
	// ã�� ��ü�� ��� �簢������ �׸�
	polylines(dst, corners_dst, true, Scalar(0, 255, 0), 2, LINE_AA);

	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}


// ���� �̾� ���̱�
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

	// Stitcher ��ü ����
	Ptr<Stitcher> stitcher = Stitcher::create();
	Mat dst;
	// ���� �̾� ���̱�, �����ϸ� Stitcher::Status::OK ��ȯ
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