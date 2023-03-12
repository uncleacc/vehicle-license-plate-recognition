#ifndef COLORLOCATE_H
#define COLORLOCATE_H

#include "PlateLocate.h"

class ColorLocate : public PlateLocate
{
public:
    ColorLocate();
    ~ColorLocate();

    void colorLocate(Mat src, Mat &dst);
};

#endif //COLORLOCATE_H