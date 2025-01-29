#pragma once
#ifndef DATALOADER_H
#define DATALOADER_H
#include"Matrix.h"
class data_loader
{
private:
	matrix<ftype> train_features;
	vector<ftype> train_targets;
	matrix<ftype> test_features;
	vector<ftype> test_targets;

public:
	data_loader() {};

	// train_test_split() from sklearn in python
	data_loader(matrix<ftype> features, vector<ftype> targets, ftype split_criterion)
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

	data_loader(matrix<ftype> train_features, vector<ftype> train_targets, matrix<ftype> test_features, vector<ftype> test_targets)
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

#endif // !DATALOADER_H
