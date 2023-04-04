#include "../Headers/TemplatePredict.h"

char TemplatePredict::CHAR[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};
string TemplatePredict::CHAR_ZH[] = {
        "²Ø", "´¨", "¸Ê", "¸Ó", "¹ã", "¹ó", "º£", "ºÚ", "»¦", "¼ª", "¼Ã", "¼½", "½ò", "½ú", "¾©", "ÁÉ", "Â³", "ÃÉ", "Ãö", "Äþ", "Çà", "Çí", "ÉÂ", "Éò", "ËÕ", "Íî", "Ïæ", "ÐÂ", "Óå", "Ô¥", "ÔÁ", "ÔÆ", "Õã"
};
TemplatePredict::TemplatePredict(string imgPath) {
    path = imgPath;
}
//¶ÁÈ¡ÎÄ¼þ  Í¼Æ¬
bool Read_Data(string filename, vector<Mat>&dataset) {
    vector<String> imagePathList;
    glob(filename, imagePathList);
    if (imagePathList.empty()) return false;

    for (int i = 0; i < imagePathList.size(); i++) {
        Mat image = imread(imagePathList[i]);
        if(image.empty()) {
            cout << imagePathList[i] << endl;
            continue;
        }
        cvtColor(image, image, COLOR_BGR2GRAY);
        threshold(image, image, 0, 255, THRESH_BINARY | THRESH_OTSU);
        resize(image, image, Size(50, 100));
        dataset.push_back(image);
    }

    return true;
}
int pixCount(Mat &img) {
    int res = 0;
    for(int i = 0; i < img.size().width; i ++) {
        for(int j = 0; j < img.size().height; j ++) {
            if(img.at<uchar>(i, j) != 0) res ++;
        }
    }
    return res;
}
string predict(Mat &img, vector<Mat> &dataSet, int flag) {
    int id = 0, MI = 0x3f3f3f3f, i = 0;
    for(auto &p : dataSet) {
        Mat dest;
        absdiff(img, p, dest);
        int count = pixCount(dest);
        if(count < MI) {
            MI = count;
            id = i;
        }
        i ++;
    }
    Show(img);
    Show(dataSet[id]);
    if(flag == 0) return TemplatePredict::CHAR_ZH[id];
    else {
        string res; res += TemplatePredict::CHAR[id];
        return res;
    }
}
string TemplatePredict::doPredict(vector<Mat> plates) {
    for(auto &p : plates) resize(p, p, Size(50, 100));
    vector<Mat> dataSetCHAR, dataSetCHARZH;
    if(!Read_Data(path+"\\CHAR", dataSetCHAR)) return "Î´Ê¶±ðµ½³µÅÆ";
    if(!Read_Data(path+"\\CHAR_ZH", dataSetCHARZH)) return "Î´Ê¶±ðµ½³µÅÆ";

    string res;
    res += predict(plates[0], dataSetCHARZH, 0);
    for(int i = 1; i < 7; i ++) res += predict(plates[i], dataSetCHAR, 1);
    return res;
}