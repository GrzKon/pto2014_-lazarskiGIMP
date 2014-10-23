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

	int current = 0;

	for (int i = 0; i < maskSize; i++) {
		for (int j = 0; j < maskSize; j++) {
			mask(i, j) = tmpMask.at(current).toDouble();
			if (normalize) {
				mask(i, j) = mask(i, j) / sum(mask);
			}

			current++;
		}
	}

    return convolute(mask, RepeatEdge);
}
