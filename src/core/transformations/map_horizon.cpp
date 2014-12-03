#include "map_horizon.h"

#include "map_height.h"

MapHorizon::MapHorizon(PNM* img) :
    Transformation(img)
{
}

MapHorizon::MapHorizon(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* MapHorizon::transform()
{
    int width  = image->width(),
        height = image->height();

    double scale     = getParameter("scale").toDouble();
    int    sun_alpha = getParameter("alpha").toInt();
    int dx, dy;

    switch (getParameter("direction").toInt())
    {
    case NORTH: dx = 0; dy = -1; break;
    case SOUTH: dx = 0; dy = 1; break;
    case EAST:  dx = 1; dy = 0; break;
    case WEST:  dx = -1; dy = 0; break;
    default:
        qWarning() << "Unknown direction!";
        dx =  0;
        dy = -1;
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);
	PNM* mapHeight = MapHeight(image).transform();
	for (int x = 0; x<width; x++){
		for (int y = 0; y<height; y++){
			float alfa = 0.0;
			int ch = qRed(mapHeight->pixel(x, y));
			for (int n = x + dx, m = y + dy; n < width && m < height && n >= 0 && m >= 0; n += dx, m += dy){
				int rh = qRed(mapHeight->pixel(n, m));
				if (ch < rh){
					float dis = sqrt(pow(n - x, 2) + pow(m - y, 2)) * scale;
					float ra = atan((rh - ch) / dis) * 180. / 3.14;
					if (ra > alfa){
						alfa = ra;
					}
				}
			}
			float delta = alfa - sun_alpha;
			if (delta <= 0){
				newImage->setPixel(x, y, 255);
			}
			else{
				newImage->setPixel(x, y, cos(delta * 3.14 / 180.) * 255);
			}
		}
	}

    return newImage;
}
