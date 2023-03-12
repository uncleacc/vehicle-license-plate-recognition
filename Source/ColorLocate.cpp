#include "../Headers/ColorLocate.h"
#include "../Headers/compoents.h"

ColorLocate::ColorLocate() {
}
ColorLocate::~ColorLocate() {
}
/**
* color ��λ
*/
void ColorLocate::colorLocate(Mat src, Mat &dst) {
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);
    //����ɫ���أ��ο� https://www.jianshu.com/p/8ed28846ac9a  hsv��Χ.png
    //h(ɫ��): 100-124
    //s(���Ͷ�): 43-255
    //v(ɫ��������): 46-255
    int channels = hsv.channels(); //ͨ���� = 3
    int width = hsv.cols * channels;
    int height = hsv.rows;
    uchar *p;
    if (hsv.isContinuous()) { //�ж����ڴ����Ƿ������洢
        width *= height;
        height = 1;
    }
    for (int i = 0; i < height; i++) {
        p = hsv.ptr<uchar>(i); //��i�е�����
        for (int j = 0; j < width; j += channels) //ע�ⲽ��
        {
            int h = p[j]; //H����
            int s = p[j + 1]; //S����
            int v = p[j + 2]; //V����

            bool isBlue = false;
            if (h >= 100 && h <= 124 &&
                s >= 43 && s <= 255 &&
                v >= 46 && v <= 255) {
                isBlue = true;
            }

            if (isBlue) { //����ɫ���ֳ��루͹�ԣ�����
                p[j] = 0;
                p[j + 1] = 0;
                p[j + 2] = 255;
            } else { //����ɫ���ص�Ϳ��
                p[j] = 0;
                p[j + 1] = 0;
                p[j + 2] = 0;
            }
        }
    }
    //����
    vector<Mat> hsv_split;
    split(hsv, hsv_split); //��ͨ�����з��� merge
    //imshow("����V����", hsv_split[2]);

    //��ֵ��
    Mat shold;
    threshold(hsv_split[2], shold, 0, 255, THRESH_OTSU + THRESH_BINARY);
    //imshow("��ֵ��", shold);
    //�ղ���
    Mat element = getStructuringElement(MORPH_RECT, Size(17, 3)); //����ֵ���ɵ�
    Mat close;
    morphologyEx(shold, close, MORPH_CLOSE, element);
    //imshow("close", close);

    //������
    vector<vector<Point>> contours;
    findContours(
            close, //����ͼ��
            contours, //���ͼ��
            RETR_EXTERNAL, //ȡ�������
            CHAIN_APPROX_NONE //ȡ�������������ص�
    );
    Rect rt;
    vector<RotatedRect> vec_color_rects;
    for (vector<Point> points: contours) {
        rt = boundingRect(points); //ȡ��С��Ӿ��Σ�����ת/���Ƕȣ�
//        rectangle(src, rt, Scalar(0, 0, 255)); //����ɫ����
    }
    dst = src(rt);
}

