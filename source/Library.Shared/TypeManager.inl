#include "TypeManager.h"

namespace Library
{
#pragma region Registry
	template<typename T>
	inline void TypeManager::Register()
	{
		const IdType typeId = T::TypeIdClass();
		const TypeInfo& typeInfo = T::TypeInfo();

		if (typeInfo.parentTypeId != Attributed::TypeIdClass() && !mRegistry.ContainsKey(typeInfo.parentTypeId))
		{
			throw std::runtime_error("Parent type is not registered.");
		}

		auto [it, isNew] = mRegistry.Insert({ typeId, typeInfo });

		if (!isNew)
		{
			throw std::runtime_error("Type registered more than once.");
		}
	}

	inline float TypeManager::RegistryLoadFactor()
	{
		return mRegistry.LoadFactor();
	}
#pragma endregion Registry

#pragma region Global Registry Function
	template<typename T>
	inline void RegisterType()
	{
		if (!TypeManager::Instance()) throw std::runtime_error("TypeManager instance null.");

		TypeManager::Instance()->Register<T>();
	}
#pragma endregion Global Registry Function
}