#pragma once
#ifndef ARRAY_H
#define ARRAY_H
#include"Assets.h"
template<typename T>
class array
{
private:
	T* arr;
	size_t size;
	uitype capacity;

	void deepCopy(T*& dest, const T* src, size_t size, size_t capacity) {

		/*if (dest != nullptr) {
			delete[] dest;
			dest = nullptr;
		}*/

		if (src == nullptr || size == 0) {
			return;
		}
		dest = new T[capacity];
		for (size_t i = 0; i < size; ++i) {
			dest[i] = src[i];
		}
	}

public:
	array()
	{
		this->arr = nullptr;
		this->size = 0;
		this->capacity = vector_capacity_unit;
	}

	array(T* arr, uitype size)
	{
		this->size = size;
		uitype i = 0;
		for (; vector_capacity_unit * i < size; ++i) {}
		this->capacity = vector_capacity_unit * i;
		copy(this->arr, arr, size, this->capacity);
	}

	array(const array<T>& arr_)
	{
		this->size = arr_.size;
		this->capacity = arr_.capacity;
		this->deepCopy(this->arr, arr_.arr, arr_.size, arr_.capacity);
	}

	array(uitype size)
	{
		this->size = size;
		uitype i = 0;
		for (; vector_capacity_unit * i < size; ++i) {}
		this->capacity = vector_capacity_unit * i;
		this->arr = new T[capacity];
	}

	~array()
	{
		if (this->arr != nullptr)
		{
			delete[] this->arr;
			this->arr = nullptr;
			this->size = 0;
			this->capacity = 0;
		}
	}

	void set_size(uitype size)
	{
		this->size = size;
	}

	uitype get_size() const
	{
		return this->size;
	}

	uitype get_capacity() const
	{
		return this->capacity;
	}

	void set_arr(T* arr)
	{
		if (this->arr != nullptr)
		{
			delete[] this->arr;
		}
		this->arr = arr;
	}

	T* get_arr() const
	{
		return this->arr;
	}

	void push_back(T val)
	{
		if (this->capacity == this->size)
		{
			this->capacity += vector_capacity_unit;
			T* new_vect = new T[this->capacity];
			for (uitype counter = 0; counter < this->size; ++counter)
			{
				new_vect[counter] = this->arr[counter];
			}
			delete[] this->arr;
			new_vect[this->size] = val;
			this->arr = new_vect;
		}
		else
		{
			if (this->arr == nullptr)
			{
				this->arr = new T[this->capacity];
			}
			this->arr[size] = val;
		}
		++this->size;
	}

	void pop_back()
	{
		assert(this->arr != nullptr);
		assert(this->size != 0);
		if (this->capacity == this->size + vector_capacity_unit)
		{
			if (this->capacity != vector_capacity_unit)
			{
				this->capacity -= vector_capacity_unit;
			}
			T* new_vect = new T[this->capacity];
			for (uitype counter = 0; counter < this->size - 1; ++counter)
			{
				new_vect[counter] = this->arr[counter];
			}
			delete[] this->arr;
			this->arr = new_vect;
		}
		--this->size;
	}

	array<array<T>> reshape(itype rows_num, itype cols_num)
	{
		array<array<T>> new_arr(rows_num);
		array<T> row(cols_num);
		for (itype i = 0; i < rows_num; ++i)
		{
			for (uitype j = 0; j < cols_num; ++j)
			{
				row[j] = this->arr[i * cols_num + j];
			}
			new_arr[i] = row;
		}
		return new_arr;
	}

	array<T> operator()(itype beg, itype end)
	{
		itype size = abs(end - beg);
		array<T> a(size);
		for (itype i = 0; i < size; ++i)
		{
			a[i] = this->arr[beg + i];
		}
		return a;
	}

	void clear()
	{
		if (this->arr != nullptr)
		{
			delete[] this->arr;
		}
		this->capacity = 20;
		this->size = 0;
		this->arr = new T[this->capacity];
	}

	void operator=(const array<T>& arr_) 
	{
		if (this != &arr_) 
		{
			deepCopy(arr, arr_.arr, arr_.size, arr_.capacity);
			size = arr_.size;
			capacity = arr_.capacity;
		}
	}

	T& operator[](const itype ind)
	{
		assert(abs(ind) <= this->size);
		assert(this->arr != nullptr);
		return ind >= 0 ? this->arr[ind] : this->arr[this->size + ind];
	}

	const T& operator[](const itype ind) const
	{
		assert(abs(ind) < this->size);
		assert(this->arr != nullptr);
		return ind >= 0 ? this->arr[ind] : this->arr[this->size + ind];
	}

	class iterator
	{
	private:
		T* current;

	public:
		iterator(T* ptr)
		{
			current = ptr;
		}

		iterator& operator++()
		{
			++current;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++current;
			return temp;
		}

		bool operator!=(const iterator& other) const
		{
			return current != other.current;
		}

		bool operator==(const iterator& other) const
		{
			return !this->operator!=(other);
		}

		T& operator*()
		{
			return *current;
		}

		T* get_ptr() const
		{
			return current;
		}
	};

	iterator begin()
	{
		return iterator(this->arr);
	}

	iterator end()
	{
		return iterator(this->arr + size);
	}
};
#endif // !ARRAY_H