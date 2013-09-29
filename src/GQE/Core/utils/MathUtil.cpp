#include <GQE/Core/utils/MathUtil.hpp>

namespace GQE
{
	void SetSeed(GQE::Uint32 theSeed)
	{
		srand(theSeed);
	}

	float Random(float theMin, float theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}
	int Random(int theMin, int theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}

	GQE::Uint32 Random(GQE::Uint32 theMin, GQE::Uint32 theMax)
	{
		return static_cast<double>(std::rand()) / RAND_MAX * theMax + theMin;
	}
}
