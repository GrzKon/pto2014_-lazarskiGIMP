#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"

#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());
	PNM* tempImage, *binImage;
	EdgeLaplacian* edla = new EdgeLaplacian(image);
	edla->setParameter("size", 3);
	tempImage = edla->transform();
	BinarizationGradient* bigra = new BinarizationGradient(tempImage);
	binImage = bigra->transform();
	Hough* hough = new Hough(binImage);
	hough->setParameter("theta_density", 3);
	hough->setParameter("skip_edge_detection", true);
	tempImage = hough->transform();
	QPainter* qPainter = new QPainter(newImage);
	qPainter->setPen(Qt::red);
	int i1 = 0, i2, j1, j2;
	for (int theta = 0; theta < tempImage->width(); theta++)
	{
		for (int rho = 0; rho < tempImage->height(); rho++)
		{
			QRgb pixel = tempImage->pixel(theta, rho);
			int valPixel = qGray(pixel);
			if (valPixel > threshold)
			{
				int real_rho = rho - tempImage->height() / 2;
				double rtheta = (theta / 3.0)*M_PI / 180.0;
				double ctheta = cos(rtheta), stheta = sin(rtheta);
				j1 = round(real_rho / stheta);
				i2 = newImage->width() - 1;
				j2 = round((real_rho - (newImage->width() - 1)*ctheta) / stheta);
				qPainter->drawLine(i1, j1, i2, j2);
			}
		}
	}
	if (!drawWholeLines == true)
	{
		for (int i = 0; i < newImage->width(); i++)
			for (int j = 0; j < newImage->height(); j++)
			{
			if (qRed(binImage->pixel(i, j)) == 0)
				{
					newImage->setPixel(i, j, image->pixel(i, j));
				}
			}
	}
    return newImage;
}
