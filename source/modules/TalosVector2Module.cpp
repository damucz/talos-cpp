#include "modules/TalosVector2Module.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{
	
	void Vector2Module::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);

		_defaultX = moduleData->GetFloat("x");
		_defaultY = moduleData->GetFloat("y");
	}

	void Vector2Module::SetX(float x)
	{
		_defaultX = x;
	}

	void Vector2Module::SetY(float y)
	{
		_defaultY = y;
	}

	float Vector2Module::GetDefaultX() const
	{
		return _defaultX;
	}

	float Vector2Module::GetDefaultY() const
	{
		return _defaultY;
	}

	void Vector2Module::DefineSlots()
	{
		_x = CreateInputSlot(X);
		_y = CreateInputSlot(Y);

		_output = CreateOutputSlot(OUTPUT);
	}

	void Vector2Module::ProcessValues()
	{
		if (_x->IsEmpty()) _x->Set(_defaultX);
		if (_y->IsEmpty()) _y->Set(_defaultY);

		_output->Set(_x, _y);
	}
	
}
