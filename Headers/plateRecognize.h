#ifndef PLATERECOGNIZE_H
#define PLATERECOGNIZE_H

#include "SobelLocate.h"
#include "ColorLocate.h"
#include "SvmPredict.h"
#include "AnnPredict.h"
#include "TemplatePredict.h"

class PlateRecognize
{
public:
	PlateRecognize(const char* svm_model, const char* ann_model, const char* ann_zh_model, const char* template_model);
	~PlateRecognize();

	/**
	* src 待识别车牌原图
	* return 车牌字符串
	*/
	String plateRecognize(Mat src);

private:
	SobelLocate* sobelLocate = nullptr;
    CharSplit* charSplit = nullptr;
    ColorLocate* colorLocate = nullptr;
	SvmPredict* svmPredict = nullptr;
	AnnPredict* annPredict = nullptr;
    TemplatePredict* temPredict = nullptr;
};


#endif // PLATERECOGNIZE_H