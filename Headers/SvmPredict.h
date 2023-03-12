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

	//candi_plates : ��ѡ���ƣ� plate��������������ճ��ƣ�
	void doPredict(vector<Mat> candi_plates, Mat& plate);

private:
	Ptr<SVM> svm;
	HOGDescriptor* svmHog = nullptr;
	void getHogFeatures(HOGDescriptor* svmHog, Mat src, Mat& dst);
};


#endif // !SVMPREDICT_H
