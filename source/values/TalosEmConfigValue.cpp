#include "values/TalosEmConfigValue.h"

namespace Talos
{
	void EmConfigValue::Load(IFileProvider::ModuleData* moduleData)
	{
		_attached = moduleData->GetBool("attached");
		_continuous = moduleData->GetBool("continuous");
		_aligned = moduleData->GetBool("aligned");
		_additive = moduleData->GetBool("additive");
		_isBlendAdd = moduleData->GetBool("isBlendAdd");
	}

	const char* EmConfigValue::GetValueType()
	{
		static const char* type = "emconfig";
		return type;
	}

	void EmConfigValue::Set(Value* value)
	{
		if (GetValueType() == value->GetValueType())
			Set((EmConfigValue*)value);
	}

	void EmConfigValue::Set(EmConfigValue* from)
	{
		_attached = from->_attached;
		_continuous = from->_continuous;
		_aligned = from->_aligned;
		_additive = from->_additive;
		_isBlendAdd = from->_isBlendAdd;
	}
	
}
