#pragma once
#ifndef MATH_H
#define MATH_H
#include"Matrix.h"

ftype loss_partial_derivative
(
	matrix<ftype> features, vector<ftype> weights, vector<ftype> targets,
	ftype(*loss_f)(matrix<ftype>, vector<ftype>, vector<ftype>), itype ind
)
{
	assert(abs(ind) < targets.get_size());
	vector<ftype> delta_weights = weights;
	delta_weights[ind] += 1e-07;
	return (loss_f(features, delta_weights, targets) - loss_f(features, weights, targets)) / 1.0E-7;
}

ftype loss_partial_derivative
(
	vector<ftype> predictions, vector<ftype> targets,
	ftype(*loss_f)(vector<ftype>, vector<ftype>), itype ind
)
{
	assert(abs(ind) < targets.get_size());
	vector<ftype> delta_predictions = predictions;
	delta_predictions[ind] += 1e-07;
	return (loss_f(delta_predictions, targets) - loss_f(predictions, targets)) / 1.0E-7;
}

vector<ftype> loss_gradient
(
	matrix<ftype> features, vector<ftype> weights, vector<ftype> targets,
	ftype(*loss_f)(matrix<ftype>, vector<ftype>, vector<ftype>)
)
{
	assert(features.get_rows_num() == targets.get_size());
	vector<ftype> grad(weights.get_size());
	for (itype ind = 0; ind < weights.get_size(); ++ind)
	{
		grad[ind] = loss_partial_derivative(features, weights, targets, loss_f, ind);
	}
	return grad;
}

vector<ftype> loss_gradient
(
	vector<ftype> predictions, vector<ftype> targets,
	ftype(*loss_f)(vector<ftype>, vector<ftype>)
)
{
	vector<ftype> grad(predictions.get_size());
	for (itype ind = 0; ind < targets.get_size(); ++ind)
	{
		grad[ind] = loss_partial_derivative(predictions, targets, loss_f, ind);
	}
	return grad;
}

ftype euclidian_distance(vector<ftype> vec1, vector<ftype> vec2)
{
	assert(vec1.get_size() == vec2.get_size());
	ftype euclidian_dist = 0;
	for (uitype ind = 0; ind < vec1.get_size(); ++ind)
	{
		euclidian_dist += pow((vec1[ind] - vec2[ind]), 2);
	}
	return sqrt(euclidian_dist);
}

ftype absolute_distance(vector<ftype> vec1, vector<ftype> vec2)
{
	assert(vec1.get_size() == vec2.get_size());
	ftype absolute_dist = 0;
	for (uitype ind = 0; ind < vec1.get_size(); ++ind)
	{
		absolute_dist += abs(vec1[ind] - vec2[ind]);
	}
	return absolute_dist;
}

void clip_gradient(vector<ftype>& gradient, ftype max_value)
{
	for (ftype& num : gradient)
	{
		num = abs(num) > abs(max_value) ? max_value : num;
	}
}

namespace loss_functions
{
	ftype mse_loss(matrix<ftype> features, vector<ftype> weights, vector<ftype> targets)
	{
		assert(features.get_rows_num() == targets.get_size());
		uitype N = targets.get_size();
		return pow(euclidian_distance(features * weights, targets), 2) / N;
	}

	ftype mae_loss(matrix<ftype> features, vector<ftype> weights, vector<ftype> targets)
	{
		assert(features.get_rows_num() == targets.get_size());
		uitype N = targets.get_size();
		return absolute_distance(features * weights, targets) / N;
	}

	ftype cross_entropy_loss(vector<ftype> input, matrix<ftype> weights, vector<ftype> targets)
	{
		ftype sum = 0;
		vector<ftype> predictions = weights * input;
		assert(input.get_size() == predictions.get_size());
		for (uitype i = 0; i < targets.get_size(); ++i)
		{
			sum += targets[i] * log(predictions[i]) + (1 - targets[i]) * log(1 - predictions[i]);
		}
		return (-1) * sum;
	}

	ftype mse_loss_(vector<ftype> predictions, vector<ftype> targets)
	{
		assert(predictions.get_size() == targets.get_size());
		uitype N = targets.get_size();
		return pow(euclidian_distance(predictions, targets), 2) / N;
	}

	ftype mae_loss_(vector<ftype> predictions, vector<ftype> targets)
	{
		assert(predictions.get_size() == targets.get_size());
		uitype N = targets.get_size();
		return absolute_distance(predictions, targets) / N;
	}

	ftype cross_entropy_loss_(vector<ftype> predictions, vector<ftype> targets)
	{
		ftype sum = 0;
		assert(targets.get_size() == predictions.get_size());
		for (uitype i = 0; i < targets.get_size(); ++i)
		{
			sum += targets[i] * log(predictions[i]) + (1 - targets[i]) * log(1 - predictions[i]);
		}
		return (-1) * sum;
	}
}

template<typename Functional_>
ftype dihotomia_method(Functional_ f, ftype a, ftype b)
{
	if (a > b)
	{
		std::swap(a, b);
	}
	while (abs(b - a) > 2 * epsilon)
	{
		f((b + a) / 2 + epsilon * 0.1) > f((b + a) / 2 - epsilon * 0.1) ? b = (b + a) / 2 : a = (b + a) / 2;
	}
	return (b + a) / 2;
}

//vector<ftype> adaptive_gradient_descent(vector<ftype> weights, ftype(*f)(vector<ftype>))
//{
//	size_t iterations = 0;
//	vector<ftype> weights_step = weights;
//	vector<ftype> grad(weights.get_size());
//	bool end_flag = false;
//	for (; iterations < max_iterations && !end_flag; ++iterations)
//	{
//		grad = gradient(weights, f);
//		grad.normalize();
//		auto fun = [f, weights, grad](ftype alpha) { return f(weights - grad * alpha); };
//		ftype learning_rate = dihotomia_method(fun, 0, 10);			
//		weights_step = weights - grad * learning_rate;
//		if (euclidian_distance(weights, weights_step) < 2 * epsilon)							
//		{
//			end_flag = true;
//		}
//		else
//		{
//			weights = weights_step;
//		}
//	}
//	return (weights + weights_step) / 2;
//}

vector<ftype> adaptive_gradient_descent_
(
	matrix<ftype> features, vector<ftype> weights, vector<ftype> targets,
	ftype(*loss_f)(matrix<ftype>, vector<ftype>, vector<ftype>)
)
{
	size_t iterations = 0;
	vector<ftype> weights_step = weights;
	vector<ftype> grad(weights.get_size());
	bool end_flag = false;
	for (; iterations < max_iterations && !end_flag; ++iterations)
	{
		grad = loss_gradient(features, weights, targets, loss_f);
		grad.normalize();
		auto fun = [loss_f, features, weights, targets, grad](ftype alpha) { return loss_f(features, weights - grad * alpha, targets); };
		ftype learning_rate = dihotomia_method(fun, 0, 100);
		weights_step = weights - grad * learning_rate;
		if (euclidian_distance(weights, weights_step) < 2 * epsilon)
		{
			end_flag = true;
		}
		else
		{
			weights = weights_step;
		}
	}
	return (weights + weights_step) / 2;
}

namespace activation_functions
{

	//vector<ftype> derivative(vector<ftype> input, vector<ftype>(*activation_function)(vector<ftype>))
	//{
	//	vector<ftype> der(input.get_size());
	//	vector<ftype> cur_point = activation_function(input);
	//	if (activation_function != activation_functions::softmax)
	//	{
	//		vector<ftype> delta_input = input + 1e-07;
	//		//std::cout << delta_input.to_string() << '\n';
	//		vector<ftype> delta_point = activation_function(delta_input);
	//		//std::cout << delta_point.to_string() << '\n';
	//		for (uitype i = 0; i < input.get_size(); ++i)
	//		{
	//			der[i] = (delta_point[i] - cur_point[i]) / 1e-07;
	//		}
	//	}
	//	else
	//	{
	//		vector<ftype> delta_input = input;
	//		for (uitype i = 0; i < input.get_size(); ++i)
	//		{
	//			delta_input[i] += 1e-07;
	//			der[i] = (activation_functions::softmax(delta_input)[i] - cur_point[i]) / 1e-07;
	//			delta_input[i] -= 1e-07;
	//		}
	//	}
	//	return der;
	//}

	vector<ftype> derivative(vector<ftype> input, vector<ftype>(*activation_function)(vector<ftype>))
	{
		vector<ftype> der(input.get_size());
		vector<ftype> cur_point = activation_function(input);

		vector<ftype> delta_input = input;
		for (uitype i = 0; i < input.get_size(); ++i)
		{
			delta_input[i] += 1e-07;
			der[i] = (activation_function(delta_input)[i] - cur_point[i]) / 1e-07;
			delta_input[i] -= 1e-07;
		}
		return der;
	}

	vector<ftype> no_activation(vector<ftype> input)
	{
		return input;
	}

	vector<ftype> ReLU(vector<ftype> input)
	{
		for (auto& num : input)
		{
			num = (num > 0) ? num : 0;
		}
		return input;
	}

	vector<ftype> sigmoid(vector<ftype> input)
	{
		for (auto& num : input)
		{
			num = 1 / (1 + pow(e, (-1) * num));
		}
		return input;
	}

	vector<ftype> leaky_ReLU(vector<ftype> input)
	{
		for (auto& num : input)
		{
			num = num = (num > 0) ? num : num * 0.1;
		}
		return input;
	}

	vector<ftype> tanh(vector<ftype> input)
	{
		for (auto& num : input)
		{
			num = 1 / (1 + pow(e, (-2) * num)) - 1;
		}
		return input;
	}

	vector<ftype> ELU(vector<ftype> input, ftype alpha)
	{
		for (auto& num : input)
		{
			num = (num > 0) ? num : alpha * (pow(e, num) - 1);
		}
		return input;
	}

	vector<ftype> softmax(vector<ftype> input)
	{
		ftype sum = 0;
		//std::cout << "Output before softmax: " << input.to_string() << '\n';
		for (auto num : input)
		{
			sum += pow(e, num);
		}
		for (auto& num : input)
		{
			num = pow(e, num) / sum;
		}
		//std::cout << "Output after softmax: " << input.to_string() << '\n';
		return input;
	}
}

#endif // !MATH_H
