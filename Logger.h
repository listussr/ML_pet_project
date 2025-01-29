#pragma once
#ifndef LOGGER_H
#define LOGGER_H
#include<fstream>
#include"Matrix.h"
class logger
{
private:
	const char* file_name = "test.txt";

	std::ofstream out;

public:
	logger()
	{
		out.open(this->file_name, std::ios::out | std::ios::trunc);
	}

	logger(const char* file_name)
	{
		this->file_name = file_name;
		out.open(this->file_name, std::ios::out | std::ios::trunc);
	}

	~logger()
	{
		if (out.is_open())
		{
			out.close();
		}
	}

	void set_file_name(const char* file_name)
	{
		if (out.is_open())
		{
			out.close();
		}
		this->file_name = file_name;
		out.open(this->file_name, std::ios::out | std::ios::trunc);
	}

	void operator()(const matrix<ftype>& matr)
	{
		out << matr.to_string();
	}

	void operator()(const vector<ftype>& vect)
	{
		out << vect.to_string();
	}

	void operator()(const ftype& num)
	{
		out << num;
	}

	void operator()(const char* str)
	{
		out << str;
	}
};

#endif // !LOGGER_H
