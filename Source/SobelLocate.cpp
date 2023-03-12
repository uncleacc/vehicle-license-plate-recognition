#include "../Headers/SobelLocate.h"
SobelLocate::SobelLocate() {}
SobelLocate::~SobelLocate() {}
/*
* sobel ��λ
*/
void SobelLocate::locate(Mat src, vector<Mat> &dst_plates) {
    //1.��˹ģ��
    Mat blur;
    //��opencv�У�ֻ֧�������뾶
    GaussianBlur(src, blur, Size(5, 5), 0);
    //2.�ҶȻ�
    Mat gray;
    cvtColor(blur, gray, COLOR_BGR2GRAY);
    //3.Sobel��canny����
    Mat sobel_16, canny_img, sobel;
    // ����sobel��Ե��⣬16λת��8λ
    Sobel(gray, sobel_16, CV_16S, 1, 0);//Sobel�󵼺�ĵ���ֵ����С��0���ߴ���255,8λ�治��
    convertScaleAbs(sobel_16, sobel);
//    Canny(gray, sobel, 100, 200, 3, true); //����canny��Ե���
//    imshow("sobel", sobel);

    //4.��ֵ��(��/��)
    //����ֵ�� �ʺϣ����ƣ����ǳ�֣�
    //����ֵ�� �ʺϣ�ǳ������
    Mat shold;
    threshold(sobel, shold, 0, 255, THRESH_OTSU + THRESH_BINARY);
//    imshow("shold", shold);

    //5.��̬ѧ�������ղ�����
    //�ó�����ĸ���������ӵ�һ�����С�˿�
    Mat element = getStructuringElement(MORPH_RECT, Size(src.cols/40, 3)); //����ֵ���ɵ�
    Mat close;
    morphologyEx(shold, close, MORPH_CLOSE, element);
    namedWindow("�ղ���", WINDOW_FREERATIO);
    imshow("�ղ���", close);
    //6.������
    vector<vector<Point>> contours;
    findContours(
            close, //����ͼ��
            contours, //�������
            RETR_LIST, //ȡ��������
            CHAIN_APPROX_NONE //ȡ�������������ص�
    );
    RotatedRect rotatedRect;
    vector<RotatedRect> vec_sobel_rects;
    for (auto points: contours) {
        rotatedRect = minAreaRect(points); //ȡ��С��Ӿ��Σ�����ת/���Ƕȣ�
//        RotatedRect_Drawing(src, rotatedRect, Scalar(0, 0, 255), 1); //����ɫ����
        //7.�ߴ��ж�
        //�������˲�����Ҫ��ľ���
        if (verifySizes(rotatedRect)) vec_sobel_rects.push_back(rotatedRect);
    }
    int num = 0;
    for (RotatedRect rect: vec_sobel_rects) {
        Point2f tmp[4];
        rect.points(tmp);
//        putText(src, C(++ num), tmp[0], FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 255));
//        RotatedRect_Drawing(src, rect, Scalar(0, 255, 0), 1); //����ɫ����
    }
//    namedWindow("���Ƴ��Ƶ�����", WINDOW_FREERATIO);
//    imshow("���Ƴ��Ƶ�����", src);

    Mat temp = src.clone(), dst;
    for (auto p: vec_sobel_rects) {
        Mat temp = src.clone(), dst;
        double angle;
        //���ν�����ֻ֧�ֳ�����������������ŵ�ͼ��ʶ���ʧ��
        if(p.size.width < p.size.height) {
            angle = 360 - 90 + p.angle;
            swap(p.size.width, p.size.height);
        }
        else angle = p.angle;
        Rotate(temp, temp, angle, 1);
        Rotate_Point(src, p.center, p.center, angle);
        getRectSubPix(temp, p.size, p.center, dst); //��ȡ��������
        //��ѡ����
        if(verifyArea(dst)) {   //��֤��ɫ���
            //������С
            Mat plate_mat;
            plate_mat.create(36, 136, CV_8UC3); //�������ߴ��й�
            resize(dst, plate_mat, plate_mat.size());
            dst_plates.push_back(plate_mat);
        }
    }
    blur.release();
    gray.release();
    sobel_16.release();
    sobel.release();
    shold.release();
    element.release();
    close.release();
}
