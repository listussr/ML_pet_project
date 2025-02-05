#pragma once
#ifndef UTILS_H
#define UTILS_H
#include"Array.h"
#include"Pair.h"
#include"Vector.h"
#include"DataLoader.h"
#include<algorithm>

array<ftype> from_string_array(array<std::string> arr)
{
	array<ftype> farr(arr.get_size());
	for (size_t i = 0; i < arr.get_size(); ++i)
	{
		farr[i] = std::stod(arr[i]);
	}
	return farr;
}

array<data_loader> create_batches(data_loader data, size_t batch_size)
{
	size_t size = data.get_size();
	size_t last_batch_size = size % batch_size;
	size_t batch_num = (size - last_batch_size) / batch_size;
	array<data_loader> batches;
	for (itype i = 0; i < batch_num; ++i)
	{
		batches.push_back(data_loader(data(i * batch_num, (i + 1) * batch_num)));
	}
	batches.push_back(data_loader(data(batch_num * batch_size, size)));
	return batches;
}

itype find_max(const vector<ftype>& vec)
{
	itype max = 0;
	for (itype i = 0; i < vec.get_size() - 1; ++i)
	{
		if (vec[i] < vec[i + 1])
		{
			max = i + 1;
		}
	}
	return max;
}

std::string draw_matrix(matrix<ftype> matr)
{
	std::string str;
	str.push_back('/');
	str += std::string(matr.get_cols_num(), '-');
	str.push_back('\\');
	str.push_back('\n');
	for (uitype i = 0; i < matr.get_rows_num(); ++i)
	{
		str.push_back('|');
		for (uitype j = 0; j < matr.get_cols_num(); ++j)
		{
			str.push_back(get_symbol(matr[i][j]));
		}
		str.push_back('|');
		str.push_back('\n');
	}
	str.push_back('\\');
	str += std::string(matr.get_cols_num(), '-');
	str.push_back('/');
	return str;
}

/// <summary>
/// function of generation linear function:
///		f[i] = k * x + b
/// </summary>
/// <param name="train"> - include noise if train</param>
/// <param name="start_val"> - coefficient "b"</param>
/// <param name="size"> - amount of points in function vector</param>
/// <param name="coefficent"> - coefficent "k"</param>
/// <returns></returns> 
vector<ftype> generate_lin_function(bool train, itype start_val, itype size, ftype coefficent)
{
	vector<ftype> fun;
	for (itype i = start_val; i < start_val + size; ++i)
	{
		fun.push_back(
			(ftype)(!train ? i : coefficent * (i + ((ftype)rand() / RAND_MAX * (rand() % 2))))
		);
	}
	return fun;
}

vector<ftype> generate_two_var_lin_function(bool train, vector<ftype> start_val, itype size, ftype coefficent_1, ftype coefficent_2)
{
	vector<ftype> fun;
	for (itype i = start_val[0], j = start_val[0]; i < start_val[0] + size; ++i, ++j)
	{
		fun.push_back(
			coefficent_1 * i + coefficent_2 * j
		);
	}
	return fun;
}

vector<ftype> generate_three_var_lin_function(bool train, vector<ftype> start_val, itype size, ftype coefficent_1, ftype coefficent_2, ftype coefficent_3)
{
	vector<ftype> fun;
	for (itype i = start_val[0], j = start_val[1], k = start_val[2]; i < start_val[0] + size; ++i, ++j, ++k)
	{
		fun.push_back(
			coefficent_1 * i + coefficent_2 * j + coefficent_3 * k
		);
	}
	return fun;
}

matrix<ftype> generate_three_var_lin_function(vector<ftype> start_val, itype size)
{
	matrix<ftype> values(size, 3);
	for (uitype i = 0; i < size; ++i)
	{
		for (uitype j = 0; j < 3; j++)
		{
			values[i][j] = start_val[j] + i;
		}
	}
	return values;
}

matrix<ftype> generate_two_var_lin_function(vector<ftype> start_val, itype size)
{
	matrix<ftype> values(size, 2);
	for (uitype i = 0; i < size; ++i)
	{
		for (uitype j = 0; j < 2; j++)
		{
			values[i][j] = start_val[j] + i;
		}
	}
	return values;
}

#endif // !UTILS_H
