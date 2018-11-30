//
// Created by gjs on 18-11-30.
//

#ifndef ASTAR_SINGLEMAP_H
#define ASTAR_SINGLEMAP_H

#include "singleton.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class SingleMap {
public:
    Mat& GetSingleMap() {
        if(imgmap.empty()) {
            imgmap = imread("../yard.png", 0);
        }
        return imgmap;
    }
private:
    Mat imgmap;
};

using SingleMapMat = Singleton<SingleMap>;

#endif //ASTAR_SINGLEMAP_H
