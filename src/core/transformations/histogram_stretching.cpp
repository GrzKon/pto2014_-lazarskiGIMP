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
		Histogram* his = image->getHistogram();
		his->get(Histogram::RChannel);
		QList<QHash<int, int>> cha;
		QHash<int, int> Rcha = *his->get(Histogram::RChannel);
		QHash<int, int> Gcha = *his->get(Histogram::GChannel);
		QHash<int, int> Bcha = *his->get(Histogram::BChannel);
		cha.append(Rcha);
		cha.append(Gcha);
		cha.append(Bcha);
		int Rmin = 255;
		int Rmax = 0;
		int Gmin = 255;
		int Gmax = 0;
		int Bmin = 255;
		int Bmax = 0;
		QHash<int, int>::const_iterator Rite;
		QHash<int, int>::const_iterator Gite;
		QHash<int, int>::const_iterator Bite;
		Rite = Rcha.constBegin();
		Gite = Gcha.constBegin();
		Bite = Bcha.constBegin();
		while (Rite != Rcha.constEnd())
		{
			int r = Rite.key();
			if (r > 0)
			{
				if (r < Rmin) Rmin = Rite.key();
				if (r > Rmax) Rmax = Rite.key();
			}
			Rite++;
		}
		while (Gite != Gcha.constEnd())
		{
			int r = Gite.key();
			if (r > 0)
			{
				if (r < Gmin) Gmin = Gite.key();
				if (r > Gmax) Gmax = Gite.key();
			}
			Gite++;
		}
		while (Bite != Bcha.constEnd())
		{
			int r = Bite.key();
			if (r > 0)
			{
				if (r < Bmin) Bmin = Bite.key();
				if (r > Bmax) Bmax = Bite.key();
			}
			Bite++;
		}
			double top = 255.0;
			double Rbot = Rmax - Rmin;
			double Rstr = top / Rbot;
			double Gbot = Gmax - Gmin;
			double Gstr = top / Gbot;
			double Bbot = Bmax - Bmin;
			double Bstr = top / Bbot;
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					QRgb pix;
					QColor npix;
					int red;
					int gre;
					int blu;
					double Rszu;
					double Gszu;
					double Bszu;
					pix = image->pixel(x, y);
					red = qRed(pix);
					gre = qGreen(pix);
					blu = qBlue(pix);
					Rszu = Rstr*(red - Rmin);
					Gszu = Gstr*(gre - Gmin);
					Bszu = Bstr*(blu - Bmin);
					npix = QColor((int)Rszu, (int)Gszu, (int)Bszu);
					newImage->setPixel(x, y, npix.rgb());
				}
			}
		}
	return newImage;
}