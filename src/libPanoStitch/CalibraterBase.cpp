
#include "CalibraterBase.h"
#include "error_code.h"


namespace LibPanoStitch
{

CalibraterBase::CalibraterBase(void) 
{
}


CalibraterBase::~CalibraterBase(void)
{
}

int CalibraterBase::SetParameters(const CalibrateParam_t & params)
{
	return 0;
}

int CalibraterBase::SetRigFile(const string& rigJsonFile)
{
	_rigJsonFile = rigJsonFile;
	return ERRCODE_OK;
}


}