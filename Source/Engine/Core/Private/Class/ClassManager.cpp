#include "Class/ClassObject.h"
#include "Class/ClassManager.h"

void SClassManager::Init() noexcept
{
#if WITH_DEBUG_CODE
	std::sort(mClassObjects.begin(), mClassObjects.end(), [](const std::tuple<uint64_t, IClassObjectInterface*>& _left, const std::tuple<uint64_t, IClassObjectInterface*>& _right) {return std::get<0>(_left) < std::get<0>(_right); });
	if (mClassObjects.size() > 1)
	{
		for (size_t i = 1; i != mClassObjects.size(); ++i)
			CHECK(std::get<0>(mClassObjects[i]) != std::get<0>(mClassObjects[i - 1]));
	}
#endif
}

void SClassManager::Clear() noexcept
{
}

void ClassObjectDetail::RegistClassObject(uint64_t _typeHash, IClassObjectInterface* _classObject) noexcept
{
	SClassManager::Get().RegistClassObject(_typeHash, _classObject);
}
