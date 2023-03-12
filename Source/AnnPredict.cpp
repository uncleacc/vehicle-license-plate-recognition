#include "../Headers/AnnPredict.h"

string AnnPredict::ZHCHARS[] = {"��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "³",
                                "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "ԥ", "��", "��",
                                "��", "��", "��"};
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

string AnnPredict::doPredict(Mat final_plate, vector<Mat> &plateCharMats) {
    string str_plate;
    predict(plateCharMats, str_plate);
    return str_plate;
}

void AnnPredict::predict(vector<Mat> plateCharMats, string &str_plate) {
    for (int i = 0; i < plateCharMats.size(); i++) {
        Mat plate_char = plateCharMats[i];
        resize(plate_char, plate_char, Size(20, 20));
//        Show(plate_char);
        Mat features;
        getHogFeatures(annHog, plate_char, features);
        Mat sample = features.reshape(1, 1);
        Mat response;
        Point maxLoc;
        if (i) { //��0��true
            //��ĸ + ����
            ann->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //��������ֵ(CHAR���������ֵ��
//            cout << index << " " << CHARS[index] << endl;
            str_plate += CHARS[index];
        } else { //����
            ann_zh->predict(sample, response);
            minMaxLoc(response, nullptr, nullptr, nullptr, &maxLoc);
            int index = maxLoc.x; //��������ֵ(ZHCHAR���������ֵ��
            str_plate += ZHCHARS[index];
//            cout << index << " " << ZHCHARS[index] << endl;
        }
    }
}

void AnnPredict::getHogFeatures(HOGDescriptor *hog, Mat src, Mat &dst) {
    //��һ��
    Mat trainImg = Mat(hog->winSize, CV_32S);
    resize(src, trainImg, hog->winSize);
//	��������
    vector<float> descriptor;
    hog->compute(trainImg, descriptor, hog->winSize);

    Mat feature(descriptor);
    feature.copyTo(dst);

    feature.release();
    trainImg.release();
}
