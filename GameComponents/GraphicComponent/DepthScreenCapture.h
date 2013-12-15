#ifndef __DepthScreenCapture__
#define __DepthScreenCapture__

#include "ScreenCapture.h"
#include "DepthScreenShot.h"

class DepthScreenCapture : public ScreenCapture
{
public:
	DepthScreenCapture(const std::string& inputID);

	virtual void Init();
	virtual void Destory();
	virtual void Update(double realTime, double deltaTime);
	virtual void Snap(std::hash_map<std::string, SP_INFO>& objects);
	virtual std::shared_ptr<ScreenCapture> clone() const;
	virtual ~DepthScreenCapture(){}

	static std::shared_ptr<DepthScreenCapture> Spawn(const std::string& inputID,
		const std::string& textureID,
		unsigned int width,
		unsigned int height,
		CHL::Matrix4x4 cameraMatrix,
		CHL::Matrix4x4 prespectiveMatrix);

	std::shared_ptr<DepthScreenShot>	ScreenShot;
	CHL::Matrix4x4						prespectiveMatrix;
	CHL::Matrix4x4						cameraMatrix;
	unsigned int						width;
	unsigned int						height;
};


#endif //__DepthScreenCapture__
