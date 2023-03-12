#include "../Headers/compoents.h"

int cnt = 0;
string C(int n) {
    string res;
    while (n) {
        res += char('0' + (n % 10));
        n /= 10;
    }
    reverse(res.begin(), res.end());
    return res;
}

void Show(Mat &src, string name) {
    namedWindow(name, WINDOW_FREERATIO);
    imshow(name, src);
}
// ��image�ϻ���rt��ת���εı߿�
void RotatedRect_Drawing(Mat &image, RotatedRect &rt, Scalar color = Scalar(255, 0, 0), int thickness = 5) {
    Point2f lists[4];
    rt.points(lists);
    for(int i = 0; i < 4; i ++) {
        line(image, lists[i], lists[(i + 1) % 4], color, thickness);
    }
}
Mat nearestNeighRotate(cv::Mat img, float angle)
{
    int len = (int)(sqrtf(pow(img.rows, 2) + pow(img.cols, 2)) + 0.5);

    Mat retMat = Mat::zeros(len, len, CV_8UC3);
    float anglePI = angle * CV_PI / 180;
    int xSm, ySm;

    for (int i = 0; i < retMat.rows; i++)
        for (int j = 0; j < retMat.cols; j++)
        {
            xSm = (int)((i - retMat.rows / 2)*cos(anglePI) - (j - retMat.cols / 2)*sin(anglePI) + 0.5);
            ySm = (int)((i - retMat.rows / 2)*sin(anglePI) + (j - retMat.cols / 2)*cos(anglePI) + 0.5);
            xSm += img.rows / 2;
            ySm += img.cols / 2;

            if (xSm >= img.rows || ySm >= img.cols || xSm <= 0 || ySm <= 0) {
                retMat.at<Vec3b>(i, j) = Vec3b(0, 0);
            }
            else {
                retMat.at<Vec3b>(i, j) = img.at<Vec3b>(xSm, ySm);
            }
        }

    return retMat;
}
// ͼ����ת��ͼ��ȫ����ʾ��
void Rotate(Mat &img1, Mat &img2, double angle, double ratio = 0.6) {   //angle��ʾҪ��ת�ĽǶ�
    Point2f center(img1.cols / 2, img1.rows / 2);//����
    Mat m = getRotationMatrix2D(center, angle, ratio);//������ת�ķ���任����0.6����ȫ����ʾ�Ĺؼ�
    //���ڰ���תӦ�õ�����任�����
    warpAffine(img1, img2, m, Size(img1.cols, img1.rows));//����任
    //circle(img2, center, 2, Scalar(255, 0, 0));
}
void Rotate_Point(Mat &src, Point2f p1, Point2f &res, double theta) {
    theta = 360 - theta;    //˳ʱ��ĳ���ʱ��ת
    int rows = src.rows, cols = src.cols;   //ͼƬ�ĳߴ�
    res = p1;
    res.x -= cols / 2;  //���������᲻ͬ����Ҫת������
    res.y = rows / 2 - res.y;
    theta = theta / 180 * M_PI; // ���Ǻ����õ���pi
    res = Point2f(res.x * cos(theta) + res.y * sin(theta), -res.x * sin(theta) + res.y * cos(theta));
    res.x += cols / 2;  //���ȥ
    res.y = -res.y + rows / 2;
}