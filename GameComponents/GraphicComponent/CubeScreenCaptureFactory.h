#ifndef __CubeScreenCaptureFactory__
#define __CubeScreenCaptureFactory__

#include "Linker.h"
#include <iCubeScreenCaptureFactory.h>

class GraphicComponentDLL_API CubeScreenCaptureFactory : public iCubeScreenCaptureFactory
{
public:
	virtual void Create(unsigned int width,
						unsigned int height,
						unsigned int priority,
						const std::string& cameraID,
						const std::string& drawSettingsID,
						std::string& ID,
						std::string& textureID);
	virtual void SetCameraID(const std::string& id, const std::string& cameraid);
	virtual void SetDrawSettingsID(const std::string& id, const std::string& dsID);
	virtual void SetPriority(const std::string& id, int p);
	virtual void Release(std::string ID);
};

#endif //__CubeScreenCaptureFactory__