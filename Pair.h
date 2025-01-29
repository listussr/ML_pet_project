#pragma once
#ifndef PAIR_H
#define PAIR_H

template<typename T, typename K>
class pair
{
private:
	T first;
	K second;

public:

	pair() {}

	pair(T first, K second)
	{
		this->first = first;
		this->second = second;
	}

	T get_first() const
	{
		return this->first;
	}

	K get_second() const
	{
		return this->second;
	}

	void set_first(const T first)
	{
		this->first = first;
	}

	void set_second(const T second)
	{
		this->second = second;
	}
};
#endif // !PAIR_H
