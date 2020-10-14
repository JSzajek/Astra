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
	
	protected:
		Character(int id, double xTextureCoord, double yTextureCoord, double xMaxTextureCoord, double yMaxTextureCoord, double xOffset, double yOffset, double xSize, double ySize)
			: id(id), xTextureCoord(xTextureCoord), yTextureCoord(yTextureCoord), xMaxTextureCoord(xMaxTextureCoord), yMaxTextureCoord(yMaxTextureCoord),
				xOffset(xOffset), yOffset(yOffset), xSize(xSize), ySize(ySize)
		{
		}
	public:
		inline const int const Id() { return id; }

		inline const double const XTextureCoord() { return xTextureCoord; }
		inline const double const YTextureCoord() { return yTextureCoord; }
		inline const double const XMaxTextureCoord() { return xMaxTextureCoord; }
		inline const double const YMaxTextureCoord() { return yMaxTextureCoord; }
		

	};
}