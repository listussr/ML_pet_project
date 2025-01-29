#pragma once
#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H
#include"Assets.h"
#include"vector.h"
#include"Math.h"
#include"DataLoader.h"
/// <summary>
/// Поля класса:
/// 1) matrix<ftype> features  - матрица признаков
/// 2) vector<ftype> targets   - вектор таргетов
/// 3) vector<ftype> weights   - вектор весов регрессии
/// 
/// * Количество элементов массива featurеs = размерности targets
/// * Количество элементов в weights = features[0].size + 1, т.к. последний элемент в weights - смещение
/// 
/// </summary>
class LinearRegression
{
private:
	data_loader data;
	vector<ftype> weights;

	ftype(*loss_function)(matrix<ftype>, vector<ftype>, vector<ftype>) = loss_functions::mse_loss;

	bool bias = 0;

	void step(matrix<ftype> features, vector<ftype> targets)
	{
		weights = adaptive_gradient_descent_(features, this->weights, targets, loss_function);
	}

	void init_weights(const size_t num_of_features)
	{
		if (!this->weights.get_size())
		{
			this->weights = vector<ftype>(num_of_features);
		}
		for (size_t ind = 0; ind < this->weights.get_size(); ++ind)
		{
			this->weights.set_val_by_ind(ind, (ftype)rand() / RAND_MAX);
		}
	}

public:
	LinearRegression(){}

	~LinearRegression(){}

	LinearRegression(data_loader data, bool bias)
	{
		srand(time(NULL));
		
		this->bias = bias;
		this->data = data;
		this->init_weights(data.get_test_features().get_cols_num() + this->bias);
	}

	ftype predict(vector<ftype> features)
	{
		ftype sum = 0;
		for (size_t ind = 0; ind < features.get_size(); ++ind)
		{
			sum += features[ind] * this->weights[ind];
		}
		if (bias)
		{
			sum += weights[-1];
		}
		return sum;
	}

	vector<ftype> get_weights()
	{
		return this->weights;
	}

	void fit(itype epochs)
	{
		assert(epochs > 0);
		clock_t start, end;
		for (uitype i = 0; i < epochs; ++i)
		{
			start = clock();
			step(this->data.get_train_features(), this->data.get_train_targets());
			end = clock();
			ftype time_taken = ftype(end - start) / ftype(CLOCKS_PER_SEC);
			std::cout << "Epoch #" << std::setw(4) << i << " | Loss = " << std::setw(10)
				<< loss_function(this->data.get_test_features(), this->weights, this->data.get_test_targets())
				<< " | " << std::setw(10) << time_taken << " seconds" << '\n';
		}
	}
};

#endif // !LINEARREGRESSION_H
