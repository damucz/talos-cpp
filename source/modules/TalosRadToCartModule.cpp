#include "modules/TalosRadToCartModule.h"
#include "values/TalosNumericalValue.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	
	void RadToCartModule::DefineSlots()
	{
		_a = CreateInputSlot(A);
		_l = CreateInputSlot(L);

		_output = CreateOutputSlot(OUTPUT);

		_a->SetFlavour(NumericalValue::ANGLE);
	}

	void RadToCartModule::ProcessValues()
	{
		float x = _l->GetFloat();
		float y = 0;

		float angle = _a->GetFloat() * (float)M_PI / 180.0f;
		
		float cos = cosf(angle);
		float sin = sinf(angle);

		_output->Set(x * cos - y * sin, x * sin + y * cos);
	}
	
}
