#pragma once
#ifndef ASSETS_H
#define ASSETS_H
#include<cassert>
#include<string>
#include<cmath>
#include<ctime>
#include<iostream>
#include<iomanip>

#define ftype double

#define uitype size_t

#define itype long long

const ftype epsilon = 1e-05;

const uitype vector_capacity_unit = 20;

const uitype max_iterations = 2000;

const ftype e = 2.7182818;

template<typename T>
void copy(T*& dest, const T* source, const uitype size_, const uitype capacity)
{
	if (source == nullptr || size_ == 0) 
	{
		return;
	}
	// allocate memory in size of capacity
	dest = new T[capacity];
	uitype counter = 0;
	// copy elements from source to dest
	for (; counter < size_; ++counter)
	{
		dest[counter] = source[counter];
	}
}

char get_symbol(const ftype& num)
{
	if (abs(1 - num) < abs(num - 0.5))
	{
		return '*';
	}
	else if (abs(num) > abs(num - 0.5))
	{
		return '.';
	}
	else
	{
		return ' ';
	}
}

#endif // !ASSETS_H
