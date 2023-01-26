#include "FissionPCH.hpp"
#include "SphereShape.hpp"

namespace Fission {

	SphereShape::SphereShape(float InRadius)
	    : m_Radius(InRadius)
	{
		//m_InertiaTensor = glm::mat3(2.0f / 5.0f * InRadius * InRadius);
	}

}
