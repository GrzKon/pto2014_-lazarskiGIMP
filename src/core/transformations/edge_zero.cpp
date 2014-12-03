#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
	int width = image->width(),
		height = image->height();

	int    size = getParameter("size").toInt();
	double sigma = getParameter("sigma").toDouble();
	int    t = getParameter("threshold").toInt();

	PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

	EdgeLaplaceOfGauss elof(image, getSupervisor());
	elof.setParameter("size", size);
	elof.setParameter("sigma", sigma);
	PNM* lof = elof.transform();
	int v = 128;
	if (image->format() == QImage::Format_Indexed8){
		for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				math::matrix<float> mask = elof.getWindow(x, y, size, Transformation::LChannel, Transformation::RepeatEdge);
				float minimum = 255.0, maximum = 0;
				for (int i = 0; i < size; i++){
					for (int j = 0; j < size; j++){
						if (mask(i, j) < minimum){
							minimum = mask(i, j);
						}
						if (mask(i, j) > maximum){ 
							maximum = mask(i, j);
						}
					}
				}
				if (minimum<v - t && maximum>v + t){
					newImage->setPixel(x, y, qGray(lof->pixel(x, y)));
				}
				else {
					newImage->setPixel(x, y, 0);
				}
			}
		}
	}
	else{
		for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				math::matrix<float> maskR = elof.getWindow(x, y, size, Transformation::RChannel, Transformation::RepeatEdge);
				math::matrix<float> maskG = elof.getWindow(x, y, size, Transformation::GChannel, Transformation::RepeatEdge);
				math::matrix<float> maskB = elof.getWindow(x, y, size, Transformation::BChannel, Transformation::RepeatEdge);
				float minimumR = 255.0, maximumR = 0, minimumG = 255.0, maximumG = 0, minimumB = 255.0, maximumB = 0;
				for (int i = 0; i < size; i++){
					for (int j = 0; j < size; j++){
						if (maskR(i, j) < minimumR){
							minimumR = maskR(i, j);
						}
						if (maskR(i, j) > maximumR){
							maximumR = maskR(i, j);
						}
					}
				}
				for (int i = 0; i < size; i++){
					for (int j = 0; j < size; j++){
						if (maskG(i, j) < minimumG){
							minimumG = maskG(i, j);
						}
						if (maskG(i, j) > maximumG){
							maximumG = maskG(i, j);
						}
					}
				}
				for (int i = 0; i < size; i++){
					for (int j = 0; j < size; j++){
						if (maskB(i, j) < minimumB){
							minimumB = maskB(i, j);
						}
						if (maskB(i, j) > maximumB){
							maximumB = maskB(i, j);
						}
					}
				}
				int r, g, b;
				if (minimumR<v - t && maximumR>v + t){
					newImage->setPixel(x, y, maskR(size / 2, size / 2));
				} 
				else if (minimumG<v - t && maximumG>v + t){
					newImage->setPixel(x, y, maskG(size / 2, size / 2));
				}
				else if (minimumB<v - t && maximumB>v + t){
					newImage->setPixel(x, y, maskB(size / 2, size / 2));
				}
				else {
					newImage->setPixel(x, y, 0);
				}
			}
		}
	}
	return newImage;
}