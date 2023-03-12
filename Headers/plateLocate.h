#ifndef PLATELOCATE_H
#define PLATELOCATE_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PlateLocate
{
public:
    PlateLocate();
    ~PlateLocate();

    /**
    * src: 带检测车牌原图
    * dst_plates: 输出候选车牌向量
    */
    void locate(Mat src, vector<Mat>& dst_plates);

protected:
    int verifySizes(RotatedRect rotatedRect);
    bool verifyArea(Mat &img);
    void safeRect(Mat src, RotatedRect rect, Rect2f& safa_rect);
    void rotation(Mat src, Mat& dst, Size rect_size, Point2f center, double angle);
    void tortuosity(Mat src, vector<RotatedRect>& rects, vector<Mat>& dst_plates);
};
#endif // !PLATELOCATE_H
