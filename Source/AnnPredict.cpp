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

Rect GetCenterRect(Mat &in) {
    Rect _rect;

    int top = 0;
    int bottom = in.rows - 1;

    // find the center rect

    for (int i = 0; i < in.rows; ++i) {
        bool bFind = false;
        for (int j = 0; j < in.cols; ++j) {
            if (in.data[i * in.step[0] + j] > 20) {
                top = i;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }

    }
    for (int i = in.rows - 1;
         i >= 0;
         --i) {
        bool bFind = false;
        for (int j = 0; j < in.cols; ++j) {
            if (in.data[i * in.step[0] + j] > 20) {
                bottom = i;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }

    }


    int left = 0;
    int right = in.cols - 1;
    for (int j = 0; j < in.cols; ++j) {
        bool bFind = false;
        for (int i = 0; i < in.rows; ++i) {
            if (in.data[i * in.step[0] + j] > 20) {
                left = j;
                bFind = true;
                break;
            }
        }
        if (bFind) {
            break;
        }

    }
    for (int j = in.cols - 1;
         j >= 0;
         --j) {
        bool bFind = false;
        for (int i = 0; i < in.rows; ++i) {
            if (in.data[i * in.step[0] + j] > 20) {
                right = j;
                bFind = true;

                break;
            }
        }
        if (bFind) {
            break;
        }
    }

    _rect.x = left;
    _rect.y = top;
    _rect.width = right - left + 1;
    _rect.height = bottom - top + 1;

    return _rect;
}

Mat CutTheRect(Mat &in, Rect &rect) {
    int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
    Mat dstMat(size, size, CV_8UC1);
    dstMat.setTo(Scalar(0, 0, 0));

    int x = (int)floor((float)(size - rect.width) / 2.0f);
    int y = (int)floor((float)(size - rect.height) / 2.0f);

    for (int i = 0; i < rect.height; ++i) {

        for (int j = 0; j < rect.width; ++j) {
            dstMat.data[dstMat.step[0] * (i + y) + j + x] =
                    in.data[in.step[0] * (i + rect.y) + j + rect.x];
        }
    }

    //
    return dstMat;
}

float countOfBigValue(Mat &mat, int iValue) {
    float iCount = 0.0;
    if (mat.rows > 1) {
        for (int i = 0; i < mat.rows; ++i) {
            if (mat.data[i * mat.step[0]] > iValue) {
                iCount += 1.0;
            }
        }
        return iCount;

    }
    else {
        for (int i = 0; i < mat.cols; ++i) {
            if (mat.data[i] > iValue) {
                iCount += 1.0;
            }
        }

        return iCount;
    }
}

Mat ProjectedHistogram(Mat img, int t, int threshold = 20) {
    int sz = (t) ? img.rows : img.cols;
    Mat mhist = Mat::zeros(1, sz, CV_32F);

    for (int j = 0; j < sz; j++) {
        Mat data = (t) ? img.row(j) : img.col(j);

        mhist.at<float>(j) = countOfBigValue(data, threshold);
    }

    // Normalize histogram
    double min, max;
    minMaxLoc(mhist, &min, &max);

    if (max > 0)
        mhist.convertTo(mhist, -1, 1.0f / max, 0);

    return mhist;
}

Mat charFeatures(Mat in, int sizeData) {
    const int VERTICAL = 0;
    const int HORIZONTAL = 1;

    // cut the cetner, will afect 5% perices.
    Rect _rect = GetCenterRect(in);
    Mat tmpIn = CutTheRect(in, _rect);
    //Mat tmpIn = in.clone();

    // Low data feature
    Mat lowData;
    resize(tmpIn, lowData, Size(sizeData, sizeData));

    // Histogram features
    Mat vhist = ProjectedHistogram(lowData, VERTICAL);
    Mat hhist = ProjectedHistogram(lowData, HORIZONTAL);

    // Last 10 is the number of moments components
    int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

    Mat out = Mat::zeros(1, numCols, CV_32F);
    // Asign values to

    int j = 0;
    for (int i = 0; i < vhist.cols; i++) {
        out.at<float>(j) = vhist.at<float>(i);
        j++;
    }
    for (int i = 0; i < hhist.cols; i++) {
        out.at<float>(j) = hhist.at<float>(i);
        j++;
    }
    for (int x = 0; x < lowData.cols; x++) {
        for (int y = 0; y < lowData.rows; y++) {
            out.at<float>(j) += (float)lowData.at <unsigned char>(x, y);
            j++;
        }
    }

    //std::cout << out << std::endl;

    return out;
}

void classify(cv::Mat featureRows, std::vector<int>& out_maxIndexs,
                             std::vector<float>& out_maxVals, std::vector<bool> isChineseVec) {
    int rowNum = featureRows.rows;

    cv::Mat output(rowNum, kCharsTotalNumber, CV_32FC1);
    ann_->predict(featureRows, output);

    for (int output_index = 0; output_index < rowNum; output_index++) {
        Mat output_row = output.row(output_index);
        int result = 0;
        float maxVal = -2.f;
        bool isChinses = isChineseVec[output_index];
        if (!isChinses) {
            result = 0;
            for (int j = 0; j < kCharactersNumber; j++) {
                float val = output_row.at<float>(j);
                // std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        else {
            result = kCharactersNumber;
            for (int j = kCharactersNumber; j < kCharsTotalNumber; j++) {
                float val = output_row.at<float>(j);
                //std::cout << "j:" << j << "val:" << val << std::endl;
                if (val > maxVal) {
                    maxVal = val;
                    result = j;
                }
            }
        }
        out_maxIndexs[output_index] = result;
        out_maxVals[output_index] = maxVal;
    }
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
int identify(std::vector<cv::Mat> inputs, std::vector<std::pair<std::string, std::string>>& outputs,
                            std::vector<bool> isChineseVec) {
    Mat featureRows;
    size_t input_size = inputs.size();
    for (size_t i = 0; i < input_size; i++) {
        Mat input = inputs[i];
        cv::Mat feature = charFeatures(input, 10);
        featureRows.push_back(feature);
    }

    std::vector<int> maxIndexs;
    std::vector<float> maxVals;
    classify(featureRows, maxIndexs, maxVals, isChineseVec);

    for (size_t row_index = 0; row_index < input_size; row_index++) {
        int index = maxIndexs[row_index];
        if (index < kCharactersNumber) {
            outputs[row_index] = std::make_pair(kChars[index], kChars[index]);
        }
        else {
            const char* key = kChars[index];
            std::string s = key;
            std::string province = kv_->get(s);
            outputs[row_index] = std::make_pair(s, province);
        }
    }
    return 0;
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
