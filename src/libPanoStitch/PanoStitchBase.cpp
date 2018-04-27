
#include "PanoStitchBase.h"
#include "error_code.h"


namespace LibPanoStitch
{

PanoStitchBase::PanoStitchBase(void) 
{
}


PanoStitchBase::~PanoStitchBase(void)
{
}

int PanoStitchBase::SetParameters(const PanoStitchParam_t & params)
{
	_params = params;
	return 0;
}



int PanoStitchBase::SetRigFile (const string& rigJsonFile)
{
	_rigJsonFile = rigJsonFile;
	return ERRCODE_OK;
}




}