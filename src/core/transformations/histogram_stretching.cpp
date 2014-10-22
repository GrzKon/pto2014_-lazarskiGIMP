#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

	if (image->format() == QImage::Format_Indexed8)
	{
		int min = 255;
		int max = 0;
		Histogram* his = image->getHistogram();
		his->get(Histogram::LChannel);
		QHash<int, int> cha = *his->get(Histogram::LChannel);
		QHash<int, int>::const_iterator ite;
		ite = cha.constBegin();
		while (ite != cha.constEnd()){
			if (ite.key() > 0)
			{
				if (ite.key() < min) min = ite.key();
				if (ite.key() > max) max = ite.key();
			}
			ite++;
		}
		double top = 255.0;
		double bot = max - min;
		double str = top / bot;
		for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				QRgb strpix = image->pixel(x, y);
				int grapix = qGray(strpix);
				grapix = str*(grapix - min);
				newImage->setPixel(x, y, grapix);
			}

		}
	}
	else
	{
		int ln = 0;
		Histogram* his = image->getHistogram();
		his->get(Histogram::RChannel);
		QList<QHash<int, int>> cha;
		QHash<int, int> Rcha = *his->get(Histogram::RChannel);
		QHash<int, int> Gcha = *his->get(Histogram::GChannel);
		QHash<int, int> Bcha = *his->get(Histogram::BChannel);
		cha.append(Rcha);
		cha.append(Gcha);
		cha.append(Bcha);
		for (QHash<int, int> cha : cha)
		{
			int min = 255;
			int max = 0;
			QHash<int, int>::const_iterator ite;
			ite = cha.constBegin();
			while (ite != cha.constEnd())
			{
				int r = ite.key();
				if (r > 0)
				{
					if (r < min) min = ite.key();
					if (r > max) max = ite.key();
				}
				ite++;
			}
			double top = 255.0;
			double bot = max - min;
			double str = top / bot;
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					QRgb pix;
					QRgb tpix;
					QColor npix;
					int red;
					int gre;
					int blu;
					double szu;
					if (ln == 0)
					{
						pix = image->pixel(x, y);
						red = qRed(pix);
						gre = qGreen(pix);
						blu = qBlue(pix);
						szu = str *(red - min);
						npix = QColor((int)szu, gre, blu);
					}
					if (ln == 1)
					{
						pix = image->pixel(x, y);
						tpix = newImage->pixel(x, y);
						red = qRed(tpix);
						gre = qGreen(pix);
						blu = qBlue(pix);
						szu = str *(gre - min);
						npix = QColor(red, (int)szu, blu);
					}
					if (ln == 2)
					{
						pix = image->pixel(x, y);
						tpix = newImage->pixel(x, y);
						red = qRed(tpix);
						gre = qGreen(tpix);
						blu = qBlue(pix);
						szu = str *(blu - min);
						npix = QColor(red, gre, (int)szu);
					}
					newImage->setPixel(x, y, npix.rgb());
				}
			}
			ln++;
		}
	}
	return newImage;
}