#pragma once
#include <memory>

typedef unsigned int uint;

#define _deleteArray(x) if(x) {delete[] x; x = 0;}
#define _delete(x) if(x) {delete x; x = 0;}

template <typename T> class Storage
{
	T** data{};
	T** temp{};

	uint capacity = 0;
	uint size = 0;

	uint iteratorOld = 0;
	uint iterator = 0;



	void Temp()
	{
		_deleteArray(temp);
		temp = new T * [capacity] {};
		if(data)
			memcpy(temp, data, capacity * 8);
	}

	uint FindSpace()
	{
		iteratorOld = iterator;
		while (iterator < capacity)
		{
			if (data[iterator] == 0)
				return iterator++;
			iterator++;
		}

		iterator = 0;
		while (iterator < iteratorOld)
		{

			if (!data[iterator])
			{
				iteratorOld = 0;
				return iterator++;
			}
			iterator++;

		}
	}

public:


	T* operator [] (uint id)
	{
		return data[id];
	}

	void Resize(uint capacity)
	{
		if (data)
		{

			Temp();
			_deleteArray(data);
			data = new T * [capacity] {};
			memcpy(data, temp, this->capacity * 8);
			_deleteArray(temp);

		}

		else
		{
			data = new T * [capacity] {};
		}
		this->capacity = capacity;
	}

	void DeleteElement(uint id)
	{
		_delete(data[id]);
		size--;
	}

	uint Set(T* value)
	{
		uint id;
		data[id = FindSpace()] = value;
		size++;
		return id;
	}

	uint Set(T value)
	{
		uint id;
		T* i = new T;
		*i = value;

		data[id = FindSpace()] = i;
		size++;
		return id;
	}

	uint Set()
	{
		uint id;
		T* i = new T;
		data[id = FindSpace()] = i;
		size++;
		return id;
	}

	T* Get(uint id)
	{
		return data[id];
	}

	uint GetSize() { return size; }

};
