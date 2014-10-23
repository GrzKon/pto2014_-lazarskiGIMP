#include "blur_uniform.h"

BlurUniform::BlurUniform(PNM* img) :
    Convolution(img)
{
}

BlurUniform::BlurUniform(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> BlurUniform::getMask(int size, Mode)
{
    math::matrix<float> mask(size, size);

	for (int i = size; i-- != 0;) {
		for (int j = size; j-- != 0;) {
			mask(i, j) = 1;
		}
	}

    return mask;
}
