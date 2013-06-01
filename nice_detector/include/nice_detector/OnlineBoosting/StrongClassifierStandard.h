#ifndef STRONG_CLASSIFIER_STANDARD_H
#define STRONG_CLASSIFIER_STANDARD_H

#include "StrongClassifier.h"

namespace nice_detector{
class StrongClassifierStandard  : public StrongClassifier
{
public:

	StrongClassifierStandard(int numBaseClassifier, int numWeakClassifier,
		Size patchSize, bool useFeatureExchange = false, int iterationInit = 0); 

	virtual ~StrongClassifierStandard();
	
	bool update(ImageRepresentation *image, Rect ROI, int target, float importance = 1.0);

private:

	bool *m_errorMask;
	float *m_errors;

};
}

#endif