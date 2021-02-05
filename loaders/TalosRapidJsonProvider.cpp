#include "TalosRapidJsonProvider.h"
#include <assert.h>

namespace Talos
{
	RapidJsonProvider::RJEmitterData::RJEmitterData(rapidjson::GenericValue<rapidjson::UTF8<>>& e)
		: emitter(e)
	{
	}

	RapidJsonProvider::RJModuleData::RJModuleData(rapidjson::GenericValue<rapidjson::UTF8<>>& m)
		:  module(m)
	{
	}

	const char* RapidJsonProvider::RJModuleData::GetClass()
	{
		return module["class"].GetString();
	}

	int RapidJsonProvider::RJModuleData::GetIndex()
	{
		return module["index"].GetInt();
	}

	float RapidJsonProvider::RJModuleData::GetFloat(const char* key, float defaultVal)
	{
		if (!module.HasMember(key) || !module[key].IsNumber()) return defaultVal;
		return (float)module[key].GetDouble();
	}

	int RapidJsonProvider::RJModuleData::GetInt(const char* key, int defaultVal)
	{
		if (!module.HasMember(key) || !module[key].IsInt()) return defaultVal;
		return module[key].GetInt();
	}

	bool RapidJsonProvider::RJModuleData::GetBool(const char* key, bool defaultVal)
	{
		if (!module.HasMember(key) || !module[key].IsBool()) return defaultVal;
		return module[key].GetBool();
	}

	const char* RapidJsonProvider::RJModuleData::GetString(const char* key)
	{
		return module[key].GetString();
	}

	std::vector<Point> RapidJsonProvider::RJModuleData::GetPoints(const char* key)
	{
		std::vector<Point> points;
		if (module.HasMember(key))
		{
			auto& p = module[key];
			for (rapidjson::SizeType i = 0; i < p.Size(); i++)
			{
				points.push_back({(float)p[i]["x"].GetDouble(), (float)p[i]["y"].GetDouble()});
			}
		}

		return points;
	}

	std::vector<ColorPoint> RapidJsonProvider::RJModuleData::GetColorPoints(const char* key)
	{
		std::vector<ColorPoint> colorPoints;
		if (module.HasMember(key))
		{
			auto& p = module[key];
			for (rapidjson::SizeType i = 0; i < p.Size(); i++)
			{
				colorPoints.push_back({ (float)p[i]["r"].GetDouble(), (float)p[i]["g"].GetDouble(),
					(float)p[i]["b"].GetDouble(), (float)p[i]["pos"].GetDouble() });
			}
		}

		return colorPoints;
	}

	IFileProvider::ModuleData* RapidJsonProvider::RJModuleData::GetSubData(const char* key)
	{
		if (module.HasMember(key))
			return new RJModuleData(module[key]);
		assert(false);
		return nullptr;
	}

	RapidJsonProvider::RJConnectionData::RJConnectionData(rapidjson::GenericValue<rapidjson::UTF8<>>& c)
	{
		if (c.HasMember("moduleFrom"))
			moduleFrom = c["moduleFrom"].GetInt();
		if (c.HasMember("moduleTo"))
			moduleTo = c["moduleTo"].GetInt();
		if (c.HasMember("slotFrom"))
			slotFrom = c["slotFrom"].GetInt();
		if (c.HasMember("slotTo"))
			slotTo = c["slotTo"].GetInt();
	}

	RapidJsonProvider::RapidJsonProvider(const char* fileName)
	{
		FILE* f = fopen(fileName, "rb");
		assert(f);
		if (f)
		{
			fseek(f, 0, SEEK_END);
			long size = ftell(f);
			fseek(f, 0, SEEK_SET);

			char* json = new char[size+1];
			assert(fread(json, 1, size, f) == (size_t)size);
			json[size] = 0;
			fclose(f);

			_document.Parse<rapidjson::kParseDefaultFlags>(json);
			assert(_document.IsObject());

			delete[] json;
		}
	}

	RapidJsonProvider::~RapidJsonProvider()
	{
	}

	std::vector<IFileProvider::EmitterData*> RapidJsonProvider::GetEmitters()
	{
		assert(_document["emitters"].IsArray());

		std::vector<EmitterData*> list;
		auto& e = _document["emitters"];
		for (rapidjson::SizeType i = 0; i < e.Size(); i++)
		{
			list.push_back(new RJEmitterData(e[i]));
		}

		return list;
	}

	std::vector<IFileProvider::ModuleData*> RapidJsonProvider::GetModules(EmitterData* emitter)
	{
		auto* e = (RJEmitterData*)emitter;
		assert(e->emitter["modules"].IsArray());

		std::vector<ModuleData*> list;
		auto& m = e->emitter["modules"];
		for (rapidjson::SizeType i = 0; i < m.Size(); i++)
		{
			list.push_back(new RJModuleData(m[i]));
		}

		return list;
	}

	std::vector<IFileProvider::ConnectionData*> RapidJsonProvider::GetConnections(EmitterData* emitter)
	{
		auto* e = (RJEmitterData*)emitter;
		assert(e->emitter["modules"].IsArray());

		std::vector<ConnectionData*> list;
		auto& m = e->emitter["connections"];
		for (rapidjson::SizeType i = 0; i < m.Size(); i++)
		{
			list.push_back(new RJConnectionData(m[i]));
		}

		return list;
	}
}
