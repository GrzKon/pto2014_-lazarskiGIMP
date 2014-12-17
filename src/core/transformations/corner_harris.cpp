#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);
	math::matrix<double>
		imx(width, height),	
		imy(width, height), 
		imxy(width, height), 
		corncan(width, height), 
		cornnonsup(width, height);
	PNM* tempImage = ConversionGrayscale(image).transform();
	BlurGaussian blurGauss(tempImage);
	blurGauss.setParameter("size", 3);
	blurGauss.setParameter("sigma", 3.6);
	tempImage = blurGauss.transform();
	EdgeSobel edgsob(tempImage);
	math::matrix<float>* xgra = edgsob.rawHorizontalDetection();
	math::matrix<float>* ygra = edgsob.rawVerticalDetection();
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			double xgrapi = (*xgra)(x, y), ygrapi = (*ygra)(x, y);
			imx(x, y) = xgrapi * xgrapi;
			imy(x, y) = ygrapi * ygrapi;
			imxy(x, y) = xgrapi * ygrapi;
		}
	}
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
			{
				corncan(x, y) = 0;
				cornnonsup(x, y) = 0;
			}
			else 
			{
				double sx = 0, sy = 0, sxy = 0;
				for (int k = -1; k < 2; k++){
					for (int j = -1; j < 2; j++){
						double g_val = BlurGaussian::getGauss(k, j, sigma);
						sx += imx(x + k, y + j)*g_val;
						sy += imy(x + k, y + j)*g_val;
						sxy += imxy(x + k, y + j)*g_val;
					}
				}
				sx /= sigma_weight;
				sy /= sigma_weight;
				sxy /= sigma_weight;
				math::matrix<double> H(2, 2);
				H(0, 0) = sx; H(1, 1) = sy; H(0, 1) = sxy; H(1, 0) = sxy;
				double r = H(0, 0)*H(1, 1) - H(0, 1)*H(1, 0) - k_param*pow(H(0, 0) + H(1, 1), 2);
				corncan(x, y) = (r > threshold) ? r : 0;
			}
		}
	}
	bool maknonsup = true;
	while (maknonsup)
	{
		maknonsup = false;
		for (int x = 1; x < width - 1; x++)
		{
			for (int y = 1; y < height - 1; y++)
			{
				double val = corncan(x, y);
				if (val > corncan(x - 1, y - 1) && val > corncan(x - 1, y) && val > corncan(x - 1, y + 1) && val > corncan(x, y - 1) && val > corncan(x, y + 1) && val > corncan(x + 1, y - 1) && val > corncan(x + 1, y) && val > corncan(x + 1, y + 1))
				{
					cornnonsup(x, y) = val;
				}
				else 
				{
					if (val > 0)
					{
						maknonsup = true;
					}
					cornnonsup(x, y) = 0;
				}
			}
		}
		corncan = cornnonsup;
	}
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (corncan(x, y) == 0){
				newImage->setPixel(x, y, 0);
			}
			else
			{
				newImage->setPixel(x, y, 1);
			}
		}
	}
    return newImage;
}
