#ifndef __CameraINFO__
#define __CameraINFO__

#include "Linker.h"
#include "INFO.h"
#include <Vector.h>
#include <array>
#include <set>

class CommonDLL_API CameraINFO : public INFO
{
public:
	static const unsigned int GLOBALUSERDATASIZE = 16;
	enum class InclusionType { Exclude = 0, Include = 1 };

	CameraINFO();

	// Camera Stuff
	CML::Vec4 Eye;
	CML::Vec4 TargetMagnitude;
	CML::Vec4 Up;
	double Roll;
	double Pitch;
	double Yaw;

	// Prespective Stuff
	double FovAngleY;
	double NearZ;
	double FarZ;

	CML::Vec4 ClearColor;

	bool Process2D;

	std::vector<std::string> Global2DTexture;
	std::vector<std::string> GlobalCubeTexture;

	std::array<float, GLOBALUSERDATASIZE> GlobalUserData;

	InclusionType InclusionState;
	std::set<std::string> ObjectList;

	virtual std::shared_ptr<Object> Get(const std::string& ID);
	virtual void Set(const std::string& ID, std::shared_ptr<Object> obj);
	virtual std::shared_ptr<INFO> Clone();
};

#endif //__CameraINFO__