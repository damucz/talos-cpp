#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class StaticValueModule : public AbstractModule
	{
	public:
        enum T
        {
            OUTPUT = 0,
		};

		~StaticValueModule();

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetStaticValue(float val);
        float GetStaticValue() const;
        NumericalValue* GetOutputValue();
		
	protected:
        NumericalValue* _staticValue = nullptr;
        NumericalValue* _outputValue = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
