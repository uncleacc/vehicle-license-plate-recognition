#ifndef VEHICLE_LICENSE_PLATE_RECOGNITION_CHARSPLIT_H
#define VEHICLE_LICENSE_PLATE_RECOGNITION_CHARSPLIT_H
#include "compoents.h"

class CharSplit {
public:
    void getChineseRect(Rect cityRect, Rect& chineseRect);
    int getCityIndex(vector<Rect> rects);
    bool verifyCharSize(Mat src);
    bool clearMaoDing(Mat& src);
    bool charSplit(Mat final_plate, vector<Mat> &plateCharMats);
};

#endif //VEHICLE_LICENSE_PLATE_RECOGNITION_CHARSPLIT_H
