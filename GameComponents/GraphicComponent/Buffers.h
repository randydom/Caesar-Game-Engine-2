#ifndef __cBuffer__
#define __cBuffer__

#include "Linker.h"

#include <string>
#include <D3D11.h>
#include <xnamath.h>
#include <vector>

class cBuffer
{
public:
	static const int NUMBEROFLIGHTS = 10;

	struct MaterialInfo
	{
		XMFLOAT4 diffuse;
	};

	struct CLightDesc
	{
		XMFLOAT4 pos;		
		XMFLOAT4 dir;		
		MaterialInfo material;
		XMFLOAT4 attenuation;				
		XMFLOAT4 lightPowerRangeType;	// // x = power, y = range, z = light type, w = not used
	};

	struct cbObject
	{
		XMMATRIX world;
		XMMATRIX worldViewProj;
		MaterialInfo colour;   
	};

	struct cbInfo
	{
		XMMATRIX view;
		XMMATRIX proj;
		XMFLOAT4 eye;
	};
};
#endif //__Buffer__