#include "TalosParticleEmitterDescriptor.h"
#include "modules/TalosEmitterModule.h"

// to be registered
#include "modules/TalosAttractorModule.h"
#include "modules/TalosCartToRadModule.h"
#include "modules/TalosColorModule.h"
#include "modules/TalosCurveModule.h"
#include "modules/TalosDynamicRangeModule.h"
#include "modules/TalosEmConfigModule.h"
#include "modules/TalosFakeMotionBlurModule.h"
#include "modules/TalosForceApplierModule.h"
#include "modules/TalosFromToModule.h"
#include "modules/TalosGlobalScopeModule.h"
#include "modules/TalosGradientColorModule.h"
#include "modules/TalosInputModule.h"
#include "modules/TalosInterpolationModule.h"
#include "modules/TalosMathModule.h"
#include "modules/TalosMixModule.h"
#include "modules/TalosNoiseModule.h"
#include "modules/TalosOffsetModule.h"
#include "modules/TalosParticleModule.h"
#include "modules/TalosPolylineModule.h"
#include "modules/TalosRadToCartModule.h"
#include "modules/TalosRandomRangeModule.h"
#include "modules/TalosShadedSpriteModule.h"
#include "modules/TalosStaticValueModule.h"
#include "modules/TalosTextureModule.h"
#include "modules/TalosVector2Module.h"
//#include "modules/TalosVectorFieldModule.h"

namespace Talos
{

    std::map<const char*, ParticleEmitterDescriptor::TCreateModuleFunction> ParticleEmitterDescriptor::_registeredModules;
	
	ParticleEmitterDescriptor::ParticleEmitterDescriptor(ParticleEffectDescriptor* descriptor)
	{
		_particleEffectResourceDescriptor = descriptor;
		RegisterModules();
	}

    ParticleEmitterDescriptor::~ParticleEmitterDescriptor()
    {
		for (auto* module : _modules)
            delete module;
    }

    const std::map<const char*, ParticleEmitterDescriptor::TCreateModuleFunction>& ParticleEmitterDescriptor::GetRegisteredModules()
	{
		RegisterModules();
		return _registeredModules;
	}

	void ParticleEmitterDescriptor::RegisterModules()
	{
		if (_registeredModules.empty())
		{
            //_registeredModules.insert(std::pair<const char*, TCreateModuleFunction>(name, fnc));
            _registeredModules["AttractorModule"] = &CreateModule<AttractorModule>;
            _registeredModules["CartToRadModule"] = &CreateModule<CartToRadModule>;
            _registeredModules["ColorModule"] = &CreateModule<ColorModule>;
            _registeredModules["CurveModule"] = &CreateModule<CurveModule>;
            _registeredModules["DynamicRangeModule"] = &CreateModule<DynamicRangeModule>;
            _registeredModules["EmConfigModule"] = &CreateModule<EmConfigModule>;
            _registeredModules["EmitterModule"] = &CreateModule<EmitterModule>;
            _registeredModules["FakeMotionBlurModule"] = &CreateModule<FakeMotionBlurModule>;
            _registeredModules["ForceApplierModule"] = &CreateModule<ForceApplierModule>;
            _registeredModules["FromToModule"] = &CreateModule<FromToModule>;
            _registeredModules["GlobalScopeModule"] = &CreateModule<GlobalScopeModule>;
            _registeredModules["GradientColorModule"] = &CreateModule<GradientColorModule>;
            _registeredModules["InputModule"] = &CreateModule<InputModule>;
            _registeredModules["InterpolationModule"] = &CreateModule<InterpolationModule>;
            _registeredModules["MathModule"] = &CreateModule<MathModule>;
            _registeredModules["MixModule"] = &CreateModule<MixModule>;
            _registeredModules["NoiseModule"] = &CreateModule<NoiseModule>;
            _registeredModules["OffsetModule"] = &CreateModule<OffsetModule>;
            _registeredModules["ParticleModule"] = &CreateModule<ParticleModule>;
            _registeredModules["PolylineModule"] = &CreateModule<PolylineModule>;
            _registeredModules["RadToCartModule"] = &CreateModule<RadToCartModule>;
            _registeredModules["RandomRangeModule"] = &CreateModule<RandomRangeModule>;
            _registeredModules["ShadedSpriteModule"] = &CreateModule<ShadedSpriteModule>;
            _registeredModules["StaticValueModule"] = &CreateModule<StaticValueModule>;
            _registeredModules["TextureModule"] = &CreateModule<TextureModule>;
            _registeredModules["Vector2Module"] = &CreateModule<Vector2Module>;
            //_registeredModules["VectorFieldModule"] = &CreateModule<VectorFieldModule>;
			/*
            registeredModules.add(FlipbookModule.class);
            registeredModules.add(NinePatchModule.class);
            registeredModules.add(RibbonModule.class);

            registeredModules.add(RandomInputModule.class);
            registeredModules.add(ScriptModule.class);
			*/
		}
	}

	AbstractModule* ParticleEmitterDescriptor::CreateModule(const char* clazz)
	{
		for (auto it = _registeredModules.begin(); it != _registeredModules.end(); ++it)
		{
			if (strcmp(clazz, it->first) == 0)
			{
                return (it->second)();
			}
		}
        return nullptr;
	}

	bool ParticleEmitterDescriptor::AddModule(AbstractModule* module)
	{
        bool added = true;
        if (module->IsParticleModule())
        {
            if (_particleModule == nullptr)
                _particleModule = (ParticleModule*)module;
            else
                added = false;
        }
        if (module->IsEmitterModule())
        {
            if (_emitterModule == nullptr)
                _emitterModule = (EmitterModule*)module;
            else
                added = false;
        }

        if (added)
            _modules.push_back(module);

        return added;
	}

	void ParticleEmitterDescriptor::RemoveModule(AbstractModule* module)
	{
        // was this module connected to someone?
        for (AbstractModule* toModule : _modules)
        {
            if (toModule->IsConnectedTo(module))
                toModule->Detach(module);
        }

        auto it = std::find(_modules.begin(), _modules.end(), module);
        if (it != _modules.end())
        {
            delete* it;
            _modules.erase(it);
        }

        if (module->IsParticleModule())
            _particleModule = nullptr;
        if (module->IsEmitterModule())
            _emitterModule = nullptr;
	}

	void ParticleEmitterDescriptor::ConnectNode(AbstractModule* from, AbstractModule* to, int slotFrom, int slotTo)
	{
        // slotTo is the input of module to
		// slotFrom is the output of slot from
        from->AttachModuleToMyOutput(to, slotFrom, slotTo);
        to->AttachModuleToMyInput(from, slotTo, slotFrom);
	}

	void ParticleEmitterDescriptor::RemoveNode(AbstractModule* module, int slot, bool isInput)
	{
        module->Detach(slot, isInput);
	}

	void ParticleEmitterDescriptor::ResetRequesters()
	{
        for (AbstractModule* module : _modules)
            module->ResetLastRequester();
	}

	ParticleModule* ParticleEmitterDescriptor::GetParticleModule() const
	{
        return _particleModule;
	}

	EmitterModule* ParticleEmitterDescriptor::GetEmitterModule() const
	{
        return _emitterModule;
	}

	const std::vector<AbstractModule*>& ParticleEmitterDescriptor::GetModules() const
	{
        return _modules;
	}

	ParticleEffectDescriptor* ParticleEmitterDescriptor::GetEffectDescriptor() const
	{
        return _particleEffectResourceDescriptor;
	}

	bool ParticleEmitterDescriptor::IsContinuous() const
	{
        return GetEmitterModule()->IsContinuous();
	}

	int ParticleEmitterDescriptor::GetSortPosition() const
	{
        return _sortPosition;
	}

	void ParticleEmitterDescriptor::SetSortPosition(int sortPosition)
	{
        _sortPosition = sortPosition;
	}
}
