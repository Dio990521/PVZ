#pragma once
#include "State.h"
class Asset
{
public:
	virtual bool Load(const string& filename) = 0;
	virtual void Release() = 0;
};

 