#pragma once
#ifndef DATALOADER_H
#define DATALOADER_H
#include"Matrix.h"
#include"Pair.h"

class data_loader_train_test
{
private:
	matrix<ftype> train_features;
	vector<ftype> train_targets;
	matrix<ftype> test_features;
	vector<ftype> test_targets;

public:
	data_loader_train_test() {};

	// train_test_split() from sklearn in python
	data_loader_train_test(matrix<ftype> features, vector<ftype> targets, ftype split_criterion)
	{
		assert(split_criterion > 0 && split_criterion < 1);
		itype train_size = (itype)(features.get_rows_num() * split_criterion);
		itype test_size = features.get_rows_num() - train_size;

		this->train_features = matrix<ftype>(train_size, features.get_cols_num());
		this->train_targets = vector<ftype>(train_size);

		this->test_features = matrix<ftype>(test_size, features.get_cols_num());
		this->test_targets = vector<ftype>(test_size);

		for (uitype i = 0; i < train_size; ++i)
		{
			this->train_features[i] = features[i];
			this->train_targets[i] = targets[i];
		}

		for (uitype i = 0; i < test_size; ++i)
		{
			this->test_features[i] = features[i + train_size];
			this->test_targets[i] = targets[i + train_size];
		}
	}

	data_loader_train_test(matrix<ftype> train_features, vector<ftype> train_targets, matrix<ftype> test_features, vector<ftype> test_targets)
	{
		this->train_features = train_features;
		this->train_targets = train_targets;

		this->test_features = test_features;
		this->test_targets = test_targets;
	}

	matrix<ftype> get_train_features() const
	{
		return this->train_features;
	}

	vector<ftype> get_train_targets() const
	{
		return this->train_targets;
	}

	matrix<ftype> get_test_features() const
	{
		return this->test_features;
	}

	vector<ftype> get_test_targets() const
	{
		return this->test_targets;
	}
};

class data_loader
{
private:
	array<pair<vector<ftype>, vector<ftype>>> data;

public:
	data_loader() {};

	data_loader(const array<vector<ftype>> features, const array<vector<ftype>> targets)
	{
		for (size_t i = 0; i < features.get_size(); ++i)
		{
			this->data.push_back(pair<vector<ftype>, vector<ftype>>(features[i], targets[i]));
		}
	}

	data_loader(const array<pair<vector<ftype>, vector<ftype>>> data)
	{
		this->data = data;
	}

	data_loader(const data_loader& copy)
	{
		this->data = copy.data;
	}

	array<pair<vector<ftype>, vector<ftype>>> get_data()
	{
		return this->data;
	}

	pair<vector<ftype>, vector<ftype>>& operator[](itype ind)
	{
		return this->data[ind];
	}

	const pair<vector<ftype>, vector<ftype>>& operator[](itype ind) const
	{
		return this->data[ind];
	}

	array<pair<vector<ftype>, vector<ftype>>> operator()(itype first_ind, itype second_ind) const
	{
		array<pair<vector<ftype>, vector<ftype>>> arr;
		for (itype i = first_ind; i < second_ind; ++i)
		{
			arr.push_back(data[i]);
		}
		return arr;
	}

	void operator=(array<pair<vector<ftype>, vector<ftype>>> arr)
	{
		this->data = arr;
	}

	size_t get_size()
	{
		return this->data.get_size();
	}
};

#endif // !DATALOADER_H
