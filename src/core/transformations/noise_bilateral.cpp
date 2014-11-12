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
	Mode mode = RepeatEdge;
	QRgb pixel;
	for (int x = 0; x<width; x++){
		for (int y = 0; y<height; y++){
			if (image->format() == QImage::Format_RGB32){
				pixel = getPixel(x, y, mode);
				int r = trimValue(calcVal(x, y, RChannel));
				int g = trimValue(calcVal(x, y, GChannel));
				int b = trimValue(calcVal(x, y, BChannel));
				QColor newPixel = QColor(r, g, b);
				newImage->setPixel(x, y, newPixel.rgb());
			}
			if (image->format() == QImage::Format_Indexed8){
				int temp = calcVal(x, y, LChannel);
				newImage->setPixel(x, y, temp);
			}
		}
	}
	return newImage;
}
int NoiseBilateral::trimValue(int value) {
	if (value > 255){
		value = 255;
	}
	else if (value < 0) {
		value = 0;
	}
	return value;
}
int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
	int pix = image->pixel(x, y);
	int size = radius * 2 + 1;
	int pixv = 0;
	math::matrix<float> win = getWindow(x, y, size, channel, Transformation::RepeatEdge);
	if (channel == Channel::LChannel)
	{
		pixv = qGray(pix);
	}
	else if (channel == Channel::RChannel)
	{
		pixv = qRed(pix);
	}
	else if (channel == Channel::GChannel)
	{
		pixv = qGreen(pix);
	}
	else if (channel == Channel::BChannel)
	{
		pixv = qBlue(pix);
	}

	double valc;
	double vals; 
	double mucl;
	double licz = 0;
	double	mian = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			valc = colorCloseness((int)win(i, j), pixv);
			vals = spatialCloseness(QPoint(x - radius, y - radius), QPoint(x, y));
			mucl = valc * vals;
			licz += (win(i, j) *mucl);
			mian += mucl;
		}
	}
	int return_val = 0;
	if (mian != 0){
		return_val = (int)(licz / mian);
	}

	return return_val;
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
	float E = 2.718281828459;

	return (float)(E - ((pow((double)(val1 - val2), 2.0)) / (2.0 * pow((double)sigma_r, 2.0))));
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
	float E = 2.718281828459;
	return (float)(E - ((pow(point1.x() - point2.x(), 2.0) + pow((point1.y() - point2.y()), 2.0)) / (2.0 * pow(sigma_d, 2.0))));
}
