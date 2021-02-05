#include "TalosSlot.h"
#include "values/TalosValue.h"

namespace Talos
{
	Slot::Slot(AbstractModule* currentModule, int index, bool isInput)
	{
		_currentModule = currentModule;
		_index = index;
		_isInput = isInput;
	}

	Slot::~Slot()
	{
		delete _value;
	}

	AbstractModule* Slot::GetTargetModule() const
	{
		return _targetModule;
	}

	bool Slot::IsInput() const
	{
		return _isInput;
	}

	void Slot::SetIndex(int index)
	{
		_index = index;
	}

	void Slot::SetValue(Value* value)
	{
		_value = value;
	}

	int Slot::GetIndex() const
	{
		return _index;
	}

	void Slot::Connect(AbstractModule* targetModule, Slot* targetSlot)
	{
		_targetModule = targetModule;
		_targetSlot = targetSlot;
		if (_value != nullptr)
			_value->SetEmpty(true);
	}

	Slot* Slot::GetTargetSlot() const
	{
		return _targetSlot;
	}

	Value* Slot::GetValue() const
	{
		return _value;
	}

	bool Slot::IsCompatable(Slot* slot) const
	{
		if (_value == nullptr || slot->_value == nullptr)
			return true;

		return _value->GetValueType() == slot->_value->GetValueType();
	}

	void Slot::Detach()
	{
		_targetModule = nullptr;
		_targetSlot = nullptr;
	}
}
