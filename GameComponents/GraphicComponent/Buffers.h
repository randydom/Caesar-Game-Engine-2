#ifndef __cBuffer__
#define __cBuffer__

#include <string>
#include <D3D11.h>
#include <xnamath.h>
#include <vector>

class cBuffer
{
public:

	static const unsigned int numOfLights = 10;
	static const unsigned int numOfTextures = 5;

	struct MaterialInfo
	{
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
	};

	struct CLightDesc
	{
		MaterialInfo material;
		XMFLOAT4 pos;		
		XMFLOAT4 dir;		
		XMFLOAT4 attenuation;	
		float spot;
		float range;
		int type;
		int shadowNum;
	};

	struct cbObject
	{
		XMMATRIX world;
		XMMATRIX worldViewProj;
		MaterialInfo colour;  
		unsigned int NumberOf2DTextures;
		unsigned int NumberOfCubeTextures;
		int HasLight;
		XMMATRIX userData;
	};

	struct cbInfo
	{
		XMMATRIX view;
		XMMATRIX proj;
		XMFLOAT4 eye;
		unsigned int numberOfGlobal2DTexture;
		XMMATRIX globalUserData;
	};

	struct cbLight
	{
		CLightDesc lights[numOfLights];
	};

	struct cbShadows
	{
		XMMATRIX shadows[numOfLights];
	};
};
#endif //__Buffer__