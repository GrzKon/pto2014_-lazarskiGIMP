#include "blur_gaussian.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

	Mode mode = RepeatEdge;
	for (int i = 0; i<size; i++){
		for (int j = 0; j<size; j++){
			mask(i, j) = getGauss(i-radius, j-radius, sigma);
		}
	}

	return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{    
	float pi = 3.14159265359f;
	float e = 2.718281828459f;
	float g1 = (1 / (2 * pi*pow(sigma,2)));
	float p1 = pow(x ,2) + pow(y ,2);
	float p2 = (2 * pow(sigma, 2));
	float g2 = pow(e,(-(p1) / p2));
	float gauss = g1*g2;
return gauss;
}

