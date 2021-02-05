#pragma once

#include "values/TalosValue.h"
#include "serialization/TalosIFileProvider.h"

namespace Talos
{

	class EmConfigValue : public Value
	{
	public:
        void Load(IFileProvider::ModuleData* moduleData);
		
        const char* GetValueType() override;
		
        void Set(Value* value) override;
        void Set(EmConfigValue* from);

        bool _attached = false;
        bool _continuous = true;
        bool _aligned = false;
        bool _additive = true;
        bool _isBlendAdd = false;
	};
	
}
