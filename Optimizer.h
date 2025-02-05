#pragma once
#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include"NeuralNetwork.h"

class Optimizer
{
protected:
	NeuralNetwork net;

	// 'delta' param for counting hidden layer weights gradient
	vector<ftype> delta_counting;
	// gradients of weights on the each layer
	array<matrix<ftype>> delta_weights;
	ftype learning_rate;

	logger logg;
	bool log_into_file;

	void open_logger()
	{
		this->logg.set_file_name(net.logg.get_file_name());
	}

	void get_log_flag()
	{
		this->log_into_file = this->net.log_into_file;
	}

	void last_layer_gradient(vector<ftype> targets)
	{
		/**
		* partial derivatives for weights
		*  dL	   dL  	 dp_i	dz_i	 dL    df_activation
		* ----- = ---- * ---- *	----- = ---- * ------------- (z_i) * x_j
		* dW_ij   dp_i	 dz_i	dW_ij   dp_i	    dz_i
		*
		*/
		if (net.layers.get_size() == 0)
		{
			std::cout << "No layers in network!" << '\n';
			return;
		}

		//+----------------------------------------------------------------------
		// gradient of loss function for output vector
		//+----------------------------------------------------------------------
		vector<ftype> grad = loss_gradient(net.temp_counting[-1], targets, net.loss_function);
		//std::cout << grad.to_string() << '\n';
		if (this->log_into_file)
		{
			this->logg("Result: ");
			this->logg(net.temp_counting[-1]);
			this->logg("\nTarget:");
			this->logg(targets);
			this->logg("\n");
		}

		uitype rows_num = net.layers[-1].get_weights().get_rows_num();
		uitype cols_num = net.layers[-1].get_weights().get_cols_num();

		//+----------------------------------------------------------------------
		// correction on weights for last layer
		//+----------------------------------------------------------------------
		matrix<ftype> delta_weights = net.layers[-1].get_weights();

		//+----------------------------------------------------------------------
		// vector of derivatives of activation function for each outuput neuron
		//+----------------------------------------------------------------------
		vector<ftype> activation_function_der = activation_functions::derivative(net.temp_counting[-2], net.layers[-1].get_activation_function());
		//std::cout << activation_function_der.to_string() << '\n';
	
		//+----------------------------------------------------------------------
		// delta value that is used for counting next layers weights correction
		//+----------------------------------------------------------------------
		this->delta_counting = grad.elementwise(activation_function_der);
		//std::cout << delta_counting.to_string() << '\n';

		grad = grad * net.learning_rate;
		//std::cout << grad.to_string() << '\n';
		vector<ftype> norm_temp_counting = net.temp_counting[-3];

		for (uitype i = 0; i < rows_num; ++i)
		{
			for (uitype j = 0; j < cols_num; ++j)
			{
				delta_weights[i][j] = grad[i] * norm_temp_counting[j] * activation_function_der[i];
			}
		}
		this->delta_weights.push_back(delta_weights);
	}

	void other_layers_gradients()
	{
		if (net.layers.get_size() <= 1)
		{
			return;
		}

		for (uitype n = net.layers.get_size() - 1, k = 2; n > 0; --n, k += 2)
		{

			uitype rows_num = net.layers[n - 1].get_weights().get_rows_num();
			uitype cols_num = net.layers[n - 1].get_weights().get_cols_num();

			// counting delta_j^(n)
			vector<ftype> tmp_delta(rows_num);

			//+----------------------------------------------------------------------
			// vector of derivatives of activation function for each outuput neuron
			//+----------------------------------------------------------------------
			vector<ftype> tmp_derivative = activation_functions::derivative(net.temp_counting[-2 - k], net.layers[n - 1].get_activation_function());


			vector<ftype> delta_bias = tmp_derivative * net.learning_rate;

			for (uitype j = 0; j < tmp_delta.get_size(); ++j)
			{
				ftype sum = 0;
				for (uitype m = 0; m < this->delta_counting.get_size(); ++m)
				{
					sum = this->delta_counting[m] * net.layers[n].get_weights()[m][j];
				}
				tmp_delta[j] = sum * tmp_derivative[j];

			}
			//std::cout << tmp_delta.to_string() << '\n';
			this->delta_counting = tmp_delta;

			matrix<ftype> delta_weights(net.layers[n - 1].get_weights());

			vector<ftype> norm_temp_counting = net.temp_counting[-3 - k];

			for (uitype i = 0; i < rows_num; ++i)
			{
				for (uitype j = 0; j < cols_num; ++j)
				{
					delta_weights[i][j] = net.learning_rate * this->delta_counting[i] * norm_temp_counting[j];
				}
			}
			this->delta_weights.push_back(delta_weights);
		}
	}

	void update_weights_in_layer(itype ind, matrix<ftype> matr)
	{
		this->net.layers[ind].update_weights(matr);
	}

public:

	Optimizer(){}

	Optimizer(NeuralNetwork net)
	{
		this->net = net;
	}

	~Optimizer() = default;

	void count_batch_gradients(data_loader data)
	{
		array<matrix<ftype>> gradients(this->net.layers.get_size());
		for (size_t i = 0; i < data.get_size(); ++i)
		{
			this->delta_weights.clear();
			this->net.forward(data[i].get_first());
			this->last_layer_gradient(data[i].get_second());
			this->other_layers_gradients();


			if (!i)
			{
				for (size_t j = 0; j < this->delta_weights.get_size(); ++j)
				{
					gradients[j] = this->delta_weights[j];
				}
			}
			else
			{
				for (size_t j = 0; j < this->delta_weights.get_size(); ++j)
				{
					gradients[j] += this->delta_weights[j];
				}
			}
		}
		
		for (size_t j = 0; j < this->delta_weights.get_size(); ++j)
		{
			//std::cout << gradients[j].to_string() << '\n';
			//std::cout << "-----------" << '\n';
			gradients[j] /= (ftype)data.get_size();
		}
		this->delta_weights = gradients;
	}
};

class GD : private Optimizer
{
public:
	void set_Net(NeuralNetwork nn)
	{
		this->net = nn;
		this->open_logger();
		this->get_log_flag();
	}

	void backward(data_loader data)
	{

		this->count_batch_gradients(data);
		itype size = this->net.get_layers().get_size() + 1;

		for (itype i = 1; i < size; ++i)
		{
			matrix<ftype> weights = this->net.get_layers()[-i].get_weights();
			if (this->log_into_file)
			{
				logg("\n# Layer<"); logg(size - i); logg(">\n");
				logg("Current weights:\n"); logg(weights); logg("\n");
				logg("Correction weights:\n"); logg(this->delta_weights[i - 1]); logg("\n");
			}

			weights = weights - this->delta_weights[i - 1];
			this->update_weights_in_layer(-i, weights);
		}
		if (this->log_into_file)
		{
			logg("----------------------------------------------------------------------------------------------\n");
		}
	}
};

#endif // !OPTIMIZER_H
