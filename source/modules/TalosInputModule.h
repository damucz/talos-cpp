#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class InputModule : public AbstractModule
	{
	public:
		enum T
		{
			OUTPUT = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetInput(int scopeKey);
        int GetInput() const;
		
	protected:
        NumericalValue* _outputValue = nullptr;
        int _scopeKey = -1;

        void DefineSlots() override;
        void ProcessValues() override;
	};

}
