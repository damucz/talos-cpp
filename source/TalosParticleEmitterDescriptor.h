#pragma once

#include <vector>
#include <map>

namespace Talos
{

    class ParticleEffectDescriptor;
    class AbstractModule;
    class ParticleModule;
    class EmitterModule;

	class ParticleEmitterDescriptor
	{
	public:
        typedef AbstractModule* (*TCreateModuleFunction)();
		
        ParticleEmitterDescriptor(ParticleEffectDescriptor* descriptor);
        ~ParticleEmitterDescriptor();

        static const std::map<const char*, TCreateModuleFunction>& GetRegisteredModules();
        static void RegisterModules();
        static AbstractModule* CreateModule(const char* clazz);

        bool AddModule(AbstractModule* module);
        void RemoveModule(AbstractModule* module);
		
        void ConnectNode(AbstractModule* from, AbstractModule* to, int slotFrom, int slotTo);
        void RemoveNode(AbstractModule* module, int slot, bool isInput);
        void ResetRequesters();

        ParticleModule* GetParticleModule() const;
        EmitterModule* GetEmitterModule() const;
        const std::vector<AbstractModule*>& GetModules() const;
        ParticleEffectDescriptor* GetEffectDescriptor() const;

        bool IsContinuous() const;
        int GetSortPosition() const;
        void SetSortPosition(int sortPosition);

    protected:
        ParticleEffectDescriptor* _particleEffectResourceDescriptor = nullptr;
        std::vector<AbstractModule*> _modules;
        ParticleModule* _particleModule = nullptr;
		EmitterModule* _emitterModule = nullptr;
        int _sortPosition = 0;

        template<typename T>
        static AbstractModule* CreateModule();

        static std::map<const char*, TCreateModuleFunction> _registeredModules;
	};

    template <typename T>
    AbstractModule* ParticleEmitterDescriptor::CreateModule()
    {
        return new T;
    }

}
