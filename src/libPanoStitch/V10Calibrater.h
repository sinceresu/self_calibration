#pragma once
#include "CalibraterBase.h"

#include <memory>
#include <vector>

namespace LibPanoStitch
{
class V10Calibrater :
	public CalibraterBase
{
public:
	V10Calibrater(void);
	~V10Calibrater(void);
	virtual int Calibrate(vector<cv::Mat> & inputImages, const string& outputRigFile);
private:

};
}