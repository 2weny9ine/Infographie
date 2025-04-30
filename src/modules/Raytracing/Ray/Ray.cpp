#include "Ray.h"
#include <glm/geometric.hpp>

Ray::Ray(const glm::vec3& orig, const glm::vec3& dir)
	: origin(orig)
	, direction(glm::normalize(dir))
{}
