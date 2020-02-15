#pragma once

template<typename T>
class SingletonManager
{
public:
	static T& Instance() noexcept(std::is_nothrow_constructible<T>::value)
	{
		static T instance;

		return instance;
	}

protected:
	SingletonManager() noexcept = default;
	SingletonManager(const SingletonManager&) = delete;
	SingletonManager& operator=(const SingletonManager&) = delete;

	virtual ~SingletonManager() = default;

};