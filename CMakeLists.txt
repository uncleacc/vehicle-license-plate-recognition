cmake_minimum_required(VERSION 3.24)
project(vehicle_license_plate_recognition)

set(CMAKE_CXX_STANDARD 17)
# 添加2行代码，注意OpenCV_DIR的目录是自己编译生成的文件夹
set(OpenCV_DIR E:/opencv/MinGW-build/install/x64/mingw/lib)
find_package(OpenCV REQUIRED)

add_executable(vehicle_license_plate_recognition Headers/plateLocate.h Headers/AnnPredict.h Headers/ColorLocate.h Headers/SvmPredict.h Source/NeVehicleLicensePlateRecognition.cpp Source/AnnPredict.cpp Source/ColorLocate.cpp Source/PlateLocate.cpp Source/PlateRecognize.cpp Source/SobelLocate.cpp Source/SvmPredict.cpp Headers/plateRecognize.h Headers/SobelLocate.h Source/compoents.cpp Headers/Test.h Source/Test.cpp Headers/CharSplit.h Headers/CharSplit.h Headers/CharSplit.h Source/CharSplit.cpp Source/CharSplit.cpp)
# 添加1行代码
target_link_libraries(vehicle_license_plate_recognition ${OpenCV_LIBS})
