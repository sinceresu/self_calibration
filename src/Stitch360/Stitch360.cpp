// Stitch360.cpp : 定义控制台应用程序的入口点。
//
#include "StringUtil.h"
#include "SystemUtil.h"
#include "CvUtil.h"

#include <gflags/gflags.h>
#include <glog/logging.h>


#include "libPanoStitch.h"
#include "IPanoStitch.h"

using namespace surround360::util;
using namespace LibPanoStitch;

DEFINE_string(rig_json_file, "", "path to json file drescribing camera array");
DEFINE_string(imgs_dir, "", "path to folder of images with names matching cameras in the rig file");
DEFINE_string(output_equirect_path, "", "path to write output oculus 360 cubemap");

DEFINE_int32(eqr_width, 256, "width of spherical projection image (0 to 2pi)");
DEFINE_int32(eqr_height, 128, "height of spherical projection image (0 to pi)");

void initStitch360(int argc, char** argv) {
	// Initialize Google's logging library
	google::InitGoogleLogging(argv[0]);
	gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
	gflags::HandleCommandLineHelpFlags();

}

#define YI

void stitchPanorama() {
	requireArg(FLAGS_rig_json_file, "rig_json_file");
	requireArg(FLAGS_imgs_dir, "imgs_dir");

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

	auto stitcher = CreatePanoStitch();
	stitcher->SetRigFile(FLAGS_rig_json_file.c_str());
	IPanoStitch::PanoStitchParam_t stitchParam = {
		FLAGS_eqr_width, FLAGS_eqr_height,
		0
	};
	stitcher->SetParameters(stitchParam);

	Mat stitchedImage;
	stitcher->Stitch(inputImages, stitchedImage);
	const string stitchedImagePath =
		FLAGS_imgs_dir + "/" + "stitched.png";

	imwrite(stitchedImagePath, stitchedImage);

}
int main(int argc, char** argv)
{
	initStitch360(argc, argv);
	stitchPanorama();
    return 0;
}

