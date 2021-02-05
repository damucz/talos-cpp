#include "modules/TalosStaticValueModule.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{
	
	StaticValueModule::~StaticValueModule()
	{
		delete _staticValue;
	}

	void StaticValueModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		SetStaticValue(moduleData->GetFloat("value"));
	}

	void StaticValueModule::SetStaticValue(float val)
	{
		_staticValue->Set(val);
	}

	float StaticValueModule::GetStaticValue() const
	{
		return _staticValue->GetFloat();
	}

	NumericalValue* StaticValueModule::GetOutputValue()
	{
		return _outputValue;
	}

	void StaticValueModule::DefineSlots()
	{
		_outputValue = CreateOutputSlot(OUTPUT);

		_staticValue = new NumericalValue();
		_staticValue->Set(1.0f);
	}

	void StaticValueModule::ProcessValues()
	{
		_outputValue->Set(_staticValue);
	}
	
}
