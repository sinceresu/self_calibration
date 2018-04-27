
#include "V10Calibrater.h"
#include <iostream>
#include <unordered_map>
#include <random>
//#define BOOST_FILESYSTEM_NO_DEPRECATED
//#include <boost/filesystem.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "ceres/ceres.h"
#include "ceres/rotation.h"

#include "Camera.h"
#include "CvUtil.h"
#include "RigDescription.h"
#include "KeypointMatchers.h"
#include "GeometricCalibration.h"




using namespace surround360;
using namespace surround360::calibration;

void generateMatchedPoints(
	KeypointMap& keypointMap,
	std::vector<Overlap>& overlaps,
	vector< pair<Point2f, Point2f> > matchPointPairsFB,
	const std::string& i0,
	const std::string& i1) {

	overlaps.emplace_back(i0, i1);
	for (const pair<Point2f, Point2f>& matchPoint : matchPointPairsFB) {
		Camera::Vector2 point0(matchPoint.first.x, matchPoint.first.y);
		keypointMap[i0].push_back(point0);
		Camera::Vector2 point1(matchPoint.second.x, matchPoint.second.y);
		keypointMap[i1].push_back(point1);

		overlaps.back().matches.push_back({ {
				keypointMap[i0].size() - 1,
				keypointMap[i1].size() - 1 }});
	}
}

const Camera::Vector3 kGlobalUp = Camera::Vector3::UnitZ();

namespace LibPanoStitch
{

V10Calibrater::V10Calibrater(void)
{
}


V10Calibrater::~V10Calibrater(void)
{
}

int V10Calibrater::Calibrate(vector<cv::Mat> & inputImages, const string& outputRigFile)
{
	RigDescription rig(_rigJsonFile);

	const Camera& camera0 = rig.findCameraByDirection(kGlobalUp);
	const Camera& camera1 = rig.findCameraByDirection(-kGlobalUp);

	vector< pair<Point2f, Point2f> > matchPointPairsFB;
	getKeypointMatchesWithAllAlgorithms(inputImages[0], inputImages[1],
		matchPointPairsFB);

	KeypointMap keypointMap;
	std::vector<Overlap> overlaps;
	generateMatchedPoints(keypointMap, overlaps, matchPointPairsFB, camera0.id, camera1.id);

	Camera::Rig rigCalibrated;

	calibrate(rig.rig, keypointMap, overlaps, "debug", rigCalibrated);
	//Camera::saveRig(outputRigFile, cameras);



	return 0;
}


}