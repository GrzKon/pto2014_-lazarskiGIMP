#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

	if (image->format() == QImage::Format_Indexed8)
	{
		Histogram* his = image->getHistogram();
		his->get(Histogram::LChannel);
		QHash<int, int> cha = *his->get(Histogram::LChannel);
		QHash<int, int>::const_iterator ite;
		double dis[256] = { 0 };
		double bot = width * height;
		ite = cha.constBegin();
		while (ite != cha.constEnd())
		{
			dis[ite.key()] += ite.value() / bot;
			ite++;
		}
		for (int i = 1; i < 256; i++)
		{
			dis[i] += dis[i - 1];
		}
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				QRgb pix = image->pixel(x, y);
				int qgre = qGray(pix);
				int szu = dis[qgre] * 255;
				newImage->setPixel(x, y, szu);

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
		for (QHash<int, int> cha : cha){
			QHash<int, int>::const_iterator ite;
			double dis[256] = { 0 };
			double bot = width * height;
			ite = cha.constBegin();
			while (ite != cha.constEnd())
			{
				dis[ite.key()] += ite.value() / bot;
				ite++;
			}
			for (int i = 1; i < 256; i++)
			{
				dis[i] += dis[i - 1];
			}
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
					int szu;
					if (ln == 0)
					{
						pix = image->pixel(x, y);
						red = qRed(pix);
						gre = qGreen(pix);
						blu = qBlue(pix);
						szu = dis[red] * 255;
						npix = QColor((int)szu, gre, blu);
					}
					if (ln == 1)
					{
						pix = image->pixel(x, y);
						tpix = newImage->pixel(x, y);
						red = qRed(tpix);
						gre = qGreen(pix);
						blu = qBlue(pix);
						szu = dis[gre] *255;
						npix = QColor(red, (int)szu, blu);
					}
					if (ln == 2)
					{
						pix = image->pixel(x, y);
						tpix = newImage->pixel(x, y);
						red = qRed(tpix);
						gre = qGreen(tpix);
						blu = qBlue(pix);
						szu = dis[blu] * 255;
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

