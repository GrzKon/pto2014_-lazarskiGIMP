#include "histogram.h"

#include <QDebug>
#include <QPainter>

#include <cmath>

Histogram::Histogram(QImage* image)
{
    R = new QHash<int, int>;
    G = new QHash<int, int>;
    B = new QHash<int, int>;
    L = new QHash<int, int>;
    generate(image);
}

Histogram::~Histogram()
{
    delete R;
    delete G;
    delete B;
    delete L;
}

void Histogram::generate(QImage* image)
{
   // qDebug() << Q_FUNC_INFO << "Not implemented yet!";

	int width = image->width();
	int height = image->height();
	//int i = 0;

		for (int x = 0; x<width; x++)
			for (int y = 0; y<height; y++)
			{
			QRgb pixel = image->pixel(x, y); // Getting the pixel(x,y) value

			int r = qRed(pixel);    // Get the 0-255 value of the R channel
			int g = qGreen(pixel);  // Get the 0-255 value of the G channel
			int b = qBlue(pixel);   // Get the 0-255 value of the B channel
			int l = qGray(pixel);
			R->insert(r, R->value(r) + 1);
			G->insert(g, G->value(g) + 1);
			B->insert(b, B->value(b) + 1);
			L->insert(l, L->value(l) + 1);
			//get(RChannel).insert(i, r);

			//QColor newPixel = QColor(r, g, b);
			//newImage->setPixel(x, y, r + g + b);
			}

	
}

/** Returns the maximal value of the histogram in the given channel */
int Histogram::maximumValue(Channel selectedChannel = RGB)
{
    qDebug() << Q_FUNC_INFO << "Not implemented yet!";
	int max = 0;

	if (selectedChannel == RGB) {
		if (maximumValue(RChannel) > max) {
			max = maximumValue(RChannel);
		}
		if (maximumValue(GChannel) > max) {
			max = maximumValue(GChannel);
		}
		if (maximumValue(BChannel) > max) {
			max = maximumValue(BChannel);
		}
	}
	else {
		QHash<int, int>* hash = get(selectedChannel);
		QHash<int, int>::iterator i = hash->begin();
		for (i = hash->begin(); i != hash->end(); ++i) {
			if (i.value() > max) {
				max = i.value();
			}
		}

	}

	return max;
}


/** Returns a pointer to the given channel QHash<int, int> */
QHash<int, int>* Histogram::get(Channel channel = LChannel)
{
    if (channel==LChannel) return L;
    if (channel==RChannel) return R;
    if (channel==GChannel) return G;
    if (channel==BChannel) return B;

    return 0;
}

/**
 *  Returns a 255 by 100 QImage containing a Histogram for the given channel.
 *  The background is transparent (Alpha 0, RGB=255) */
QImage Histogram::getImage(Channel channel = LChannel, QBrush pen = Qt::gray)
{
    // Create blank QImage and fill it with transparent background:
    QImage histImage(255, 100, QImage::Format_ARGB32);
    histImage.fill(0);
    QPainter painter(&histImage);
    painter.setBrush(Qt::transparent); 
    painter.setPen(Qt::transparent);
    painter.drawRect(0,0,255,100);

    // Calculate the aspect ratio using the maximal value of the color histograms
    int maximum = (channel == LChannel ? maximumValue(LChannel) :  maximumValue(RGB));
    float ratio = 100.0/float(maximum);

    // Preparing the painter:
    painter.setBrush(pen);
    painter.setPen(pen.color());

    int h;
    // Draw histogram
    QHash<int, int>* hist = get(channel);
    QHash<int, int>::const_iterator cit = hist->begin();

    while (cit != hist->end())
    {
        h = 100 - floor(ratio*cit.value());
        painter.drawLine(cit.key(), h, cit.key(), 100);
        ++cit;
    }

    return histImage;
}
