#pragma once

#include <vector>

using namespace std;
namespace cv {
	class Mat;
}

namespace LibPanoStitch
{
class IPanoStitch
{
public:
	enum {
		MAX_STITCH_SRCS = 17
	};
	typedef struct PanoStitchParam_t {
		int eqr_width;
		int eqr_height;
		int alpha_feather_size;
	}PanoStitchParam_t;
	virtual ~IPanoStitch(void) {};

	virtual int SetRigFile (const string& rigJsonFile) = 0;
	virtual int SetParameters(const PanoStitchParam_t& params) = 0;
	virtual int Stitch(vector<cv::Mat> & inputImages, cv::Mat& stitchedMat ) = 0;

};

}