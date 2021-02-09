#include "TalosParticle.h"
#include "TalosIEmitter.h"
#include "TalosScopePayload.h"
#include "TalosIParticleDrawable.h"
#include "modules/TalosParticleModule.h"
#include "modules/TalosEmitterModule.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	
	void Particle::Init(IEmitter* emitterReference)
	{
		float seed = emitterReference->GetRandom().NextFloat();
		Init(emitterReference, seed);
	}

	void Particle::Init(IEmitter* emitterReference, float seed)
	{
		_emitterReference = emitterReference;
		ParticleModule* particleModule = emitterReference->GetParticleModule();

		_seed = seed;

		// inner variable defaults
		_alpha = 0.0f;

		particleModule->UpdateScopeData(this);

		_life = particleModule->GetLife(); // really makes more sense like this, for deterministic purposes

		particleModule->GetStartPosition(_positionX, _positionY);	// offset
		emitterReference->GetEffectPosition(_spawnPositionX, _spawnPositionY);

		_durationAtInit = emitterReference->GetAlpha();
	}

	void Particle::Update(float delta)
	{
		if (_alpha == 1.0f)
			return;

		if (_emitterReference == nullptr)
			return;

		//scope data
		ParticleModule* particleModule = _emitterReference->GetParticleModule();
		if (particleModule == nullptr)
			return;

		_life = particleModule->GetLife(); // maybe should remove this

		_alpha += delta / _life;
		if (_alpha > 1.0f)
			_alpha = 1.0f;

		ApplyAlpha(_alpha, delta);
	}

	void Particle::ApplyAlpha(float alpha, float delta)
	{
		ParticleModule* particleModule = _emitterReference->GetParticleModule();
		if (particleModule == nullptr)
			return;

		particleModule->UpdateScopeData(this);

		//update variable values
		float targetX, targetY;
		float angle = 0;
		if (particleModule->GetTarget(targetX, targetY))
		{
			angle = (float)(atan2f(targetY - _positionY, targetX - _positionX) * 180.0f / (float)M_PI);
			if (angle < 0)
				angle += 360.0f;
		}
		else
			angle = particleModule->GetAngle();		// do we take angle or target

		float velocity = particleModule->GetVelocity();
		_transparency = particleModule->GetTransparency();

		if (_emitterReference->GetEmitterModule()->IsAligned())
			_rotation = angle + particleModule->GetRotation();
		else
			_rotation = particleModule->GetRotation();

		_drawable = particleModule->GetDrawable(); // important to get drawable before size
		_emitterReference->GetScope()->Set(ScopePayload::DRAWABLE_ASPECT_RATIO, _drawable->GetAspectRatio());

		particleModule->GetSize(_sizeX, _sizeY);
		float positionOverrideX, positionOverrideY;
		// perform inner operations
		if (particleModule->GetPosition(positionOverrideX, positionOverrideY))
		{
			_positionX = positionOverrideX;
			_positionY = positionOverrideY;
		}
		else
		{
			_positionX += cosf((float)(angle / 180.0f * M_PI)) * velocity * delta;
			_positionY += sinf((float)(angle / 180.0f * M_PI)) * velocity * delta;
		}
		particleModule->GetColor(_colorR, _colorG, _colorB, _colorA);
	}

	float Particle::GetX() const
	{
		if (_emitterReference->GetEmitterModule()->IsAttached())
		{
			float x, y;
			_emitterReference->GetEffectPosition(x, y);
			return x + _positionX;
		}
		else
			return _spawnPositionX + _positionX;
	}

	float Particle::GetY() const
	{
		if (_emitterReference->GetEmitterModule()->IsAttached())
		{
			float x, y;
			_emitterReference->GetEffectPosition(x, y);
			return y + _positionY;
		}
		else
			return _spawnPositionY + _positionY;
	}

	float Particle::GetEmitterAlpha() const
	{
		return _emitterReference->GetAlpha();
	}

	IEmitter* Particle::GetEmitter() const
	{
		return _emitterReference;
	}

	void Particle::NotifyKill()
	{
		ParticleModule* particleModule = _emitterReference->GetParticleModule();
		if (particleModule == nullptr) return;
		particleModule->UpdateScopeData(this);

		_drawable = particleModule->GetDrawable();

		if (_drawable != nullptr) {
			_drawable->NotifyDispose(this);
		}
	}
	
}
