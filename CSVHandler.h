#pragma once
#ifndef CSVHANDLER_H
#define CSVHANDLER_H
#include"Assets.h"
#include<fstream>
#include"Array.h"
#include<iostream>
#include <sstream>

class csv_handler
{
private:
	const char* file_name;
	std::ifstream in;

public:
	csv_handler() = default;

	csv_handler(const char* file_name)
	{
		this->file_name = file_name;
		in.open(this->file_name);
	}

	~csv_handler()
	{
		if (this->in.is_open())
		{
			this->in.close();
		}
	}

	array<array<std::string>> read_file(uitype amount)
	{
		if (!this->in.is_open())
		{
			std::cout << "File cannot be opened!" << '\n';
			exit(0);
		}
		std::string line, word, temp;
		array<array<std::string>> all_rows;
		array<std::string> row;
		uitype i = 0;
		while (!this->in.eof())
		{
			if (i == amount)
			{
				break;
			}
			row.clear();
			getline(this->in, line);
			std::stringstream strstream(line);
			while (getline(strstream, word, ','))
			{
				row.push_back(word);
			}
			all_rows.push_back(row);
			++i;
		}
		return all_rows;
	}
};
#endif // !CSVHANDLER_H
