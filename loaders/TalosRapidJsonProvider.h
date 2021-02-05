#pragma once

#include "serialization/TalosIFileProvider.h"
#include <rapidjson/document.h>

namespace Talos
{

	class RapidJsonProvider : public IFileProvider
	{
	public:
		class RJEmitterData : public EmitterData
		{
		public:
			RJEmitterData(rapidjson::GenericValue<rapidjson::UTF8<>>& e);
			rapidjson::GenericValue<rapidjson::UTF8<>>& emitter;
		};

		class RJModuleData : public ModuleData
		{
		public:
			RJModuleData(rapidjson::GenericValue<rapidjson::UTF8<>>& m);
			rapidjson::GenericValue<rapidjson::UTF8<>>& module;

			const char* GetClass() override;
			int GetIndex() override;
			float GetFloat(const char* key, float defaultVal = 0) override;
			int GetInt(const char* key, int defaultVal = 0) override;
			bool GetBool(const char* key, bool defaultVal = false) override;
			const char* GetString(const char* key) override;
			std::vector<Point> GetPoints(const char* key) override;
			std::vector<ColorPoint> GetColorPoints(const char* key) override;

			ModuleData* GetSubData(const char* key) override;
		};

		class RJConnectionData : public ConnectionData
		{
		public:
			RJConnectionData(rapidjson::GenericValue<rapidjson::UTF8<>>& c);
		};
		
		RapidJsonProvider(const char* fileName);
		~RapidJsonProvider();
		
		std::vector<EmitterData*> GetEmitters() override;
		std::vector<ModuleData*> GetModules(EmitterData* emitter) override;
		std::vector<ConnectionData*> GetConnections(EmitterData* emitter) override;
		
	protected:
		rapidjson::Document _document;
	};
	
}
