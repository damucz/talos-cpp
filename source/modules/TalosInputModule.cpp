#include "modules/TalosInputModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"

namespace Talos
{
	
	void InputModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		SetInput(moduleData->GetInt("scopeKey"));
	}

	void InputModule::SetInput(int scopeKey)
	{
		_scopeKey = scopeKey;
	}

	int InputModule::GetInput() const
	{
		return _scopeKey;
	}

	void InputModule::DefineSlots()
	{
		_outputValue = CreateOutputSlot(OUTPUT);
	}

	void InputModule::ProcessValues()
	{
		NumericalValue* value = GetScope()->Get(_scopeKey);
		_outputValue->Set(value);
	}
	
}
