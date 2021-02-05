#include "modules/TalosEmConfigModule.h"
#include "values/TalosEmConfigValue.h"

namespace Talos
{
	EmConfigModule::~EmConfigModule()
	{
		delete _userValue;
	}

	void EmConfigModule::Init()
	{
        AbstractModule::Init();

        _userValue = new EmConfigValue;

        _userValue->_attached = false;
        _userValue->_continuous = true;
        _userValue->_additive = true;
        _userValue->_isBlendAdd = false;
        _userValue->_aligned = false;
	}

	void EmConfigModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_userValue->Load(moduleData);
		if (_outputValue != nullptr)
			_outputValue->Set(GetUserValue());
	}

	void EmConfigModule::ProcessValues()
	{
		_outputValue->Set(_userValue);
	}

	EmConfigValue* EmConfigModule::GetUserValue() const
	{
		return _userValue;
	}

	void EmConfigModule::DefineSlots()
	{
		_outputValue = (EmConfigValue*)CreateOutputSlot(OUTPUT, new EmConfigValue);
	}
	
}
