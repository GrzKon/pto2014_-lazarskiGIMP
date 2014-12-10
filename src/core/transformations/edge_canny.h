#ifndef EDGECANNY_H
#define EDGECANNY_H

#include "convolution.h"

class EdgeCanny : public Convolution
{
public:
    EdgeCanny(PNM*);
    EdgeCanny(PNM*, ImageViewer*);

    virtual PNM* transform();


private:
	int kos[361];
	math::matrix<float> direction,
		power;
};

#endif // EDGECANNY_H
