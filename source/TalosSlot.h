#pragma once

namespace Talos
{

    class AbstractModule;
    class Value;

	class Slot
	{
	public:
		enum Flavour
		{
			ANGLE,
		};

        Slot(AbstractModule* currentModule, int index, bool isInput);
        ~Slot();

        AbstractModule* GetTargetModule() const;
        bool IsInput() const;
        void SetIndex(int index);
        void SetValue(Value* value);
        int GetIndex() const;
        void Connect(AbstractModule* targetModule, Slot* targetSlot);
        Slot* GetTargetSlot() const;
        Value* GetValue() const;
        bool IsCompatable(Slot* slot) const;
        void Detach();
		
	protected:
        int _index = -1;
        bool _isInput = false;
        Flavour _flavour = ANGLE;

        AbstractModule* _currentModule = nullptr;
        AbstractModule* _targetModule = nullptr;
        Slot* _targetSlot = nullptr;

        Value* _value = nullptr;		
	};
	
}
