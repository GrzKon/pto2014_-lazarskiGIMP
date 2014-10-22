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
		QHash<int, int>::const_iterator Rite;
		QHash<int, int>::const_iterator Gite;
		QHash<int, int>::const_iterator Bite;
		double Rdis[256] = { 0 };
		double Gdis[256] = { 0 };
		double Bdis[256] = { 0 };
		double bot = width * height;
		Rite = Rcha.constBegin();
		Gite = Gcha.constBegin();
		Bite = Bcha.constBegin();
		while (Rite != Rcha.constEnd())
		{
			Rdis[Rite.key()] += Rite.value() / bot;
			Rite++;
		}
		while (Gite != Gcha.constEnd())
		{
			Gdis[Gite.key()] += Gite.value() / bot;
			Gite++;
		}
		while (Bite != Bcha.constEnd())
		{
			Bdis[Bite.key()] += Bite.value() / bot;
			Bite++;
		}
		for (int i = 1; i < 256; i++)
		{
			Rdis[i] += Rdis[i - 1];
		}
		for (int i = 1; i < 256; i++)
		{
			Gdis[i] += Gdis[i - 1];
		}
		for (int i = 1; i < 256; i++)
		{
			Bdis[i] += Bdis[i - 1];
		}
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					QRgb pix;
					QColor npix;
					int red;
					int gre;
					int blu;
					int Rszu;
					int Gszu;
					int Bszu;
					pix = image->pixel(x, y);
					red = qRed(pix);
					gre = qGreen(pix);
					blu = qBlue(pix);
					Rszu = Rdis[red] * 255;
					Gszu = Gdis[gre] * 255;
					Bszu = Bdis[blu] * 255;
					npix = QColor((int)Rszu, (int)Gszu, (int)Bszu);
					newImage->setPixel(x, y, npix.rgb());
				}
			}
		
	}

    return newImage;
}

