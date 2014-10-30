#include "bin_manual.h"

BinarizationManual::BinarizationManual(PNM* img) :
    Transformation(img)
{
}

BinarizationManual::BinarizationManual(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationManual::transform()
{
    int threshold = getParameter("threshold").toInt();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);


	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++)
		{
			int pixel = image->pixel(x, y); // Getting the pixel(x,y) value

			newImage->setPixel(x, y, qGray(pixel) > threshold ? Qt::color1 : Qt::color0);
		}
	}


    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return newImage;
}




