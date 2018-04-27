#pragma once
#include "ICalibrate.h"
#include <memory>

namespace LibPanoStitch
{
class CalibraterBase :
	public ICalibrate
{
public:
	CalibraterBase(void);
	~CalibraterBase(void);

	virtual int SetParameters(const CalibrateParam_t& params);
	virtual int SetRigFile(const string& rigJsonFile);
protected:
	CalibrateParam_t _params;
	string _rigJsonFile;
};

}