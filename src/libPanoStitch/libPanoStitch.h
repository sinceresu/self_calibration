// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBIMAGEMERGER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBPanoStitch_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#if defined WIN32 || defined _WIN32
#ifdef LIBPANOSTITCH_EXPORTS 
#define LIBPANOSTITCH_API __declspec(dllexport)
#else
#define LIBPANOSTITCH_API __declspec(dllimport)
#endif
#else
#define LIBPANOSTITCH_API 
#endif

#include <memory>

namespace LibPanoStitch
{

	class IPanoStitch;
	class ICalibrate;
	enum eStitchType_t{
		STITCH_TYPE_SIMPLE = 0,
		STITCH_TYPE_FISHEYE,
		STITCH_TYPE_YIJUMP
	};	
	LIBPANOSTITCH_API std::shared_ptr<IPanoStitch> CreatePanoStitch(eStitchType_t type = STITCH_TYPE_SIMPLE);
	LIBPANOSTITCH_API std::shared_ptr<ICalibrate> CreateCalibrater(eStitchType_t type = STITCH_TYPE_SIMPLE);

}