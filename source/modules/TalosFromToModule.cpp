#include "modules/TalosFromToModule.h"
#include "values/TalosNumericalValue.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	
	void FromToModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_defaultFromX = moduleData->GetFloat("fromX");
		_defaultFromY = moduleData->GetFloat("fromY");
		_defaultToX = moduleData->GetFloat("toX");
		_defaultToY = moduleData->GetFloat("toY");
	}

	void FromToModule::SetDefaults(float dFromX, float dFromY, float dToX, float dToY)
	{
		_defaultFromX = dFromX;
		_defaultFromY = dFromY;
		_defaultToX = dToX;
		_defaultToY = dToY;
	}

	void FromToModule::DefineSlots()
	{
		_from = CreateInputSlot(FROM);
		_to = CreateInputSlot(TO);

		_position = CreateOutputSlot(POSITION);
		_rotation = CreateOutputSlot(ROTATION);
		_length = CreateOutputSlot(LENGTH);
	}

	void FromToModule::ProcessValues()
	{
		if (_from->IsEmpty()) _from->Set(_defaultFromX, _defaultFromY);
		if (_to->IsEmpty()) _to->Set(_defaultToX, _defaultToY);

		float x = _to->Get(0) - _from->Get(0);
		float y = _to->Get(1) - _from->Get(1);

		_position->Set(_from->Get(0) + x / 2.0f, _from->Get(1) + y / 2.0f);
		
		float angle = atan2f(y, x) * 180.0f / (float)M_PI;
		if (angle < 0) angle += 360.0f;
		_rotation->Set(angle);

		_length->Set(sqrtf(x * x + y * y));
	}
	
}
