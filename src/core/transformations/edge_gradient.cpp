#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
	return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
	return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
	PNM* newImage = new PNM(image->width(), image->height(), image->format());

	int width = image->width();
	int height = image->height();

	PNM* imageX = horizontalDetection();
	PNM* imageY = verticalDetection();

	if (image->format() == QImage::Format_Indexed8)
	{
		int q;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				QRgb xPixel = imageX->pixel(x, y);
				QRgb yPixel = imageY->pixel(x, y);
				q = (int)sqrt(pow(qGray(xPixel), 2.0) + pow(qGray(yPixel), 2.0));
				newImage->setPixel(x, y, q);
			}
		}
	}
	else
	{
		int r, g, b;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				QRgb xPixel = imageX->pixel(x, y);
				QRgb yPixel = imageY->pixel(x, y);
				r = (int)sqrt(pow(qRed(xPixel), 2.0) + pow(qRed(yPixel), 2.0));
				g = (int)sqrt(pow(qGreen(xPixel), 2.0) + pow(qGreen(yPixel), 2.0));
				b = (int)sqrt(pow(qBlue(xPixel), 2.0) + pow(qBlue(yPixel), 2.0));
				newImage->setPixel(x, y, QColor(r, g, b).rgb());
			}
		}
	}

	return newImage;

	return newImage;
}

