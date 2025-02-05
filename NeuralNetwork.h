#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include"Matrix.h"
#include"Linear.h"
#include"Array.h"
#include"Pair.h"
#include"Logger.h"

class Optimizer;

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
		this->learning_rate = nn.learning_rate;
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
		for (linear layer : layers)
		{
			this->temp_counting.push_back(features);
			features = layer.forward(features, linear_operation_memory);
			this->temp_counting.push_back(linear_operation_memory);
		}
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

	void save_model()
	{
		std::ofstream out("model.txt");
		
	}

	void operator=(const NeuralNetwork& nn)
	{
		this->learning_rate = nn.learning_rate;
		this->delta_biases = nn.delta_biases;
		this->delta_counting = nn.delta_counting;
		this->delta_weights = nn.delta_weights;
		this->layers = nn.layers;
		this->log_into_file = nn.log_into_file;
		this->logg.set_file_name("learning.txt");
		this->loss_function_name = nn.loss_function_name;
		this->loss_function = nn.loss_function;
	}

	friend class Optimizer;
};


#endif // !NEURALNETWORK_H
