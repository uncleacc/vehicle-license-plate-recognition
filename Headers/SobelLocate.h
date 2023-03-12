#ifndef SOBELLOCATE_H
#define SOBELLOCATE_H

#include "plateLocate.h"
#include "compoents.h"

class SobelLocate : public PlateLocate
{
public:
	SobelLocate();
	~SobelLocate();
	void locate(Mat src, vector<Mat>& dst_plates);
};

#endif //SOBELLOCATE_H