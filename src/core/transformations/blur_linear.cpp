#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);

	//int r = (maskSize / 2) + 1, 
		int current = 0;

	for (int i = 0; i < maskSize; i++) {
		for (int j = 0; j < maskSize; j++) {
			mask(i, j) = tmpMask.at(current).toDouble();
			current++;
			if (normalize) {
				mask(i, j) = mask(i, j) / sum(mask);
			}
		}
	}

    return convolute(mask, RepeatEdge);
}
