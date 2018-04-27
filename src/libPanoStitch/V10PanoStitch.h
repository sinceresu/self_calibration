#pragma once
#include "PanoStitchBase.h"

#include <memory>
#include <vector>

namespace LibPanoStitch
{
class V10PanoStitch :
	public PanoStitchBase
{
public:
	V10PanoStitch(void);
	~V10PanoStitch(void);
	virtual int Stitch(vector<cv::Mat> & inputImages, cv::Mat& stitchedMat);
private:

};
}