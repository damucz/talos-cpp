#include "modules/TalosAbstractModule.h"
#include "values/TalosValue.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include "TalosSlot.h"
#include "TalosParticleEmitterDescriptor.h"
#include "TalosParticleEffectDescriptor.h"
#include "TalosParticleEffectInstance.h"

namespace Talos
{

	AbstractModule::~AbstractModule()
	{
		for (auto& slot : _inputSlots)
			delete slot.second;
		for (auto& slot : _outputSlots)
			delete slot.second;
	}

	void AbstractModule::Init()
	{
		DefineSlots();
	}

	void AbstractModule::Load(IFileProvider::ModuleData* moduleData)
	{
		SetIndex(moduleData->GetIndex());
	}

	void AbstractModule::SetModuleGraph(ParticleEmitterDescriptor* graph)
	{
		_graph = graph;
	}

	void AbstractModule::AttachModuleToMyInput(AbstractModule* module, int mySlot, int targetSlot)
	{
		auto my = _inputSlots.find(mySlot);
		auto target = module->_outputSlots.find(targetSlot);
		
		if (my == _inputSlots.end() || target == module->_outputSlots.end())
			return;
		my->second->Connect(module, target->second);
	}

	void AbstractModule::AttachModuleToMyOutput(AbstractModule* module, int mySlot, int targetSlot)
	{
		auto my = _outputSlots.find(mySlot);
		auto target = module->_inputSlots.find(targetSlot);

		if (my == _outputSlots.end() || target == module->_inputSlots.end())
			return;
		my->second->Connect(module, target->second);
	}

	void AbstractModule::Detach(AbstractModule* module)
	{
		for (auto it = _inputSlots.begin(); it != _inputSlots.end(); ++it)
		{
			Slot* slot = it->second;
			if (slot->GetTargetModule() == module)
			{
				slot->GetTargetSlot()->Detach();
				slot->Detach();
			}
		}
	}

	void AbstractModule::Detach(int slot, bool isInput)
	{
		auto it = _inputSlots.find(slot);
		if (isInput && it != _inputSlots.end())
			it->second->Detach();
	}

	bool AbstractModule::IsConnectedTo(AbstractModule* module)
	{
		for (auto it = _inputSlots.begin(); it != _inputSlots.end(); ++it)
		{
			if (it->second->GetTargetModule() == module)
				return true;
		}

		return false;
	}

	void AbstractModule::SetIndex(int index)
	{
		_index = index;
	}

	void AbstractModule::FetchInputSlotValue(int slotId)
	{
		//find what it is connected to
		auto it = _inputSlots.find(slotId);
		if (it == _inputSlots.end())
			return;
		Slot* inputSlot = it->second;

		if (inputSlot->GetTargetSlot() == nullptr)
		{
			if (inputSlot->GetValue() == nullptr)
				return;

			inputSlot->GetValue()->SetEmpty(true);
		}
		else
		{
			//ask it's module give it's output value
			Value* result = inputSlot->GetTargetModule()->FetchOutputSlotValue(inputSlot->GetTargetSlot()->GetIndex());
			if (result != nullptr)
			{
				inputSlot->GetValue()->Set(result);
				inputSlot->GetValue()->SetEmpty(false);
			}
		}
	}

	Value* AbstractModule::FetchOutputSlotValue(int slotId)
	{
		float requester = GetScope()->Get(ScopePayload::REQUESTER_ID)->GetFloat();

		if (_lastRequester != requester || (_lastRequester == requester && requester == 0.0f)) // caching mechanism
		{
			//fetch all local inputs
			FetchAllInputSlotValues();
			// process
			ProcessValues();
			_graph->GetEffectDescriptor()->GetInstanceReference()->ReportNodeCall();

			_lastRequester = requester;
		}

		return _outputSlots[slotId]->GetValue();
	}

	void AbstractModule::FetchAllInputSlotValues()
	{
		for (auto it = _inputSlots.begin(); it != _inputSlots.end(); ++it)
		{
			FetchInputSlotValue(it->second->GetIndex());
		}
	}

	Value* AbstractModule::CreateInputSlot(int slotId, Value* value)
	{
		_inputSlots[slotId] = new Slot(this, slotId, true);
		_inputSlots[slotId]->SetValue(value);

		return value;
	}

	Value* AbstractModule::CreateOutputSlot(int slotId, Value* value)
	{
		_outputSlots[slotId] = new Slot(this, slotId, false);
		_outputSlots[slotId]->SetValue(value);

		return value;
	}

	NumericalValue* AbstractModule::CreateInputSlot(int slotId)
	{
		_inputSlots[slotId] = new Slot(this, slotId, true);
		NumericalValue* value = new NumericalValue;
		_inputSlots[slotId]->SetValue(value);

		return value;
	}

	NumericalValue* AbstractModule::CreateOutputSlot(int slotId)
	{
		_outputSlots[slotId] = new Slot(this, slotId, false);
		NumericalValue* value = new NumericalValue;
		_outputSlots[slotId]->SetValue(value);

		return value;
	}

	ScopePayload* AbstractModule::GetScope() const
	{
		return _graph->GetEffectDescriptor()->GetInstanceReference()->GetScope();
	}

	Slot* AbstractModule::GetInputSlot(int slotId) const
	{
		return _inputSlots.at(slotId);
	}

	Slot* AbstractModule::GetOutputSlot(int slotId) const
	{
		return _outputSlots.at(slotId);
	}

	void AbstractModule::ResetLastRequester()
	{
		_lastRequester = -1.0f;
	}

	int AbstractModule::GetIndex() const
	{
		return _index;
	}

	std::map<int, Slot*> AbstractModule::GetInputSlots() const
	{
		return _inputSlots;
	}

	std::map<int, Slot*> AbstractModule::GetOutputSlots() const
	{
		return _outputSlots;
	}

	bool AbstractModule::IsParticleModule() const
	{
		return false;
	}

	bool AbstractModule::IsEmitterModule() const
	{
		return false;
	}

}
