
#include "V10PanoStitch.h"
#include <assert.h>
#include <algorithm>

#include "Camera.h"
#include "CvUtil.h"
#include "Filter.h"
#include "ImageWarper.h"
#include "MathUtil.h"
#include "MonotonicTable.h"
#include "NovelView.h"
#include "OpticalFlowFactory.h"
#include "OpticalFlowVisualization.h"
#include "RigDescription.h"
#include "StringUtil.h"
#include "SystemUtil.h"
#include "VrCamException.h"


using namespace surround360;

const Camera::Vector3 kGlobalUp = Camera::Vector3::UnitZ();

namespace LibPanoStitch
{

V10PanoStitch::V10PanoStitch(void)
{
}


V10PanoStitch::~V10PanoStitch(void)
{
}
void bicubicRemapToSpherical(
	Mat& dst,
	const Mat& src,
	const Camera& camera,
	const float leftAngle,
	const float rightAngle,
	const float topAngle,
	const float bottomAngle) {
	Mat warp(dst.size(), CV_32FC2);
	for (int x = 0; x < warp.cols; ++x) {
		// sweep xAngle from leftAngle to rightAngle
		const float xFrac = (x + 0.5f) / warp.cols;
		const float xAngle = (1 - xFrac) * leftAngle + xFrac * rightAngle;
		for (int y = 0; y < warp.rows; ++y) {
			// sweep yAngle from topAngle to bottomAngle
			const float yFrac = (y + 0.5f) / warp.rows;
			float yAngle = (1 - yFrac) * topAngle + yFrac * bottomAngle;
			const Camera::Vector3 unit(
				cos(yAngle) * cos(xAngle),
				cos(yAngle) * sin(xAngle),
				sin(yAngle));
			const Camera::Vector2 pixel =
				camera.pixel(unit * int(Camera::kNearInfinity));
			warp.at<Point2f>(y, x) = Point2f(pixel.x() - 0.5, pixel.y() - 0.5);
		}
	}
	Mat tmp = src;
	if (src.channels() == 3 && dst.channels() == 4) {
		cvtColor(src, tmp, CV_BGR2BGRA);
	}
	remap(tmp, dst, warp, Mat(), CV_INTER_CUBIC, BORDER_CONSTANT);
}

// similar to prepareBottomImagesThread but there is no pole removal
void prepareTopImages(
	const RigDescription& rig,
	const Mat& topImage,
	V10PanoStitch::PanoStitchParam_t params,
	Mat* topSpherical) {

	const Camera& camera = rig.findCameraByDirection(kGlobalUp);
	topSpherical->create(
		params.eqr_height * camera.getFov() / M_PI,
		params.eqr_width,
		CV_8UC3);
	bicubicRemapToSpherical(
		*topSpherical,
		topImage,
		camera,
		2.0f * M_PI,
		0,
		M_PI / 2.0f,
		M_PI / 2.0f - camera.getFov());

	// alpha feather the top spherical image for flow purposes
	cvtColor(*topSpherical, *topSpherical, CV_BGR2BGRA);
	const int yFeatherStart = topSpherical->rows - 1 - params.alpha_feather_size;
	for (int y = yFeatherStart; y < topSpherical->rows; ++y) {
		for (int x = 0; x < topSpherical->cols; ++x) {
			const float alpha =
				1.0f - float(y - yFeatherStart) / float(params.alpha_feather_size);
			topSpherical->at<Vec4b>(y, x)[3] = 255.0f * alpha;
		}
	}

}

void prepareBottomImages(
	const RigDescription& rig,
	const Mat& bottomImage,
	V10PanoStitch::PanoStitchParam_t params,
	Mat* bottomSpherical) {

	const Camera& camera = rig.findCameraByDirection(-kGlobalUp);
	bottomSpherical->create(
		params.eqr_height * camera.getFov() / M_PI,
		params.eqr_width,
		CV_8UC3);
	bicubicRemapToSpherical(
		*bottomSpherical,
		bottomImage,
		camera,
		0,
		2.0f * M_PI,
		-(M_PI / 2.0f),
		-(M_PI / 2.0f - camera.getFov()));

	// if we skipped pole removal, there is no alpha channel and we need to add one.
	if (bottomSpherical->type() != CV_8UC4) {
		cvtColor(*bottomSpherical, *bottomSpherical, CV_BGR2BGRA);
	}

	// the alpha channel in bottomSpherical is the result of pole removal/flow. this can in
	// some cases cause an alpha-channel discontinuity at the boundary of the image, which
	// will have an effect on flow between bottom and sides. to mitigate that, we do another
	// pass of feathering on bottomSpherical before converting to polar coordinates.
	const int yFeatherStart = bottomSpherical->rows - 1 - params.alpha_feather_size;
	for (int y = yFeatherStart; y < bottomSpherical->rows; ++y) {
		for (int x = 0; x < bottomSpherical->cols; ++x) {
			const float alpha =
				1.0f - float(y - yFeatherStart) / float(params.alpha_feather_size);
			bottomSpherical->at<Vec4b>(y, x)[3] =
				min(bottomSpherical->at<Vec4b>(y, x)[3], (unsigned char)(255.0f * alpha));
		}
	}

}


int V10PanoStitch::Stitch(vector<cv::Mat>& inputImages, cv::Mat & stitchedMat)
{
	RigDescription rig(_rigJsonFile);

	Mat topSpherical;
	prepareTopImages(rig, inputImages[0], _params, &topSpherical);
	Mat bottomSpherical;
	prepareBottomImages(rig, inputImages[1], _params, &bottomSpherical);

	Mat warpedTopSpherical(_params.eqr_height, _params.eqr_width, topSpherical.type(), Scalar::all(0));
	Mat imgPanelRoi(warpedTopSpherical, Rect(0, 0, topSpherical.cols, topSpherical.rows));
	topSpherical.copyTo(warpedTopSpherical(Rect(0, 0, topSpherical.cols, topSpherical.rows)));

	flip(bottomSpherical, bottomSpherical, -1);

	bottomSpherical.copyTo(warpedTopSpherical(Rect(0, warpedTopSpherical.rows - bottomSpherical.rows, bottomSpherical.cols, bottomSpherical.rows)));


    Mat warpedBottomSpherical(_params.eqr_height, _params.eqr_width, topSpherical.type(), Scalar::all(0));
    bottomSpherical.copyTo(warpedBottomSpherical(Rect(0, warpedTopSpherical.rows - bottomSpherical.rows, bottomSpherical.cols, bottomSpherical.rows)));
    topSpherical.copyTo(warpedBottomSpherical(Rect(0, 0, topSpherical.cols, topSpherical.rows)));

	if (true) {
		const string debugDir =
			"debug/";
		imwriteExceptionOnFail(debugDir + "/_topSpherical.png", topSpherical);
		imwriteExceptionOnFail(debugDir + "/_bottomSpherical.png", bottomSpherical);
		imwriteExceptionOnFail(debugDir + "/warpedTopSpherical.png", warpedTopSpherical);
		imwriteExceptionOnFail(debugDir + "/warpedBottomSpherical.png", warpedBottomSpherical);
	}


	return 0;
}


}