#pragma once

#include "serialization/TalosIFileProvider.h"

namespace Talos
{

	class TextureRegionDrawable;
	class PolylineDrawable;
	class ShadedDrawable;

	class IFactory
	{
	public:
		virtual ~IFactory() {}
		virtual IFileProvider* CreateFileProvider(const char* fileName) = 0;
		
		virtual TextureRegionDrawable* CreateTextureRegionDrawable(const char* region) { return nullptr; }
		virtual PolylineDrawable* CreatePolylineDrawable(const char* region) { return nullptr; }
		virtual ShadedDrawable* CreateShadedDrawable(const char* shdrFileName) { return nullptr; }
	protected:
	};
	
}