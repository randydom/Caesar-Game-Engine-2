#ifndef __LuaProcesses__
#define __LuaProcesses__

#include <string>

#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <Converter.h>

class LuaProcesses
{
public:
	LuaProcesses(){this->ID = CHL::ToString(boost::uuids::random_generator()()); }

	virtual void Action() = 0;

	std::string ID;
};


#endif //__LuaProcesses__