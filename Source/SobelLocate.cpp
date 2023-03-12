#include "../Headers/SobelLocate.h"
SobelLocate::SobelLocate() {}
SobelLocate::~SobelLocate() {}
/*
* sobel 定位
*/
void SobelLocate::locate(Mat src, vector<Mat> &dst_plates) {
    //1.高斯模糊
    Mat blur;
    //在opencv中，只支持奇数半径
    GaussianBlur(src, blur, Size(5, 5), 0);
    //2.灰度化
    Mat gray;
    cvtColor(blur, gray, COLOR_BGR2GRAY);
    //3.Sobel或canny运算
    Mat sobel_16, canny_img, sobel;
    // 利用sobel边缘检测，16位转回8位
    Sobel(gray, sobel_16, CV_16S, 1, 0);//Sobel求导后的导数值可能小于0或者大于255,8位存不下
    convertScaleAbs(sobel_16, sobel);
//    Canny(gray, sobel, 100, 200, 3, true); //利用canny边缘检测
//    imshow("sobel", sobel);

    //4.二值化(正/反)
    //正二值化 适合：蓝牌（深底浅字）
    //反二值化 适合：浅底深字
    Mat shold;
    threshold(sobel, shold, 0, 255, THRESH_OTSU + THRESH_BINARY);
//    imshow("shold", shold);

    //5.形态学操作（闭操作）
    //让车牌字母和数字链接到一起的最小核宽
    Mat element = getStructuringElement(MORPH_RECT, Size(src.cols/40, 3)); //经验值，可调
    Mat close;
    morphologyEx(shold, close, MORPH_CLOSE, element);
    namedWindow("闭操作", WINDOW_FREERATIO);
    imshow("闭操作", close);
    //6.求轮廓
    vector<vector<Point>> contours;
    findContours(
            close, //输入图像
            contours, //输出轮廓
            RETR_LIST, //取所有轮廓
            CHAIN_APPROX_NONE //取轮廓上所有像素点
    );
    RotatedRect rotatedRect;
    vector<RotatedRect> vec_sobel_rects;
    for (auto points: contours) {
        rotatedRect = minAreaRect(points); //取最小外接矩形（可旋转/带角度）
//        RotatedRect_Drawing(src, rotatedRect, Scalar(0, 0, 255), 1); //画红色矩形
        //7.尺寸判断
        //初步过滤不符合要求的矩形
        if (verifySizes(rotatedRect)) vec_sobel_rects.push_back(rotatedRect);
    }
    int num = 0;
    for (RotatedRect rect: vec_sobel_rects) {
        Point2f tmp[4];
        rect.points(tmp);
//        putText(src, C(++ num), tmp[0], FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255));
//        RotatedRect_Drawing(src, rect, Scalar(0, 255, 0), 1); //画绿色矩形
    }
//    namedWindow("疑似车牌的区域", WINDOW_FREERATIO);
//    imshow("疑似车牌的区域", src);

    Mat temp = src.clone(), dst;
    for (auto p: vec_sobel_rects) {
        Mat temp = src.clone(), dst;
        double angle;
        //矩形矫正，只支持车牌是正的情况，倒着的图像识别会失败
        if(p.size.width < p.size.height) {
            angle = 360 - 90 + p.angle;
            swap(p.size.width, p.size.height);
        }
        else angle = p.angle;
        Rotate(temp, temp, angle, 1);
        Rotate_Point(src, p.center, p.center, angle);
        getRectSubPix(temp, p.size, p.center, dst); //截取车牌区域
        //候选车牌
        if(verifyArea(dst)) {   //验证蓝色面积
            //调整大小
            Mat plate_mat;
            plate_mat.create(36, 136, CV_8UC3); //跟样本尺寸有关
            resize(dst, plate_mat, plate_mat.size());
            dst_plates.push_back(plate_mat);
        }
    }
    blur.release();
    gray.release();
    sobel_16.release();
    sobel.release();
    shold.release();
    element.release();
    close.release();
}
