#ifndef PLATERECOGNIZE_H
#define PLATERECOGNIZE_H

#include "SobelLocate.h"
#include "ColorLocate.h"
#include "SvmPredict.h"
#include "AnnPredict.h"

class PlateRecognize
{
public:
	PlateRecognize(const char* svm_model, const char* ann_model, const char* ann_zh_model);
	~PlateRecognize();

	/**
	* src ��ʶ����ԭͼ
	* return �����ַ���
	*/
	String plateRecognize(Mat src);

private:
	SobelLocate* sobelLocate = nullptr;
    CharSplit* charSplit = nullptr;
    ColorLocate* colorLocate = nullptr;
	SvmPredict* svmPredict = nullptr;
	AnnPredict* annPredict = nullptr;
};


#endif // PLATERECOGNIZE_H