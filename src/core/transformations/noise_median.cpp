#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
Convolution(img, iv)
{
}

PNM* NoiseMedian::transform()
{
	int width = image->width();
	int height = image->height();

	PNM* newImage = new PNM(width, height, image->format());

	if (image->format() == QImage::Format_RGB32) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {

				QRgb pixel = getPixel(x, y, RepeatEdge);

				int r = trimValue(getMedian(x, y, RChannel));
				int g = trimValue(getMedian(x, y, GChannel));
				int b = trimValue(getMedian(x, y, BChannel));

				QColor newPixel = QColor(r, g, b);
				newImage->setPixel(x, y, newPixel.rgb());
			}
		}
	}
	else if (image->format() == QImage::Format_Mono) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				QRgb pixel = image->pixel(x, y);

				int v = qGray(pixel);
				v = trimValue(getMedian(x, y, LChannel));
				newImage->setPixel(x, y, v);
			}
		}
	}
	return newImage;
}

int NoiseMedian::trimValue(int value) {
	if (value > 255){
		value = 255;
	}
	else if (value < 0) {
		value = 0;
	}
	return value;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
	int radius = getParameter("radius").toInt();
	radius = radius * 2 + 1;
	math::matrix<float> window = getWindow(x, y, radius, channel, RepeatEdge);

	QVector<double> values;
	for (int i = 0; i < window.colno(); i++) {
		for (int j = 0; j < window.rowno(); j++) {
			values.append(window(i, j));
		}
	}

	qSort(values);

	return values.takeAt(values.size() / 2);
}
