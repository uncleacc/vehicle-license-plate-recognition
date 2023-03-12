#ifndef SVMPREDICT_H
#define SVMPREDICT_H

#include "../Headers/compoents.h"
using namespace std;
using namespace cv;
using namespace ml;


class SvmPredict
{
public:
	SvmPredict(const char* svm_model);
	~SvmPredict();

	//candi_plates : 候选车牌， plate：测评结果（最终车牌）
	void doPredict(vector<Mat> candi_plates, Mat& plate);

private:
	Ptr<SVM> svm;
	HOGDescriptor* svmHog = nullptr;
	void getHogFeatures(HOGDescriptor* svmHog, Mat src, Mat& dst);
};


#endif // !SVMPREDICT_H
