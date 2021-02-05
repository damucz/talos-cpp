#include "modules/TalosParticleModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include "TalosParticle.h"

namespace Talos
{
	
	void ParticleModule::ProcessValues()
	{
        // nothing to process, it's all cool as cucumber
	}

	void ParticleModule::UpdateScopeData(Particle* particle)
	{
        auto* scope = GetScope();
        scope->Set(ScopePayload::EMITTER_ALPHA, particle->GetEmitterAlpha());
        scope->Set(ScopePayload::PARTICLE_ALPHA, particle->_alpha);
        scope->Set(ScopePayload::PARTICLE_SEED, particle->_seed);
        scope->Set(ScopePayload::REQUESTER_ID, particle->_seed);
        scope->Set(ScopePayload::EMITTER_ALPHA_AT_P_INIT, particle->_durationAtInit);
        scope->Set(ScopePayload::PARTICLE_POSITION, particle->GetX(), particle->GetY());

        scope->SetParticle(particle);
	}

	IParticleDrawable* ParticleModule::GetDrawable()
	{
        FetchInputSlotValue(DRAWABLE);
        if (_drawable->IsEmpty() || _drawable->GetDrawable() == nullptr)
            return _defaultDrawable;

        return _drawable->GetDrawable();
	}

	float ParticleModule::GetTransparency()
	{
        FetchInputSlotValue(TRANSPARENCY);
        if (_transparency->IsEmpty()) return 1; // defaults
        return _transparency->GetFloat();
	}

	float ParticleModule::GetLife()
	{
        FetchInputSlotValue(LIFE);
        if (_life->IsEmpty()) return 2; // defaults
        return _life->GetFloat();
	}

	float ParticleModule::GetAngle()
	{
        FetchInputSlotValue(ANGLE);
        if (_angle->IsEmpty()) return 90; // defaults
        return _angle->GetFloat();
	}

	float ParticleModule::GetVelocity()
	{
        FetchInputSlotValue(VELOCITY);
        if (_velocity->IsEmpty()) return 0; // defaults
        return _velocity->GetFloat();
	}

	float ParticleModule::GetRotation()
	{
        FetchInputSlotValue(ROTATION);
        if (_rotation->IsEmpty()) return 0; // defaults
        return _rotation->GetFloat();
	}

	void ParticleModule::GetSize(float& x, float& y)
	{
        FetchInputSlotValue(SIZE);
        if (_size->IsEmpty())
        {
            x = 1.0f;
            y = 1.0f / GetScope()->GetFloat(ScopePayload::DRAWABLE_ASPECT_RATIO);
        }
        else if (_size->ElementsCount() == 1)
        {
            x = _size->GetFloat();
            y = _size->GetFloat() / GetScope()->GetFloat(ScopePayload::DRAWABLE_ASPECT_RATIO);
        }
        else if (_size->ElementsCount() == 2) 
        {
            x = _size->Get(0);
            y = _size->Get(1);
        }
        else
        {
            x = 1.0f;
            y = 1.0f / GetScope()->GetFloat(ScopePayload::DRAWABLE_ASPECT_RATIO);
        }
	}

	void ParticleModule::GetColor(float& r, float& g, float& b, float& a)
	{
        FetchInputSlotValue(COLOR);
        if (_color->IsEmpty())
        {
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
            a = 1.0f;
            return; // defaults
        }
        r = _color->Get(0);
        g = _color->Get(1);
        b = _color->Get(2);
        a = 1.0f;
	}

	void ParticleModule::GetStartPosition(float& x, float& y)
	{
        FetchInputSlotValue(OFFSET);
        if (_offset->IsEmpty())
        {
            x = 0;
            y = 0;
            return;
        }
        x = _offset->Get(0);
        y = _offset->Get(1);
	}

	bool ParticleModule::GetTarget(float& x, float& y)
	{
        FetchInputSlotValue(TARGET);
        if (_target->IsEmpty())
            return false;
        x = _target->Get(0);
        y = _target->Get(1);
        return true;
	}

	bool ParticleModule::GetPosition(float& x, float& y)
	{
        FetchInputSlotValue(POSITION);
        if (_position->IsEmpty())
            return false;
        x = _position->Get(0);
        y = _position->Get(1);
        return true;
	}

	void ParticleModule::SetDefaultDrawable(IParticleDrawable* defaultDrawable)
	{
        _defaultDrawable = defaultDrawable;
	}

	bool ParticleModule::IsParticleModule() const
	{
        return true;
	}

	void ParticleModule::DefineSlots()
	{
        _drawable = (DrawableValue*)CreateInputSlot(DRAWABLE, new DrawableValue);
        _offset = CreateInputSlot(OFFSET);
        _life = CreateInputSlot(LIFE);
        _velocity = CreateInputSlot(VELOCITY);
        _gravity = CreateInputSlot(GRAVITY);
        _rotation = CreateInputSlot(ROTATION);
        _target = CreateInputSlot(TARGET);
        _color = CreateInputSlot(COLOR);
        _transparency = CreateInputSlot(TRANSPARENCY);
        _angle = CreateInputSlot(ANGLE);
        _mass = CreateInputSlot(MASS);
        _size = CreateInputSlot(SIZE);
        _position = CreateInputSlot(POSITION);

        _rotation->SetFlavour(NumericalValue::ANGLE);
        _angle->SetFlavour(NumericalValue::ANGLE);
	}
	
}
