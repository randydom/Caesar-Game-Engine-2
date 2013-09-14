#ifndef __iObjectDrawable__
#define __iObjectDrawable__

#include "Object.h"

#include <memory>
#include <string>
#include <vector>
#include <limits>

class Drawable
{
#undef max
public:
	virtual void Init()									= 0;
	virtual void Clean()								= 0;
	virtual void Update(float delta)					= 0;
	virtual void Draw(std::shared_ptr<Object> object)	= 0;
	virtual Drawable* clone() const						= 0;
	virtual float GetOrder(){return std::numeric_limits<float>::max();}

	virtual ~Drawable(){}
};


#endif //__iObjectDrawable__