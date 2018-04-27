#pragma once
#include "IPanoStitch.h"
#include <memory>

namespace LibPanoStitch
{
class PanoStitchBase :
	public IPanoStitch
{
public:
	PanoStitchBase(void);
	~PanoStitchBase(void);
	virtual int SetParameters(const PanoStitchParam_t& params);
	virtual int SetRigFile (const string& rigJsonFile);
protected:
	PanoStitchParam_t _params;
	string _rigJsonFile;
};

}