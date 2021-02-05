#pragma once

#include "TalosAbstractModule.h"

namespace Talos
{

    class EmConfigValue;
	
	class EmConfigModule : public AbstractModule
	{
	public:
        enum T
        {
            OUTPUT = 0,
		};

		~EmConfigModule();

        void Init() override;
		void Load(IFileProvider::ModuleData* moduleData) override;

        void ProcessValues() override;

        EmConfigValue* GetUserValue() const;

	protected:
        EmConfigValue* _userValue = nullptr;
        EmConfigValue* _outputValue = nullptr;

        void DefineSlots() override;
	};
	
}