#include "../Headers/AnnPredict.h"

string AnnPredict::ZHCHARS[] = {"川", "鄂", "赣", "甘", "贵", "桂", "黑", "沪", "冀", "津", "京", "吉", "辽", "鲁",
                                "蒙", "闽", "宁", "青", "琼", "陕", "苏", "晋", "皖", "湘", "新", "豫", "渝", "粤",
                                "云", "藏", "浙"};
char AnnPredict::CHARS[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                            'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

AnnPredict::AnnPredict(const char *ann_model, const char *ann_zh_model) {
    ann = ANN_MLP::load(ann_model);
    ann_zh = ANN_MLP::load(ann_zh_model);
    annHog = new HOGDescriptor(Size(32, 32), Size(16, 16), Size(8, 8), Size(8, 8), 3);
}

AnnPredict::~AnnPredict() {
    ann->clear();
    ann.release();
    ann_zh->clear();
    ann_zh.release();
    if (annHog) {
        delete annHog;
        annHog = 0;
    }
}

string AnnPredict::doPredict(vector<Mat> &plateCharMats) {
//    string str_plate;
//    predict(plateCharMats, str_plate);
//    return str_plate;
    vector<cv::String> fileName;
    glob("E:\\graduation_project\\project1\\NeVehicleLicensePlateRecognition\\resources\\ann_train\\ann_train\\ann\\2", fileName, false);
    for(auto filepath : fileName) {
        Mat plate_char = imread(filepath);
        resize(plate_char, plate_char, Size(20, 20));
        Mat features;
        getHogFeatures(annHog, plate_char, features);
        Mat sample = features.reshape(1, 1);
        Mat response;
        Point maxLoc;
        int i = 1;
        string str_plate;
        if (i) { //非0即true
            //字母 + 数字
            ann->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //样本索引值(CHAR数组的索引值）
//            cout << index << " " << CHARS[index] << endl;
            str_plate += CHARS[index];
        } else { //中文
            ann_zh->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //样本索引值(ZHCHAR数组的索引值）
            str_plate += ZHCHARS[index];
//            cout << index << " " << ZHCHARS[index] << endl;
        }
        if(str_plate != "2")
            cout << filepath << endl;
    }
    return "---";
}

void AnnPredict::predict(vector<Mat> plateCharMats, string &str_plate) {
    int num = 0;
    for (int i = 0; i < plateCharMats.size(); i++) {
        Mat plate_char = plateCharMats[i];
        resize(plate_char, plate_char, Size(20, 20));
//        imwrite("E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\testSet\\"+C(++num)+".jpg", plate_char);
//        Show(plate_char); //字符分割后的车牌
        Mat features;
        getHogFeatures(annHog, plate_char, features);
        Mat sample = features.reshape(1, 1);
        Mat response;
        Point maxLoc;
        if (i) { //非0即true
            //字母 + 数字
            ann->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //样本索引值(CHAR数组的索引值）
//            cout << index << " " << CHARS[index] << endl;
            str_plate += CHARS[index];
        } else { //中文
            ann_zh->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //样本索引值(ZHCHAR数组的索引值）
            str_plate += ZHCHARS[index];
//            cout << index << " " << ZHCHARS[index] << endl;
        }
    }
}

void AnnPredict::getHogFeatures(HOGDescriptor *hog, Mat src, Mat &dst) {
    //归一化
    Mat trainImg = Mat(hog->winSize, CV_32S);
    resize(src, trainImg, hog->winSize);
//	计算特征
    vector<float> descriptor;
    hog->compute(trainImg, descriptor, hog->winSize);

    Mat feature(descriptor);
    feature.copyTo(dst);

    feature.release();
    trainImg.release();
}
