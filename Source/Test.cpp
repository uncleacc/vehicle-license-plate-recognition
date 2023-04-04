#include "../Headers/Test.h"

void test() {
    const char *svmXmlPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\model\\svm.xml";
    const char *annXmlPath = "E:\\graduation_project\\project1\\NeVehicleLicensePlateRecognition\\resources\\ann.xml";
    const char *annZhXmlPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\model\\ann_zh.xml";
    const char *templatePath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\template";
    string srcPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\img\\8\\test1.jpg";
    Mat src = imread(srcPath);
    if (src.empty()) {
        cout << "ERROR: Cannot find the file" << endl;
        return;
    }
    PlateRecognize pr(svmXmlPath, annXmlPath, annZhXmlPath, templatePath);
    string str_plate = pr.plateRecognize(src);
    cout << str_plate << endl;
    waitKey();
    destroyAllWindows();
}