#pragma once
#ifndef LINEAR_H
#define LINEAR_H
#include"Math.h"
#include"Matrix.h"
class linear
{
private:
	matrix<ftype> weights;
	uitype input_size;
	uitype output_size;
	vector<ftype> bias;
	vector<ftype> (*activation_function)(vector<ftype>);
	std::string activation_function_name = "No activation";

	void set_activation_function_name()
	{
		if (activation_function == activation_functions::ReLU)
		{
			this->activation_function_name = "ReLU";
		}
		else if (activation_function == activation_functions::leaky_ReLU)
		{
			this->activation_function_name = "LeakyReLU";
		}
		else if (activation_function == activation_functions::sigmoid)
		{
			this->activation_function_name = "sigmoid";
		}
		else if (activation_function == activation_functions::softmax)
		{
			this->activation_function_name = "softmax";
		}
		else if (activation_function == activation_functions::tanh)
		{
			this->activation_function_name = "tanh";
		}
	}

	void normalize(vector<ftype>& output)
	{
		ftype max = output[0];
		for (uitype i = 1; i < output.get_size() - 1; ++i)
		{

		}
	}

public:
	linear()
	{
		this->input_size = 0;
		this->output_size = 0;
		this->activation_function = activation_functions::no_activation;
	}

	linear(itype input_size, itype output_size)
	{
		this->input_size = input_size;
		this->output_size = output_size;
		init_weights();
		init_bias();
		this->activation_function = activation_functions::no_activation;
	}

	linear(itype input_size, itype output_size, vector<ftype> (*activation_function)(vector<ftype>))
	{
		this->input_size = input_size;
		this->output_size = output_size;
		init_weights();
		init_bias();
		this->activation_function = activation_function;
		this->set_activation_function_name();
	}

	matrix<ftype> get_weights() const
	{
		return this->weights;
	}

	vector<ftype> get_bias() const
	{
		return this->bias;
	}

	void init_bias()
	{
		srand(time(NULL));
		this->bias = vector<ftype>(this->output_size);
		for (uitype i = 0; i < this->output_size; ++i)
		{
			this->bias[i] = (ftype)rand() / RAND_MAX;
		}
	}

	std::string get_activation_function_name() const
	{
		return this->activation_function_name;
	}

	void init_weights()
	{
		srand(time(NULL));
		this->weights = matrix<ftype>(this->output_size, this->input_size);
		for (uitype i = 0; i < this->output_size; ++i)
		{
			for (uitype j = 0; j < this->input_size; ++j)
			{
				this->weights(i, j) = (ftype)rand() / RAND_MAX;
			}
		}
	}

	vector<ftype> forward(vector<ftype> features, vector<ftype>& counting_memory) const
	{
		//std::cout << this->input_size << '\n';
		//std::cout << features.get_size() << '\n';
		assert(features.get_size() == this->input_size);
		//features.normalize();
		vector<ftype> output = this->weights * features + this->bias;
		counting_memory = output;
		//std::cout << "Before activation: " << output.to_string() << '\n';
		output = this->activation_function(output);
		//std::cout << "After activation: " << output.to_string() << '\n';
		return output;
	}

	vector<ftype> forward(vector<ftype> features) const
	{
		//std::cout << this->input_size << '\n';
		//std::cout << features.get_size() << '\n';
		assert(features.get_size() == this->input_size);
		features.normalize();
		vector<ftype> output = this->weights * features + this->bias;
		output = this->activation_function(output);
		return output;
	}

	vector<ftype>(*get_activation_function() const)(vector<ftype>)
	{
		return this->activation_function;
	}

	void update_weights(matrix<ftype> new_weights)
	{
		this->weights = new_weights;
	}

	void update_bias(vector<ftype> new_bias)
	{
		this->bias = new_bias;
	}

	vector<ftype> backward(vector<ftype>& input, vector<ftype>& output, ftype learning_rate)
	{

		matrix<ftype> d_output_matrix(output, true);
		matrix<ftype> input_matrix(input, true);

		matrix<ftype> d_weights = d_output_matrix * input_matrix.transpose();
		matrix<ftype> d_bias = d_output_matrix;

		this->weights = this->weights - (d_weights * learning_rate);
		this->bias = this->bias - (d_bias.to_vector() * learning_rate);

		matrix<ftype> d_input = this->weights.transpose() * d_output_matrix;

		vector<ftype> result = d_input.to_vector();

		return result;
	}
};
#endif // !LAYER_H
