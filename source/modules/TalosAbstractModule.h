#pragma once

#include "serialization/TalosIFileProvider.h"
#include <map>

namespace Talos
{

    class ParticleEmitterDescriptor;
    class Slot;
    class Value;
    class NumericalValue;
    class ScopePayload;

    class AbstractModule
    {
    public:
        virtual ~AbstractModule();

        virtual void Init();
        virtual void Load(IFileProvider::ModuleData* moduleData);

        virtual void SetModuleGraph(ParticleEmitterDescriptor* graph);
        void AttachModuleToMyInput(AbstractModule* module, int mySlot, int targetSlot);
        void AttachModuleToMyOutput(AbstractModule* module, int mySlot, int targetSlot);
        void Detach(AbstractModule* module);
        void Detach(int slot, bool isInput);
        bool IsConnectedTo(AbstractModule* module);

        /**
		 * Need to keep the output values updated
		 */
        virtual void ProcessValues() = 0;

        void SetIndex(int index);

        /**
         * Fetch value from the input of this module
         * @param slotId
         */
        void FetchInputSlotValue(int slotId);

        /**
         * this module is asked to calculate and then give it's output value
         * @param slotId
         */
        Value* FetchOutputSlotValue(int slotId);

        virtual void FetchAllInputSlotValues();

        Value* CreateInputSlot(int slotId, Value* value);
        Value* CreateOutputSlot(int slotId, Value* value);

        NumericalValue* CreateInputSlot(int slotId);
        NumericalValue* CreateOutputSlot(int slotId);

        ScopePayload* GetScope() const;

        Slot* GetInputSlot(int slotId) const;
        Slot* GetOutputSlot(int slotId) const;

        void ResetLastRequester();

        int GetIndex() const;

        std::map<int, Slot*> GetInputSlots() const;
        std::map<int, Slot*> GetOutputSlots() const;

        virtual bool IsParticleModule() const;
        virtual bool IsEmitterModule() const;
    	
    protected:
        virtual void DefineSlots() = 0;
    	
        ParticleEmitterDescriptor* _graph = nullptr;

        std::map<int, Slot*> _inputSlots;
        std::map<int, Slot*> _outputSlots;

        int _index = -1;

        float _lastRequester = 0;
    };

}