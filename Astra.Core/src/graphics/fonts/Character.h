#pragma once

namespace Astra::Graphics
{
	struct Character
	{
	private:
		int id;
		double xTextureCoord, yTextureCoord;
		double xMaxTextureCoord, yMaxTextureCoord;
		double xOffset, yOffset;
		double xSize, ySize;
		double xAdvance;
	public:
		Character(int id, double xTextureCoord, double yTextureCoord, double xMaxTextureCoord, double yMaxTextureCoord, double xOffset, double yOffset, double xSize, double ySize, double xAdvance)
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

		inline const double GetXTextureCoord() const { return xTextureCoord; }
		inline const double GetYTextureCoord() const { return yTextureCoord; }
		inline const double GetXMaxTextureCoord() const { return xMaxTextureCoord; }
		inline const double GetYMaxTextureCoord() const { return yMaxTextureCoord; }

		inline const double GetXOffset() const { return xOffset; }
		inline const double GetYOffset() const { return yOffset; }
		inline const double GetXSize() const { return xSize; }
		inline const double GetYSize() const { return ySize; }
		
		inline const double GetXAdvance() const { return xAdvance; }
	};
}