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
// 在image上画出rt旋转矩形的边框
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
// 图像旋转（图像全部显示）
void Rotate(Mat &img1, Mat &img2, double angle, double ratio = 0.6) {   //angle表示要旋转的角度
    Point2f center(img1.cols / 2, img1.rows / 2);//中心
    Mat m = getRotationMatrix2D(center, angle, ratio);//计算旋转的仿射变换矩阵，0.6就是全部显示的关键
    //现在把旋转应用到仿射变换的输出
    warpAffine(img1, img2, m, Size(img1.cols, img1.rows));//仿射变换
    //circle(img2, center, 2, Scalar(255, 0, 0));
}
void Rotate_Point(Mat &src, Point2f p1, Point2f &res, double theta) {
    theta = 360 - theta;    //顺时针改成逆时针转
    int rows = src.rows, cols = src.cols;   //图片的尺寸
    res = p1;
    res.x -= cols / 2;  //由于坐标轴不同，需要转换坐标
    res.y = rows / 2 - res.y;
    theta = theta / 180 * M_PI; // 三角函数用的是pi
    res = Point2f(res.x * cos(theta) + res.y * sin(theta), -res.x * sin(theta) + res.y * cos(theta));
    res.x += cols / 2;  //变回去
    res.y = -res.y + rows / 2;
}