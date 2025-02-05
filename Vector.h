#pragma once
#ifndef VECTOR_H
#define VECTOR_H
#include"Assets.h"
#include"Array.h"

template<typename T>
class matrix;

/// <summary>
/// Mathematical model of the vector
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class vector
{
private:
	T* vect;
	size_t size;
	uitype capacity;

	/*void copy(T* &dest, const T* source, const size_t size_, const uitype capacity)
	{
		assert(size_ > 0);
		assert(capacity > size_);
		assert(source != nullptr);
		dest = new T[capacity];
		size_t counter = 0;
		while (counter < size_)
		{
			dest[counter] = source[counter++];
		}
	}*/

public:
	vector()
	{
		this->vect = nullptr;
		this->size = 0;
		this->capacity = vector_capacity_unit;
	}

	vector(T* vect, size_t size)
	{
		this->size = size;
		uitype i = 0;
		for (; vector_capacity_unit * i < size; ++i){}
		this->capacity = vector_capacity_unit * i;
		copy(this->vect, vect, size, this->capacity);
	}

	vector(const array<T>& arr)
	{
		this->size = arr.get_size();
		this->capacity = arr.get_capacity();
		this->vect = new T[this->capacity];
		for (itype i = 0; i < this->size; ++i)
		{
			this->vect[i] = arr[i];
		}
	}

	vector(const vector<T>& vec_)
	{
		this->size = vec_.size;
		this->capacity = vec_.capacity;
		copy(this->vect, vec_.vect, vec_.size, vec_.capacity);
	}

	vector(uitype size)
	{
		this->size = size;
		uitype i = 0;
		for (; vector_capacity_unit * i < size; ++i) {}
		this->capacity = vector_capacity_unit * i;
		this->vect = new T[capacity];
	}

	~vector()
	{
		if (this->vect != nullptr && size != 0)
		{
			delete[] this->vect;
			this->vect = nullptr;
			this->size = 0;
			this->capacity = 0;
		}
	}

	void set_size(size_t size)
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

	void set_vect(T* vect)
	{
		if (this->vect != nullptr)
		{
			delete[] this->vect;
		}
		this->vect = vect;
	}

	T* get_vect() const
	{
		return this->vect;
	}

	vector<T> operator+(const T shift)
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] += shift;
		}
		return vector<T>(vect, this->size);
	}

	const vector<T> operator+(const T shift) const
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] += shift;
		}
		return vector<T>(vect, this->size);
	}

	vector<T> operator-(const T shift)
	{
		return this->operator+((-1) * shift);
	}

	const vector<T> operator-(const T shift) const
	{
		return this->operator+((-1) * shift);
	}

	vector<T> operator+(const vector<T>& vec_)
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] += vec_.vect[ind];
		}
		return vector<T>(vect, this->size);
	}

	const vector<T> operator+(const vector<T>& vec_) const
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] += vec_.vect[ind];
		}
		return vector<T>(vect, this->size);
	}

	vector<T> operator-(const vector<T>& vec_)
	{
		return this->operator+(vec_ * (-1));
	}

	const vector<T> operator-(const vector<T>& vec_) const
	{
		return this->operator+(vec_ * (-1));
	}

	vector<T> operator*(T scale)
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] *= scale;
		}
		return vector<T>(vect, this->size);
	}

	vector<T> operator*(T scale) const
	{
		T* vect = new T[this->size];
		copy(vect, this->vect, this->size, this->capacity);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			vect[ind] *= scale;
		}
		return vector<T>(vect, this->size);
	}

	vector<T> operator/(const T shift)
	{
		return this->operator*((1 / shift));
	}

	T operator*(const vector<T>& vec_)
	{
		assert(this->size == vec_.size);
		T res = 0;
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			res += this->vect[ind] * vec_.vect[ind];
		}
		return res;
	}

	/*matrix<T> operator*(const matrix<T>& matr_)
	{
		assert(this->size == matr_.get_rows_num());
		matrix<T> result();
	}*/

	void operator+=(const T shift)
	{
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			this->vect[ind] += shift;
		}
	}

	void operator-=(const T shift)
	{
		this->operator+=((-1) * shift);
	}

	void operator+=(const vector<T>& vec_)
	{
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			this->vect[ind] += vec_.vect[ind];
		}
	}

	void operator-=(const vector<T>& vec_)
	{
		this->operator+=((-1) * vec_);
	}

	void operator*=(const T scale)
	{
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			this->vect[ind] *= scale;
		}
	}

	void operator/=(const T scale)
	{
		this->operator*=((1 / scale));
	}

	void set_val_by_ind(const itype ind, const T val)
	{
		assert(abs(ind) < this->size);
		assert(this->vect != nullptr);
		if (ind < 0)
		{
			this->vect[this->size + ind] = val;
		}
		else
		{
			this->vect[ind] = val;
		}
	}

	T& operator[](const itype ind)
	{
		assert(abs(ind) <= this->size);
		assert(this->vect != nullptr);
		return ind >= 0 ? this->vect[ind] : this->vect[this->size + ind];
	}

	const T& operator[](const itype ind) const
	{
		assert(abs(ind) < this->size);
		assert(this->vect != nullptr);
		return ind >= 0 ? this->vect[ind] : this->vect[this->size + ind];
	}

	void operator=(const vector<T>& vec_)
	{
		if (this->size != 0)
		{
			delete[] this->vect;
		}
		this->size = vec_.size;
		this->capacity = vec_.capacity;
		copy(this->vect, vec_.vect, vec_.size, vec_.capacity);
	}

	bool operator==(const vector<T> vec_) const
	{
		assert(this->size == vec_.size);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			if (this->vect[ind] != vec_.vect[ind])
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const vector<T> vec_) const
	{
		return !this->operator==(vec_);
	}

	bool operator>(const vector<T> vec_) const
	{
		assert(this->size == vec_->size);
		for (size_t ind = 0; ind < this->size; ++ind)
		{
			if (this->vect[ind] <= vec_.vect[ind])
			{
				return false;
			}
		}
		return true;
	}

	bool operator>=(const vector<T> vec_) const
	{
		return this->operator>(vec_) && this->operator==(vec_);
	}

	bool operator<(const vector<T> vec_) const
	{
		return !this->operator>=(vec_);
	}

	bool operator<=(const vector<T> vec_) const
	{
		return this->operator>(vec_);
	}

	void push_back(T val)
	{
		if (this->capacity == this->size)
		{
			this->capacity += vector_capacity_unit;
			T* new_vect = new T[this->capacity];
			size_t counter = 0;
			while (counter < this->size)
			{
				new_vect[counter] = this->vect[counter++];
			}
			delete[] this->vect;
			new_vect[this->size] = val;
			this->vect = new_vect;
		}
		else
		{
			if (this->vect == nullptr)
			{
				this->vect = new T[this->capacity];
			}
			this->vect[size] = val;
		}
		++this->size;
	}

	void pop_back()
	{
		assert(this->vect != nullptr);
		if (this->capacity == this->size + vector_capacity_unit)
		{
			if (this->capacity != vector_capacity_unit)
			{
				this->capacity -= vector_capacity_unit;
			}
			T* new_vect = new T[this->capacity];
			size_t counter = 0;
			while (counter < this->size - 1)
			{
				new_vect[counter] = this->vect[counter++];
			}
			delete[] this->vect;
			this->vect = new_vect;
		}
		--this->size;
	}

	void clear()
	{
		if (this->vect != nullptr)
		{
			delete[] this->vect;
		}
		this->capacity = 20;
		this->size = 0;
		this->vect = new T[this->capacity];
	}

	void normalize()
	{
		T square_len = 0;
		for (size_t i = 0; i < this->size; ++i)
		{
			square_len += std::pow(this->vect[i], 2);
		}
		(*this) /= sqrt(square_len);
	}

	void z_normalize()
	{
		T m = this->mean();
		T s = this->std();
		for (size_t i = 0; i < this->size; ++i)
		{
			this->vect[i] = (this->vect[i] - m) / s;
		}
	}

	void normalize_by_max()
	{
		T max = this->vect[0];
		for (size_t i = 1; i < this->size; ++i)
		{
			max = this->vect[i] > max ? this->vect[i] : max;
		}
		for (size_t i = 0; i < this->size; ++i)
		{
			this->vect[i] /= max;
		}
	}

	/// <summary>
	/// Math expectation
	/// </summary>
	/// <returns></returns>
	T mean()
	{
		T math_expectation = 0;
		for (size_t i = 0; i < this->size; ++i)
		{
			math_expectation += this->vect[i];
		}
		math_expectation /= this->size;
		return math_expectation;
	}

	/// <summary>
	/// Standard deviation
	/// </summary>
	/// <returns></returns>
	T std()
	{
		T std = 0;
		T m = this->mean();
		for (size_t i = 0; i < this->size; ++i)
		{
			std += (this->vect[i] - m) * (this->vect[i] - m);
		}
		std /= this->size;
		std = sqrt(std);
		return std;
	}

	vector<ftype> elementwise(const vector<T>& vec_)
	{
		assert(vec_.size == this->size);
		vector<ftype> tmp(this->vect, this->size);
		for (uitype i = 0; i < this->size; ++i)
		{
			tmp[i] *= vec_[i];
		}
		return tmp;
	}

	matrix<T> transponse()
	{
		vector<T>* vect_t = new vector<T>[this->size];
		T* tmp = new T[1];
		for (uitype i = 0; i < this->size; ++i)
		{
			tmp[0] = this->vect[i];
			vect_t[i] = vector<T>(tmp, 1);
		}
		matrix<T> matr_vect_t(this->size, vect_t);
		delete[] tmp;
		return matr_vect_t;
	}

	std::string to_string()
	{
		if (!this->size && this->vect == nullptr)
		{
			return std::string("()");
		}
		else
		{
			std::string str_vect = "(";
			for (size_t ind = 0; ind < this->size; ++ind)
			{
				str_vect += std::to_string(this->vect[ind]);
				if (ind != this->size - 1)
				{
					str_vect += ", ";
				}
			}
			str_vect += ")";
			return str_vect;
		}
	}

	const std::string to_string() const
	{
		if (!this->size)
		{
			return std::string("()");
		}
		else
		{
			std::string str_vect = "(";
			for (size_t ind = 0; ind < this->size; ++ind)
			{
				str_vect += std::to_string(this->vect[ind]);
				if (ind != this->size - 1)
				{
					str_vect += ", ";
				}
			}
			str_vect += ")";
			return str_vect;
		}
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
		return iterator(vect);
	}

	iterator end()
	{
		return iterator(vect + size);
	}
};
#endif // !VECTOR_H
