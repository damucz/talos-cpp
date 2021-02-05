#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class GlobalScopeModule : public AbstractModule
	{
	public:
		enum T
		{
			OUTPUT = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        int GetKey() const;
        void SetKey(int key);
	protected:
        NumericalValue* _output = nullptr;
        int _key = -1;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}