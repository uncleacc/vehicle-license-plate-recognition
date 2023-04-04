#include "../Headers/PlateRecognize.h"
#include "../Headers/ColorLocate.h"
#include "../Headers/TemplatePredict.h"

PlateRecognize::PlateRecognize(const char *svm_model, const char *ann_model, const char *ann_zh_model, const char *template_model) {
    sobelLocate = new SobelLocate();
    svmPredict = new SvmPredict(svm_model);
    colorLocate = new ColorLocate();
    charSplit = new CharSplit();
    annPredict = new AnnPredict(ann_model, ann_zh_model);
    temPredict = new TemplatePredict(template_model);
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
    if(temPredict) {
        delete temPredict;
        temPredict = nullptr;
    }
}

/**
车牌识别
*/
string PlateRecognize::plateRecognize(Mat src) {
//    vector<Mat> plates;
//    sobelLocate->locate(src, plates);
//    //筛选，svm测评
//    Mat final_plate;
//    svmPredict->doPredict(plates, final_plate);
//    //利用HSV颜色空间再次定位
//    colorLocate->colorLocate(final_plate, final_plate);
//    Show(final_plate, "colorLocate");
//    //字符分割
//    vector<Mat> plateCharMats;
//    if(!charSplit->charSplit(final_plate, plateCharMats)) return "未识别到车牌";
//    字符识别
//    string str_plate = annPredict->doPredict(plateCharMats);
//    string str_plate = temPredict->doPredict(plateCharMats);    //模板识别
//    return str_plate;
    vector<Mat> plateCharMats;
    return annPredict->doPredict(plateCharMats);
}
