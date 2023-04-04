#ifndef VEHICLE_LICENSE_PLATE_RECOGNITION_TEMPLATEPREDICT_H
#define VEHICLE_LICENSE_PLATE_RECOGNITION_TEMPLATEPREDICT_H
#include "compoents.h"

class TemplatePredict {
public:
    string path;
    static char CHAR[];
    static string CHAR_ZH[];
public:
    TemplatePredict(string);
    string doPredict(vector<Mat>);
};


#endif //VEHICLE_LICENSE_PLATE_RECOGNITION_TEMPLATEPREDICT_H
