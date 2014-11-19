#include "bin_gradient.h"

BinarizationGradient::BinarizationGradient(PNM* img) :
Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
	int width = image->width();
	int height = image->height();

	PNM* newImage = new PNM(width, height, QImage::Format_Mono);

	int top = 0, bot = 0, tym = 0, grx = 0, gry = 0, gra = 0;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			QRgb pix, pix1, pix2;
			pix1 = getPixel(x - 1, y, RepeatEdge);
			pix2 = getPixel(x + 1, y, RepeatEdge);
			grx = qGray(pix2) - qGray(pix1);
			pix1 = getPixel(x, y - 1, RepeatEdge);
			pix2 = getPixel(x, y + 1, RepeatEdge);
			gry = qGray(pix2) - qGray(pix1);
			if (grx >= gry)
			{
				gra = grx;
			}
			else {
				gra = gry;
			}
			pix = getPixel(x, y, RepeatEdge);
			top += qGray(pix) * gra;
			bot += gra;
			tym = bot == 0 ? 0 : top / bot;
			if (qGray(pix) >= tym)
			{
				newImage->setPixel(x, y, Qt::color1);
			}
			else
			{
				newImage->setPixel(x, y, Qt::color0);
			}
		}
	}


	return newImage;
}


