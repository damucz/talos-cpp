#include "modules/TalosCartToRadModule.h"
#include "values/TalosNumericalValue.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{

	void CartToRadModule::DefineSlots()
	{
		_position = CreateInputSlot(POSITION);

		_angle = CreateOutputSlot(ANGLE);
		_length = CreateOutputSlot(LENGTH);
	}

	void CartToRadModule::ProcessValues()
	{
		float x = _position->Get(0);
		float y = _position->Get(1);

		float angle = atan2f(y, x) * 180.0f / (float)M_PI;
		if (angle < 0) angle += 360.0f;
		_angle->Set(angle);
		_length->Set(sqrtf(x * x + y * y));
	}
	
}
