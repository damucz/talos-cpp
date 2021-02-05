#pragma once

#include "TalosTypes.h"
#include <vector>

namespace Talos
{

	class IFileProvider
	{
	public:
		class EmitterData
		{
		public:
			virtual ~EmitterData() {}
		};

		class ModuleData
		{
		public:
			virtual ~ModuleData() {}
			virtual const char* GetClass() = 0;
			virtual int GetIndex() = 0;
			virtual float GetFloat(const char* key, float defaultVal = 0) = 0;
			virtual int GetInt(const char* key, int defaultVal = 0) = 0;
			virtual bool GetBool(const char* key, bool defaultVal = false) = 0;
			virtual const char* GetString(const char* key) = 0;
			virtual std::vector<Point> GetPoints(const char* key) = 0;
			virtual std::vector<ColorPoint> GetColorPoints(const char* key) = 0;
			virtual ModuleData* GetSubData(const char* key) = 0;
		};

		class ConnectionData
		{
		public:
			virtual ~ConnectionData() {}
			int moduleFrom = 0;
			int moduleTo = 0;
			int slotFrom = 0;
			int slotTo = 0;
		};

		virtual ~IFileProvider() {}
		
		virtual std::vector<EmitterData*> GetEmitters() = 0;
		virtual std::vector<ModuleData*> GetModules(EmitterData* emitter) = 0;
		virtual std::vector<ConnectionData*> GetConnections(EmitterData* emitter) = 0;
	protected:
	};

}
