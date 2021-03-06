#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);

	for (int x = 0;  x < size; x++)
	{
		for (int y = 0 ; y < size; y++)
		{
			mask(x, y) = 0;
		}
	}
	mask(size / 2, size / 2) = 1;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

	float weight = sum(mask);
	QVector < Channel > cha;
	if (image->format() == QImage::Format_Indexed8)
	{
		cha.push_back(LChannel);
	}
	else
	{
		cha.push_back(RChannel);
		cha.push_back(GChannel);
		cha.push_back(BChannel);
	}
	float result[3];

	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
		{
			for (int c = 0; c<cha.size(); c++)
			{
				math::matrix<float> accum = join(mask, getWindow(x, y, mask.rowno(), cha[c], mode));

				result[c] = sum(accum);
				if (weight != 0) {
					result[c] /= weight;
				}
				if (result[c]  > 255) {
					 result[c] = 255; 
				}
				else if (result[c] < 0) {
					result[c] = 0;
				}

			}

			if (image->format() == QImage::Format_Indexed8) {
				newImage->setPixel(x, y, result[0]);
			}
			else {
				newImage->setPixel(x, y, qRgb(result[0], result[1], result[2]));
			}
		}
	}

    return newImage;
}

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			C(x, y) = A(x, y) * B(x, y);
		}
	}

    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;
	for (int i = 0; i < A.rowno(); i++)
	{
		for (int j = 0; j < A.colno(); j++)
		{
			sum += A(i, j);
		}
	}

	return sum;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			C(size - x, size - y) = A(x, y);
		}

	}

    return C;
}
