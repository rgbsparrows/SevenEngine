#include "Core/Class/ClassObject.h"
#include "Core/Class/ClassManager.h"

void ClassObjectDetail::RegistClassObject(uint64_t _typeHash, const IClassObjectInterface* _classObject) noexcept
{
	SClassManager::Get().RegistClassObject(_typeHash, _classObject);
}
