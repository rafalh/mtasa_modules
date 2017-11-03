#pragma once

#include <cstdint>
#include <unordered_map>

template <typename T>
class HandleTable : public std::unordered_map<uint16_t, std::unique_ptr<T>>
{
public:
	typedef void *Handle;

	HandleTable(uint16_t Prefix) :
		m_Prefix(Prefix) {}

	T *Get(Handle h) const
	{
		uint16_t handlePrefix = reinterpret_cast<uintptr_t>(h) >> 16;
		if (handlePrefix != m_Prefix)
			return nullptr;

		uint16_t id = reinterpret_cast<uintptr_t>(h) & 0xFFFF;
		auto it = this->find(id);
		if (it == this->end())
			return nullptr;
		return it->second.get();
	}

	void Insert(Handle h, std::unique_ptr<T> ptr)
	{
		uint16_t id = reinterpret_cast<uintptr_t>(h) & 0xFFFF;
		this->insert(std::make_pair(id, std::move(ptr)));
	}

	void Remove(Handle h)
	{
		uint16_t id = reinterpret_cast<uintptr_t>(h) & 0xFFFF;
		this->erase(id);
	}

	Handle GenerateHandle()
	{
		while (this->find(m_NextId) != this->end())
			++m_NextId;

		Handle h = reinterpret_cast<Handle>((m_Prefix << 16) | m_NextId);
		++m_NextId;
		return h;
	}

private:
	uint16_t m_Prefix;
	uint16_t m_NextId = 1;
};

