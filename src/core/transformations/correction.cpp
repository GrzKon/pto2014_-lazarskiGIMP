#include "correction.h"

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    float shift  = getParameter("shift").toFloat();
    float factor = getParameter("factor").toFloat();
    float gamma  = getParameter("gamma").toFloat();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

	for (int i = 0; i < 256; i++) {
		Correction::LUT[i] = (int)((((float)(pow((double)i, (double)gamma))) * factor) + shift);
		if (Correction::LUT[i] < 0) {
			Correction::LUT[i] = 0;
		}
		else if (Correction::LUT[i] > 255) {
			Correction::LUT[i] = 255;
		}
	}

	for (int x = 0; x<width; x++)
		for (int y = 0; y<height; y++)
		{
		QRgb pixel = image->pixel(x, y); // Getting the pixel(x,y) value

		int r = qRed(pixel);    // Get the 0-255 value of the R channel
		int g = qGreen(pixel);  // Get the 0-255 value of the G channel
		int b = qBlue(pixel);   // Get the 0-255 value of the B channel
		r = Correction::LUT[r];
		g = Correction::LUT[g];
		b = Correction::LUT[b];
		QColor newPixel = QColor(r, g, b);
		newImage->setPixel(x, y, newPixel.rgb());
		}
	


    return newImage;
}
