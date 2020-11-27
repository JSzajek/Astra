#include "ResourceManager.h"


namespace Astra::Graphics
{
	#define TrackPointer(list, pointer)\
	  if(list.find(pointer) != list.end()) { list[pointer]++; } \
	  else { list[pointer] = 1; } \

	ResourceManager::ResourceManager()
	{
	}

	const VertexArray* ResourceManager::TrackVertexArrayImpl(const VertexArray* vao)
	{
		TrackPointer(m_loaded, vao);
		return vao;
	}
	
	const Texture* ResourceManager::TrackTextureImpl(const Texture* texture)
	{
		TrackPointer(m_loaded, texture);
		return texture;
	}

	const ImageMaterial* ResourceManager::TrackImageMaterialImpl(const ImageMaterial* material)
	{
		TrackPointer(m_loaded, material);
		return material;
	}

	const ParticleMaterial* ResourceManager::TrackParticleMaterialImpl(const ParticleMaterial* material)
	{
		TrackPointer(m_loaded, material);
		return material;
	}

	const SkyboxMaterial* ResourceManager::TrackSkyboxMaterialImpl(const SkyboxMaterial* material)
	{
		TrackPointer(m_loaded, material);
		return material;
	}

	const TerrainMaterial* ResourceManager::TrackTerrainMaterialImpl(const TerrainMaterial* material)
	{
		TrackPointer(m_loaded, material);
		return material;
	}

	WaterMaterial* ResourceManager::TrackWaterMaterialImpl(WaterMaterial* material)
	{
		TrackPointer(m_loaded, material);
		return material;
	}
	
	FontType* ResourceManager::TrackFontTypeImpl(FontType* type)
	{
		TrackPointer(m_loaded, type);
		return type;
	}
	
	const FrameBuffer* ResourceManager::TrackFrameBufferImpl(const FrameBuffer* buffer)
	{
		TrackPointer(m_loaded, buffer);
		return buffer;
	}
	
	
}