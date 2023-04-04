#pragma once
#ifndef ANNPREDICT_H
#define ANNPREDICT_H

#include "CharSplit.h"
#include "compoents.h"
using namespace std;
using namespace cv;
using namespace ml;

class AnnPredict {
public:
    AnnPredict(const char* ann_model, const char* ann_zh_model);
    ~AnnPredict();

    /**
    * plate: svm测评得到的最终车牌
    * 返回：车牌字符串
    */
    string doPredict(vector<Mat> &plateCharMats);

    void predict(vector<Mat> plateCharMats, string& str_plate);

private:
    Ptr<ANN_MLP> ann; //字母&数字
    Ptr<ANN_MLP> ann_zh; //中文字符
    HOGDescriptor* annHog = nullptr;
    void getHogFeatures(HOGDescriptor* annHog, Mat src, Mat& dst);
    static string ZHCHARS[];
    static char CHARS[];
};

#endif //!ANNPREDICT_H
