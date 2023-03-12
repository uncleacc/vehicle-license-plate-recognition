#include "../Headers/PlateRecognize.h"
#include "../Headers/ColorLocate.h"

PlateRecognize::PlateRecognize(const char *svm_model, const char *ann_model, const char *ann_zh_model) {
    sobelLocate = new SobelLocate();
    svmPredict = new SvmPredict(svm_model);
    colorLocate = new ColorLocate();
    charSplit = new CharSplit();
    annPredict = new AnnPredict(ann_model, ann_zh_model);
}

PlateRecognize::~PlateRecognize() {
    if (sobelLocate) {
        delete sobelLocate;
        sobelLocate = nullptr;
    }
    if (svmPredict) {
        delete svmPredict;
        svmPredict = nullptr;
    }
    if (annPredict) {
        delete annPredict;
        annPredict = nullptr;
    }
    if (charSplit) {
        delete charSplit;
        charSplit = nullptr;
    }
}

/**
车牌识别
*/
string PlateRecognize::plateRecognize(Mat src) {
    vector<Mat> plates;
    sobelLocate->locate(src, plates);
    //筛选，svm测评
    Mat final_plate;
    svmPredict->doPredict(plates, final_plate);
    //利用HSV颜色空间再次定位
    colorLocate->colorLocate(final_plate, final_plate);
    Show(final_plate, "颜色再定位");
    //字符分割
    vector<Mat> plateCharMats;
    charSplit->charSplit(final_plate, plateCharMats);
    //字符识别
    string str_plate = annPredict->doPredict(final_plate, plateCharMats);
    return str_plate;
}
