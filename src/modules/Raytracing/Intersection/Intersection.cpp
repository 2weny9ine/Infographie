#include "Intersection.h"
#include <limits>

Intersection::Intersection()
	: hit(false)
	, distance(std::numeric_limits<float>::infinity())
	, point(0.0f)
	, normal(0.0f)
	, object(nullptr)
{}
