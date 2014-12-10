#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
	int width = img->width(),
		height = img->height();

	direction = math::matrix<float>(width, height);
	power = math::matrix<float>(width, height);
	for (int i = 0; i < 361; i++)
	{
		if ((i >= 0 && i < 23) || (i >= 158 && i < 203) || (i >= 338 && i < 361))
		{
			kos[i] = 0;
		}
		if ((i >= 23 && i < 68) || (i >= 203 && i < 248))
		{
			kos[i] = 1;
		}
		if ((i >= 68 && i < 113) || (i >= 248 && i < 293))
		{
			kos[i] = 2;
		}
		if ((i >= 113 && i < 158) || (i >= 293 && i < 338))
		{
			kos[i] = 3;
		}
	}
}

PNM* EdgeCanny::transform()
{
    int width  = image->width(),
        height = image->height();

    int upper_thresh = getParameter("upper_threshold").toInt(),
        lower_thresh = getParameter("lower_threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

	PNM* tmpImage = new PNM(width, height, QImage::Format_Indexed8);

	tmpImage = ConversionGrayscale::ConversionGrayscale(image).transform();

	BlurGaussian blurGaussian(tmpImage);
	blurGaussian.setParameter("sigma", 1.6);
	blurGaussian.setParameter("size", 3);
	tmpImage = blurGaussian.transform();
	EdgeSobel sobel(tmpImage);
	math::matrix<float>* Mx = sobel.rawHorizontalDetection();
	math::matrix<float>* My = sobel.rawVerticalDetection();
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			newImage->setPixel(i, j, PIXEL_VAL_MIN);
			power(i, j) = sqrt(pow((*Mx)(i, j), 2) + pow((*My)(i, j), 2));
			direction(i, j) = (int)((atan2((*My)(i, j), (*Mx)(i, j)) / (3.14159265359)) * 180 + 360) % 360;
		}
	}
	double ne1, ne2;
	std::list<QPoint> initialPoints;
	for (int i = 1; i < width - 1; i++)
	{
		for (int j = 1; j < height - 1; j++)
		{
			int pos = direction(i, j);
			double m = power(i, j);
			int point1n = 1;
			QPoint point1;
			if (point1n == 1)
			{
				if (kos[pos] == 0)
				{
					point1 = QPoint(0, -1);
				}
				else if (kos[pos] == 1)
				{
					point1 = QPoint(1, -1);
				}
				else if (kos[pos] == 2)
				{
					point1 = QPoint(-1, 0);
				}
				else if (kos[pos] == 3)
				{
					point1 = QPoint(-1, -1);
				}
			}
			else
			{
				if (kos[pos] == 0)
				{
					point1 = QPoint(0, 1);
				}
				else if (kos[pos] == 1)
				{
					point1 = QPoint(-1, 1);
				}
				else if (kos[pos] == 2)
				{
					point1 = QPoint(1, 0);
				}
				else if (kos[pos] == 3)
				{
					point1 = QPoint(1, 1);
				}
			}
			QPoint point2;
			int point2n = 2;
			if (point2n == 1)
			{
				if (kos[pos] == 0)
				{
					point2 = QPoint(0, -1);
				}
				else if (kos[pos] == 1)
				{
					point2 = QPoint(1, -1);
				}
				else if (kos[pos] == 2)
				{
					point2 = QPoint(-1, 0);
				}
				else if (kos[pos] == 3)
				{
					point2 = QPoint(-1, -1);
				}
			}
			else
			{
				if (kos[pos] == 0)
				{
					point2 = QPoint(0, 1);
				}
				else if (kos[pos] == 1)
				{
					point2 = QPoint(-1, 1);
				}
				else if (kos[pos] == 2)
				{
					point2 = QPoint(1, 0);
				}
				else if (kos[pos] == 3)
				{
					point2 = QPoint(1, 1);
				}
			}
			ne1 = power(i + point1.x(), j + point1.y());
			ne2 = power(i + point2.x(), j + point2.y());
			if (m > ne1 && m > ne2 && m > upper_thresh)
			{
				newImage->setPixel(i, j, PIXEL_VAL_MAX);
				initialPoints.push_back(QPoint(i, j));
			}
		}
	}
	while (!initialPoints.empty())
	{
		QPoint point = initialPoints.back();
		initialPoints.pop_back();
		int pos = direction(point.x(), point.y());
		QPoint tneipoin[7] = {
			QPoint(-1, 0),
			QPoint(-1, 1),
			QPoint(0, -1),
			QPoint(0, 1),
			QPoint(1, -1),
			QPoint(1, 0),
			QPoint(1, 1),
		};
		for (int i = 1; i <= 2; i++)
		{
			QPoint dirVe;
			int point2n = 2;
			if ( i == 1)
			{
				if (kos[pos] == 0)
				{
					dirVe = QPoint(0, -1);
				}
				else if (kos[pos] == 1)
				{
					dirVe = QPoint(1, -1);
				}
				else if (kos[pos] == 2)
				{
					dirVe = QPoint(-1, 0);
				}
				else if (kos[pos] == 3)
				{
					dirVe = QPoint(-1, -1);
				}
			}
			else
			{
				if (kos[pos] == 0)
				{
					dirVe = QPoint(0, 1);
				}
				else if (kos[pos] == 1)
				{
					dirVe = QPoint(-1, 1);
				}
				else if (kos[pos] == 2)
				{
					dirVe = QPoint(1, 0);
				}
				else if (kos[pos] == 3)
				{
					dirVe = QPoint(1, 1);
				}
			}
			QPoint cPoint = point;
			while (true)
			{
				QPoint npoint = QPoint(cPoint.x() + dirVe.x(), cPoint.y() + dirVe.y());
				if (npoint.x() == width - 1 || npoint.x() == 0 || npoint.y() == height - 1 || npoint.y() == 0) break;
				if (newImage->pixel(npoint.x(), npoint.y()) == PIXEL_VAL_MAX) break;
				if (power(npoint.x(), npoint.y()) < lower_thresh) break;
				if (kos[(int)direction(npoint.x(), npoint.y())] != kos[pos]) break;
				bool maxM = true;
				double mag = power(npoint.x(), npoint.y());
				for (int i = 0; i < 7; i++)
				{
					QPoint tnei(npoint.x() + tneipoin[i].x(), npoint.y() + tneipoin[i].y());
					if (tnei.x() == cPoint.x() && tnei.y() == cPoint.y()) continue;
					if (kos[(int)direction(tnei.x(), tnei.y())] != kos[pos]) continue;
					if (power(tnei.x(), tnei.y()) >= mag)
					{
						maxM = false;
						break;
					}
				}
				if (!maxM) break;
				newImage->setPixel(npoint.x(), npoint.y(), PIXEL_VAL_MAX);
				cPoint = npoint;
			}
		}
	}

    return newImage;
}
