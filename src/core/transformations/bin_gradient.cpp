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
	
	int top = 0;
	int bot = 0;
	int tym = 0;
	int grx = 0;
	int gry = 0;
	int gra = 0;
	for (int x = 0; x<width; ++x)
	{
		for (int y = 0; y<height; ++y)
		{
			QRgb pix1 = image->pixel(x - 1, y);
			QRgb pix2 = image->pixel(x + 1, y);
			grx= qGray(pix2) - qGray(pix1);
			pix1 = image->pixel(x, y - 1);
			pix2 = image->pixel(x, y + 1);
			gry = qGray(pix2) - qGray(pix1);
			if (grx >= gry) 
			{
				gra = grx;
			}
			else if (grx < gry)
			{
				gra = gry;
			}
			QRgb pix = image->pixel(x, y);
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


