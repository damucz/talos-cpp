#include "modules/TalosGlobalScopeModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"

namespace Talos
{
	
	void GlobalScopeModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_key = moduleData->GetInt("key");
	}

	int GlobalScopeModule::GetKey() const
	{
		return _key;
	}

	void GlobalScopeModule::SetKey(int key)
	{
		_key = key;
	}

	void GlobalScopeModule::DefineSlots()
	{
		_output = CreateOutputSlot(OUTPUT);
	}

	void GlobalScopeModule::ProcessValues()
	{
		_output->Set(GetScope()->GetDynamicValue(_key));
	}
	
}
