// Stitch360.cpp : 定义控制台应用程序的入口点。
//
#include "StringUtil.h"
#include "SystemUtil.h"
#include "CvUtil.h"

#include <gflags/gflags.h>
#include <glog/logging.h>


#include "libPanoStitch.h"
#include "ICalibrate.h"

DEFINE_string(rig_json_file, "", "path to json file drescribing camera array");
DEFINE_string(imgs_dir, "", "path to folder of images with names matching cameras in the rig file");

DEFINE_string(frames, "", "folder containing frames");
DEFINE_int64(pass_count, 10, "number of passes");
DEFINE_double(outlier_factor, 5, "reject if error is factor * median");
DEFINE_bool(robust, true, "use Huber loss function");
DEFINE_bool(force_in_front, true, "no intersections behind camera");

using namespace surround360::util;
using namespace LibPanoStitch;


void initCalibrate360(int argc, char** argv) {
	// Initialize Google's logging library
	google::InitGoogleLogging(argv[0]);
	gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
	gflags::HandleCommandLineHelpFlags();

}

#define YI

void calibrateRig() {
	requireArg(FLAGS_rig_json_file, "rig_json_file");

#ifdef YI
	const string forwardImagePath =
		FLAGS_imgs_dir + "/" + "cam_forward.png";
#else
	const string forwardImagePath =
		FLAGS_imgs_dir + "/" + "cam0.png";
#endif

	Mat forwardImage = imreadExceptionOnFail(forwardImagePath, CV_LOAD_IMAGE_COLOR);

#ifdef YI
	const string backwardImagePath =
		FLAGS_imgs_dir + "/" + "cam_backward.png";
#else
	const string backwardImagePath =
		FLAGS_imgs_dir + "/" + "cam15.png";
#endif

	Mat backwardImage = imreadExceptionOnFail(backwardImagePath, CV_LOAD_IMAGE_COLOR);

	std::vector<Mat> inputImages = { forwardImage, backwardImage };

	auto calibrater = CreateCalibrater();
	calibrater->SetRigFile(FLAGS_rig_json_file.c_str());
	ICalibrate::CalibrateParam_t calibrateParam = {
		FLAGS_pass_count, FLAGS_outlier_factor,
		FLAGS_robust
	};
	calibrater->SetParameters(calibrateParam);

	Mat stitchedImage;
	calibrater->Calibrate(inputImages, "calibrated_rig.json");

}
int main(int argc, char** argv)
{
	initCalibrate360(argc, argv);
	calibrateRig();
    return 0;
}

