#pragma once
#include <vector>

using namespace std;
namespace cv {
	class Mat;
}

namespace LibPanoStitch
{

	class ICalibrate
	{
	public:
		typedef struct CalibrateParam_t {
			int64_t pass_count;
			double outlier_factor;
			bool robust;
		}CalibrateParam_t;
		virtual ~ICalibrate(void) {};

		virtual int SetRigFile(const string& rigJsonFile) = 0;
		virtual int SetParameters(const CalibrateParam_t& params) = 0;
		virtual int Calibrate(vector<cv::Mat> & inputImages, const string& outputRigFile) = 0;
	};

}