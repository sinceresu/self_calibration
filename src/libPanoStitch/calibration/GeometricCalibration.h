#include "Camera.h"

using namespace surround360;
#include <array>

struct Keypoint {
	Camera::Vector2 position;
	Camera::Real scale;
	Camera::Real orientation;
	int index;

	Keypoint(
		const Camera::Vector2& position,
		Camera::Real scale = 0,
		Camera::Real orientation = 0,
		int index = -1) :
		position(position),
		scale(scale),
		orientation(orientation),
		index(index) {
	}
};

using KeypointMap = std::unordered_map<std::string, std::vector<Keypoint>>;


struct Overlap {
	std::array<std::string, 2> images;
	std::vector<std::array<size_t, 2>> matches;

	Overlap(const std::string& image0, const std::string& image1) {
		images[0] = image0;
		images[1] = image1;
	}

	bool isIntraFrame() const;

};


int calibrate(const Camera::Rig& rig,
	KeypointMap keypointMap,
	std::vector<Overlap> overlaps,
	const std::string& debugDir,
	Camera::Rig& rigCalibrated
);

