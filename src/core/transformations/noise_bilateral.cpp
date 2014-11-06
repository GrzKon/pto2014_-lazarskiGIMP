#include "noise_bilateral.h"

NoiseBilateral::NoiseBilateral(PNM* img) :
Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
	int width = image->width();
	int height = image->height();

	PNM* newImage = new PNM(width, height, image->format());

	sigma_d = getParameter("sigma_d").toInt();
	sigma_r = getParameter("sigma_r").toInt();
	radius = sigma_d;

	qDebug() << Q_FUNC_INFO << "Not implemented yet!";

	return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
	int width = image->width();
	int height = image->height();
	
	float up = 0.0;
	float down = 0.0;
	float part;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			
			part = colorCloseness(image->pixel(i, j), image->pixel(x, y)) * spatialCloseness(QPoint(i, j), QPoint(x, y));

			up += image->pixel(i, j) * part;
			down += part;
		}
	}

	return (int) floor(up / down);
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
	float E = 2.718281828459;

	return (float)(E - ((pow((double)(val1 - val2), 2.0)) / (2.0 * pow((double)sigma_r, 2.0))));
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
	float E = 2.718281828459;

	return (float)(E - ((pow((double)(point1.x - point2.x), 2.0) + pow((double)(point1.y - point2.y), 2.0)) / (2.0 * pow((double)sigma_d, 2.0))));
}
