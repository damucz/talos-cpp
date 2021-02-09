#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

    class NumericalValue;
    class EmConfigValue;
    class ParticleEmitterInstance;

	class EmitterModule : public AbstractModule
	{
	public:
		enum T
		{
			DELAY = 0,
			DURATION = 1,
			RATE = 2,
			CONFIG = 3,
		};
		
        float _defaultDelay = 0;
        float _defaultDuration = 2.0f;
        float _defaultRate = 50.0f;

        void Load(IFileProvider::ModuleData* moduleData) override;

        void ProcessValues() override;

        void UpdateScopeData(ParticleEmitterInstance* particleEmitter);
		
        float GetDelay();
        float GetDuration();
        float GetRate();
        bool IsContinuous();
        bool IsAttached();
        bool IsAligned();
        bool IsAdditive();
        bool IsBlendAdd();
        bool IsImmortal();

        bool IsEmitterModule() const override;

	protected:
        NumericalValue* _delay = nullptr;
        NumericalValue* _duration = nullptr;
        NumericalValue* _rate = nullptr;
        EmConfigValue* _config = nullptr;

        void DefineSlots() override;
	};

}
