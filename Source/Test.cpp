#include "../Headers/Test.h"

void test() {
    const char *svmXmlPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\model\\svm.xml";
    const char *annXmlPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\model\\ann.xml";
    const char *annZhXmlPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\model\\ann_zh.xml";
    int op;
    while (cin >> op) {
        string srcPath = "E:\\CLion_projects\\vehicle_license_plate_recognition\\resource\\img\\test";
        srcPath += C(op);
        srcPath += ".jpg";
        Mat src = imread(srcPath);
        if (src.empty()) {
            cout << "ERROR: Cannot find the file" << endl;
            return;
        }
        PlateRecognize pr(svmXmlPath, annXmlPath, annZhXmlPath);
        string str_plate = pr.plateRecognize(src);
        cout << str_plate << endl;
        waitKey();
        destroyAllWindows();
//        system("cls");
    }
}