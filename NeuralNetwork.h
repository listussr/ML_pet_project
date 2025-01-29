#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include"Matrix.h"
#include"Linear.h"
#include"Array.h"
#include"Pair.h"
#include"Logger.h"
class NeuralNetwork
{
private:
	array<linear> layers;
	// results of each operation
	array<vector<ftype>> temp_counting;
	// 'delta' param for counting hidden layer weights gradient
	vector<ftype> delta_counting;
	// gradients of weights on the each layer
	array<matrix<ftype>> delta_weights;
	//gradients of biases on the each layer
	array<vector<ftype>> delta_biases;
	// loss function
	ftype(*loss_function)(vector<ftype>, vector<ftype>);
	ftype learning_rate;
	// util for logging into the file
	logger logg;
	bool log_into_file = false;
	std::string loss_function_name = "MSELoss";

	void last_layer_backward(vector<ftype> targets)
	{
		/**
		* partial derivatives for weights
		*  dL	   dL  	 dp_i	dz_i	 dL    df_activation
		* ----- = ---- * ---- *	----- = ---- * ------------- (z_i) * x_j
		* dW_ij   dp_i	 dz_i	dW_ij   dp_i	    dz_i
		* 
		* partial derivative for biases
		*  dL	  dL    dp_i   dz_i	   dL    df_activation
		* ---- = ---- * ---- * ---- = ---- * ------------- (z_i) * 1
		* db_i   dp_i	dz_i   db_i   dp_i	    dz_i
		*/
		if (this->layers.get_size() == 0)
		{
			std::cout << "No layers in network!" << '\n';
			return;
		}

		// dL/dy_i

		//std::cout << "tmp_counting[-1]: " << temp_counting[-1].to_string() << '\n';
		//std::cout << "targets: " << targets.to_string() << '\n';
		//std::cout << "Loss: " << loss_function(temp_counting[-1], targets) << '\n';

		vector<ftype> grad = loss_gradient(temp_counting[-1], targets, loss_function);
	
		if(this->log_into_file)
		{
			logg("Result: ");
			logg(temp_counting[-1]);
			logg("\nTarget:");
			logg(targets);
			logg("\n");
		}

		//std::cout << "Loss gradient: " << grad.to_string() << '\n';

		uitype rows_num = layers[-1].get_weights().get_rows_num();
		uitype cols_num = layers[-1].get_weights().get_cols_num();

		matrix<ftype> delta_weights = layers[-1].get_weights();

		/*if (this->log_into_file)
		{
			logg("Gradient: "); logg(grad); logg("\n");
		}*/

		vector<ftype> activation_function_der = activation_functions::derivative(this->temp_counting[-2], this->layers[-1].get_activation_function());

		//std::cout << "activation_derivative: " << activation_function_der.to_string() << '\n';

		this->delta_counting = grad.elementwise(activation_function_der);

		//std::cout << "Delta counting: " << delta_counting.to_string() << '\n';

		grad = grad * learning_rate;

		vector<ftype> norm_temp_counting = this->temp_counting[-3];
		//norm_temp_counting.normalize();

		for (uitype i = 0; i < rows_num; ++i)
		{
			for (uitype j = 0; j < cols_num; ++j)
			{
				delta_weights[i][j] = grad[i] * norm_temp_counting[j] /*temp_counting[-3][j]*/ * activation_function_der[i];
			}
		}
		this->delta_weights.push_back(delta_weights);
		this->delta_biases.push_back(grad);
	}

	void layers_backward()
	{
		if (this->layers.get_size() <= 1)
		{
			return;
		}

		/*for (auto el : this->temp_counting)
		{
			std::cout << el.get_size() << '\n';
		}
		exit(0);
		*/
		for (uitype n = this->layers.get_size() - 1, k = 2; n > 0; --n, k += 2)
		{
			
			uitype rows_num = this->layers[n - 1].get_weights().get_rows_num();
			uitype cols_num = this->layers[n - 1].get_weights().get_cols_num();

			// counting delta_j^(n)
			vector<ftype> tmp_delta(rows_num);
			//std::cout << "Point to take derivative: " << this->temp_counting[-1 - k].to_string() << '\n';
			vector<ftype> tmp_derivative = activation_functions::derivative(this->temp_counting[-1 - k], this->layers[n - 1].get_activation_function());
			//std::cout << "Layer " << n << " | activation derivative: " << tmp_derivative.to_string() << '\n';
			vector<ftype> delta_bias = tmp_derivative * this->learning_rate;

			for (uitype j = 0; j < tmp_delta.get_size(); ++j)
			{
				ftype sum = 0;
				for (uitype m = 0; m < this->delta_counting.get_size(); ++m)
				{
					sum = this->delta_counting[m] * this->layers[n].get_weights().transpose()[j][m];
				}
				tmp_delta[j] = sum * tmp_derivative[j];
				
			}
			this->delta_counting = tmp_delta;
			//std::cout << "tmp_delta: " << tmp_delta.to_string() << '\n';
			matrix<ftype> delta_weights(this->layers[n - 1].get_weights());

			vector<ftype> norm_temp_counting = this->temp_counting[-3 - k];
			//norm_temp_counting.normalize();

			for (uitype i = 0; i < rows_num; ++i)
			{
				for (uitype j = 0; j < cols_num; ++j)
				{
					delta_weights[i][j] = this->learning_rate * /* learning_rate * */ /*10000*/ this->delta_counting[i] * norm_temp_counting[j]/*this->temp_counting[-3 - k][j]*/;
				}
			}
			this->delta_weights.push_back(delta_weights);
			this->delta_biases.push_back(delta_bias);
		}
		//std::cout << "________________________________\n\n";
	}

	void print_params()
	{
		logg("Learning Params:\n");
		logg("==================================================================\n");
		logg("|>> Learning rate: "); logg(learning_rate); logg("\n");
		logg("|>> Loss function: "); logg(this->loss_function_name.c_str()); logg("\n");
		logg("==================================================================\n");
	}

	void print_architecture()
	{
		logg("==================================================================\n");
		for (uitype i = 0; i < this->layers.get_size(); ++i)
		{
			logg("|>> Layer# "); logg(i);
			logg("\t| In params: "); logg(layers[i].get_weights().get_cols_num());
			logg("\t| Out params: "); logg(layers[i].get_weights().get_rows_num());
			logg("\t| Activation function: "); logg(layers[i].get_activation_function_name().c_str());
			logg("\n");
		}
		logg("==================================================================\n");
	}

	void set_loss_function_name()
	{
		if (loss_function == loss_functions::mse_loss_)
		{
			this->loss_function_name = "MSELoss";
		}
		else if (loss_function == loss_functions::cross_entropy_loss_)
		{
			this->loss_function_name = "CrossEntropyLoss";
		}
	}

public:
	NeuralNetwork() {};

	NeuralNetwork(const array<linear> layers, ftype(*loss_function)(vector<ftype>, vector<ftype>), ftype learning_rate)
	{
		this->layers = layers;
		this->loss_function = loss_function;
		this->learning_rate = learning_rate;
		logg.set_file_name("learning.txt");
		this->temp_counting = array<vector<ftype>>(layers.get_size());
		this->set_loss_function_name();
		this->print_params();
		this->print_architecture();
	}
	
	NeuralNetwork(const NeuralNetwork& nn)
	{
		this->delta_biases = nn.delta_biases;
		this->delta_counting = nn.delta_counting;
		this->delta_weights = nn.delta_weights;
		this->layers = nn.layers;
		this->log_into_file = nn.log_into_file;
		this->logg.set_file_name("learning.txt");
		this->loss_function_name = nn.loss_function_name;
		this->loss_function = nn.loss_function;
	}

	~NeuralNetwork() = default;

	vector<ftype> forward(vector<ftype> features)
	{
		/**
		* 1 .push_back() - x
		* 2 .push_back() - W * x + b
		*/


		this->temp_counting.clear();
		vector<ftype> linear_operation_memory;
		//std::cout << features.to_string() << '\n';
		for (linear layer : layers)
		{
			this->temp_counting.push_back(features);
			//std::cout << features.to_string() << '\n';
			features = layer.forward(features, linear_operation_memory);
			//getchar();
			//features.normalize();

			this->temp_counting.push_back(linear_operation_memory);
		}
		//std::cout << features.to_string() << '\n';
		this->temp_counting.push_back(features);

		return features;
	}

	void set_logging_flag(bool flag)
	{
		this->log_into_file = flag;
	}

	void push_back(linear layer)
	{
		this->layers.push_back(layer);
	}

	array<linear> get_layers() const
	{
		return this->layers;
	}

	vector<ftype> operator()(vector<ftype> features)
	{
		for (linear layer : this->layers)
		{
			features = layer.forward(features);
			//features.normalize();
		}
		return features;
	}

	void backward(vector<ftype> targets)
	{
		if (this->layers.get_size() == 0)
		{
			return;
		}

		this->delta_weights.clear();
		this->delta_biases.clear();
		this->delta_counting.clear();

		this->last_layer_backward(targets);

		this->layers_backward();

		itype size = this->layers.get_size() + 1;

		for (itype i = 1; i < size; ++i)
		{
			matrix<ftype> weights = this->layers[-i].get_weights();
			if (this->log_into_file)
			{
				logg("\n# Layer<"); logg(size - i); logg(">\n");
				logg("Current weights:\n"); logg(weights); logg("\n");
				logg("Correction weights:\n"); logg(this->delta_weights[i - 1]); logg("\n");
			}

			vector<ftype> bias = this->layers[-i].get_bias();
			if (this->log_into_file)
			{
				logg("Current bias: "); logg(bias); logg("\n");
				logg("Correction on bias: "); logg(this->delta_biases[i - 1]); logg("\n\n");
			}
			
			bias = bias - this->delta_biases[i - 1];
			weights = weights - this->delta_weights[i - 1];
			this->layers[-i].update_bias(bias);
			this->layers[-i].update_weights(weights);
		}
		if (this->log_into_file)
		{
			logg("----------------------------------------------------------------------------------------------\n");
		}
	}

	void save_model()
	{
		std::ofstream out("model.txt");
		
	}
};


#endif // !NEURALNETWORK_H
