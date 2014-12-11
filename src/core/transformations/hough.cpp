#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::transform()
{   
    int thetaDensity = getParameter("theta_density").toInt();

	int width = image->width(),
		height = image->height();

	
	PNM* tempImage = new PNM(width, height, QImage::Format_Indexed8);

	tempImage = ConversionGrayscale::ConversionGrayscale(image).transform();

	if (!getParameter("skip_edge_detection").toBool()) {
		tempImage = EdgeLaplacian::EdgeLaplacian(tempImage).transform();
	}

	int pmax = (int)sqrt(pow((double)height, 2.0) + pow((double)width, 2.0));
	int thetaSize = 180 * thetaDensity;

	PNM* newImage = new PNM(thetaSize, pmax * 2 + 1, QImage::Format_Indexed8);

	math::matrix<int> hough(thetaSize, pmax * 2 + 1);

	for (int i = 0; i < thetaSize; i++) {
		for (int j = 0, len = pmax * 2 + 1; j < len; j++) {
			hough(i, j) = 0;
		}
	}

	double theta, rho;
	int max = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			QRgb pixel = tempImage->pixel(i, j);
			if (qGray(pixel) > 0) {
				for (int k = 0; k < thetaSize; k++) {
					theta = ((double)k * 3.14159265359) / (180.0 * thetaDensity);
					rho = i*cos(theta) + j*sin(theta);
					hough(k, (int)floor(rho + pmax))++;
					if (hough(k, (int)floor(rho + pmax)) > max) {
						max = hough(k, (int)floor(rho + pmax));
					}
				}
			}
		}
	}


	for (int i = 0; i < thetaSize; i++) {
		for (int j = 0, len = pmax * 2 + 1; j < len; j++) {
			newImage->setPixel(i, j, (int)((hough(i, j) * (256 / max))));
		}
	}


	return newImage;
}
