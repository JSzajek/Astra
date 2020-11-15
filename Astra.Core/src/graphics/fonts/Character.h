#pragma once

namespace Astra::Graphics
{
	struct Character
	{
	private:
		int id;
		float xTextureCoord, yTextureCoord;
		float xMaxTextureCoord, yMaxTextureCoord;
		float xOffset, yOffset;
		float xSize, ySize;
		float xAdvance;
	public:
		Character(int id, float xTextureCoord, float yTextureCoord, float xMaxTextureCoord, float yMaxTextureCoord, float xOffset, float yOffset, float xSize, float ySize, float xAdvance)
			: id(id), xTextureCoord(xTextureCoord), yTextureCoord(yTextureCoord), xMaxTextureCoord(xTextureCoord + xMaxTextureCoord), yMaxTextureCoord(yTextureCoord + yMaxTextureCoord),
				xOffset(xOffset), yOffset(yOffset), xSize(xSize), ySize(ySize), xAdvance(xAdvance)
		{
		}
	
		Character(const Character& other)
			: id(other.id), xTextureCoord(other.xTextureCoord), yTextureCoord(other.yTextureCoord), xMaxTextureCoord(other.xMaxTextureCoord), yMaxTextureCoord(other.yMaxTextureCoord),
				xOffset(other.xOffset), yOffset(other.yOffset), xSize(other.xSize), ySize(other.ySize), xAdvance(other.xAdvance)
		{
		}

		inline const int Id() const { return id; }

		inline const float GetXTextureCoord() const { return xTextureCoord; }
		inline const float GetYTextureCoord() const { return yTextureCoord; }
		inline const float GetXMaxTextureCoord() const { return xMaxTextureCoord; }
		inline const float GetYMaxTextureCoord() const { return yMaxTextureCoord; }

		inline const float GetXOffset() const { return xOffset; }
		inline const float GetYOffset() const { return yOffset; }
		inline const float GetXSize() const { return xSize; }
		inline const float GetYSize() const { return ySize; }

		inline const float GetXAdvance() const { return xAdvance; }
	};
}