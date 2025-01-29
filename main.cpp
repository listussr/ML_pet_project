#include "LinearRegression.h"
#include"Linear.h"
#include"NeuralNetwork.h"
#include"CSVHandler.h"
//#include"MNISTDatasetTest.h"
#include<algorithm>


/// <summary>
/// function of generation linear function:
///		f[i] = k * x + b
/// </summary>
/// <param name="train"> - include noise if train</param>
/// <param name="start_val"> - coefficient "b"</param>
/// <param name="size"> - amount of points in function vector</param>
/// <param name="coefficent"> - coefficent "k"</param>
/// <returns></returns>
 
vector<ftype> generate_lin_function(bool train, itype start_val, itype size, ftype coefficent)
{
	vector<ftype> fun;
	for (itype i = start_val; i < start_val + size; ++i)
	{
		fun.push_back(
			(ftype)(!train ? i : coefficent * (i + ((ftype)rand() / RAND_MAX * (rand() % 2))))
		);
	}
	return fun;
}

vector<ftype> generate_two_var_lin_function(bool train, vector<ftype> start_val, itype size, ftype coefficent_1, ftype coefficent_2)
{
	vector<ftype> fun;
	for (itype i = start_val[0], j = start_val[0]; i < start_val[0] + size; ++i, ++j)
	{
		fun.push_back(
			coefficent_1 * i + coefficent_2 * j
		);
	}
	return fun;
}

vector<ftype> generate_three_var_lin_function(bool train, vector<ftype> start_val, itype size, ftype coefficent_1, ftype coefficent_2, ftype coefficent_3)
{
	vector<ftype> fun;
	for (itype i = start_val[0], j = start_val[1], k = start_val[2]; i < start_val[0] + size; ++i, ++j, ++k)
	{
		fun.push_back(
			coefficent_1 * i + coefficent_2 * j + coefficent_3 * k
		);
	}
	return fun;
}

matrix<ftype> generate_three_var_lin_function(vector<ftype> start_val, itype size)
{
	matrix<ftype> values(size, 3);
	for (uitype i = 0; i < size; ++i)
	{
		for (uitype j = 0; j < 3; j++)
		{
			values[i][j] = start_val[j] + i;
		}
	}
	return values;
}

matrix<ftype> generate_two_var_lin_function(vector<ftype> start_val, itype size)
{
	matrix<ftype> values(size, 2);
	for (uitype i = 0; i < size; ++i)
	{
		for (uitype j = 0; j < 2; j++)
		{
			values[i][j] = start_val[j] + i;
		}
	}
	return values;
}


std::string draw_matrix(matrix<ftype> matr)
{
	std::string str;
	str.push_back('/');
	str += std::string(matr.get_cols_num(), '-');
	str.push_back('\\');
	str.push_back('\n');
	for (uitype i = 0; i < matr.get_rows_num(); ++i)
	{
		str.push_back('|');
		for (uitype j = 0; j < matr.get_cols_num(); ++j)
		{
			str.push_back(get_symbol(matr[i][j]));
		}
		str.push_back('|');
		str.push_back('\n');
	}
	str.push_back('\\');
	str += std::string(matr.get_cols_num(), '-');
	str.push_back('/');
	return str;
}

array<linear> create_linear_model()
{
	linear lin1(28 * 28, 250, activation_functions::ReLU);
	linear lin2(250, 100, activation_functions::ReLU);
	linear lin3(100, 10, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	return layers;
}

array<ftype> from_string(const array<std::string> arr)
{
	array<ftype> farray(arr.get_size());
	for (itype i = 0; i < arr.get_size(); ++i)
	{
		farray[i] = std::stold(arr[i]);
	}
	return farray;
}

vector<ftype> create_label(const std::string label)
{
	int int_label = std::stoi(label);
	vector<ftype> labels(10);
	for (uitype i = 0; i < 10; ++i)
	{
		labels[i] = (int_label == i) ? 1 : 0;
	}
	return labels;
}

void rescale(array<ftype>& arr)
{
	for (auto& el : arr)
	{
		el /= 255;
	}
}

itype find_max(const vector<ftype>& vec)
{
	itype max = 0;
	for (itype i = 0; i < vec.get_size() - 1; ++i)
	{
		if (vec[i] < vec[i + 1])
		{
			max = i + 1;
		}
	}
	return max;
}

void handle_learning_MNIST()
{
	uitype num = 1;
	csv_handler pd("mnist_train.csv");
	array<array<std::string>> arr = pd.read_file(10);
	array<ftype> features = from_string(arr[num](1, 785));
	rescale(features);
	array<array<ftype>> arr_features = features.reshape(28, 28);
	matrix<ftype> features_matrix(arr_features);
	vector<ftype> labels = create_label(arr[num](0, 1)[0]);
	std::cout << "Labels: " << labels.to_string() << '\n';
	std::cout << draw_matrix(features_matrix) << '\n';
	NeuralNetwork model(create_linear_model(), loss_functions::cross_entropy_loss_, 1e-00);
	model.set_logging_flag(true);

	vector<ftype> predictions = model.forward(features);//model.forward(features);

	std::cout << "Predictions: " << predictions.to_string() << '\n';
	model.backward(labels);

	std::cout << "Probabilities after learning: " << model(features).to_string() << '\n';
	std::cout << "Predicted value: " << find_max(model(features)) << '\n';
}

void show_pbar(itype processed, itype correct, itype total, ftype time_taken)
{
	ftype accuracy = (ftype)(correct) / processed * 100;
	std::cout << "Processed: " << std::setw(5) << processed << " from: " << std::setw(5) << total << " | " << "Accuracy: "
		<< std::setprecision(6) << std::setw(7) << accuracy <<"% | Time: " << std::setw(7) << time_taken << " seconds" << '\n';
	std::cout.flush();
}

void learn_MNIST()
{
	uitype num = 1, amount = 1001;
	itype correct = 0;
	csv_handler pd("mnist_train.csv");
	std::cout << "File reading" << '\n';
	array<array<std::string>> arr = pd.read_file(amount);
	std::cout << "Ended file reading" << '\n';
	NeuralNetwork model(create_linear_model(), loss_functions::cross_entropy_loss_, 1e-03);
	//model.set_logging_flag(true);
	clock_t start = clock(), end;
	system("cls");
	for (; num < amount; ++num)
	{
		array<ftype> features = from_string(arr[num](1, 785));
		vector<ftype> labels = create_label(arr[num](0, 1)[0]);

		//matrix<ftype> features_matrix(features.reshape(28, 28));
		
		rescale(features);
		vector<ftype> probabilities = model.forward(features);
		//std::cout << probabilities.to_string() << '\n';
		model.backward(labels);
		//system("cls");

		int pred = find_max(probabilities);
		int target = std::stoi(arr[num](0, 1)[0]);
		correct += (pred == target);
		end = clock();
		ftype time_taken = ftype(end - start) / ftype(CLOCKS_PER_SEC);
		show_pbar(num, correct, amount - 1, time_taken);
		//std::cout << draw_matrix(features_matrix) << '\n';
	}
}

vector<ftype> get_iris_labels(const std::string iris_name)
{
	vector<ftype> vec(3);
	vec[0] = iris_name == "Iris-setosa";
	vec[1] = iris_name == "Iris-virginica";
	vec[2] = iris_name == "Iris-versicolor";
	return vec;
}

array<linear> create_linear_model_iris_1()
{
	linear lin1(2, 24, activation_functions::sigmoid);
	linear lin2(24, 12, activation_functions::sigmoid);
	linear lin3(12, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	return layers;
}

array<linear> create_linear_model_iris_2()
{
	linear lin1(4, 64, activation_functions::ReLU);
	linear lin2(64, 32, activation_functions::ReLU);
	linear lin3(32, 16, activation_functions::ReLU);
	linear lin4(16, 8, activation_functions::ReLU);
	linear lin5(8, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	layers.push_back(lin4);
	layers.push_back(lin5);
	return layers;
}

array<linear> create_linear_model_iris_3()
{
	linear lin1(4, 32, activation_functions::ReLU);
	linear lin2(32, 64, activation_functions::ReLU);
	linear lin3(64, 32, activation_functions::ReLU);
	linear lin4(32, 16, activation_functions::ReLU);
	linear lin5(16, 8, activation_functions::ReLU);
	linear lin6(8, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	layers.push_back(lin4);
	layers.push_back(lin5);
	layers.push_back(lin6);
	return layers;
}

void handle_learning_IRIS()
{
	uitype num = 8;
	csv_handler pd("iris.csv");
	array<array<std::string>> arr = pd.read_file(10);
	vector<ftype> features(from_string(arr[num](0, 4)));
	vector<ftype> labels = get_iris_labels(arr[num](4, 5)[0]);
	std::cout << "Features: " << features.to_string() << '\n';
	std::cout << "Label: " << arr[num](4, 5)[0] << '\n';
	std::cout << "Labels: " << labels.to_string() << '\n';

	NeuralNetwork model(create_linear_model_iris_1(), loss_functions::cross_entropy_loss_, 3e-01);
	//model.set_logging_flag(true);

	vector<ftype> predictions = model.forward(features);

	std::cout << "Predictions: " << predictions.to_string() << '\n';
	model.backward(labels);

	std::cout << "Probabilities after learning: " << model(features).to_string() << '\n';
	std::cout << "Predicted value: " << find_max(model(features)) << '\n';
}

void learn_iris()
{
	uitype num = 0, amount = 149, epochs = 1;
	itype correct = 0;
	csv_handler pd("iris.csv");
	std::cout << "File reading" << '\n';
	array<array<std::string>> arr = pd.read_file(amount + 1);
	array<array<std::string>> train = arr(1, amount + 1);
	array<std::string> a(3);
	for (uitype i = 0; i < train.get_size(); ++i)
	{
		a[0] = train[i][0];
		a[1] = train[i][3];
		a[2] = train[i][4];
		train[i] = a;
	}
	//array<array<std::string>> test = arr(135, 150);
	std::cout << "Ended file reading" << '\n';
	system("cls");
	NeuralNetwork model(create_linear_model_iris_1(), loss_functions::cross_entropy_loss_, 1e-01);
	model.set_logging_flag(true);
	for (uitype i = 0; i < epochs; ++i)
	{
		num = 0;
		correct = 0;
		std::random_shuffle(&train[0], &train[amount]);
		
		clock_t start = clock(), end;
		for (; num < train.get_size(); ++num)
		{
			vector<ftype> features(from_string(train[num](0, 2)));
			std::string label = train[num](2, 3)[0];
			vector<ftype> labels = get_iris_labels(label/*train[num](2, 3)[0]*/);
			vector<ftype> probabilities = model.forward(features);
			model.backward(labels);
			//int pred = find_max(probabilities);
			//std::cout << probabilities.to_string() << '\n';
			//std::cout << labels.to_string() << '\n';
			//int target = find_max(labels);
			//correct += (pred == target);
		}

		std::random_shuffle(&train[0], &train[amount]);
		for (uitype i = 0; i < train.get_size(); ++i)
		{
			vector<ftype> features(from_string(train[i](0, 2)));
			vector<ftype> labels = get_iris_labels(train[i](2, 3)[0]);
			vector<ftype> probabilities = model(features);
			int pred = find_max(probabilities);
			int target = find_max(labels);
			correct += (pred == target);
		}
		end = clock();
		ftype time_taken = ftype(end - start) / ftype(CLOCKS_PER_SEC);
		std::cout << "Epoch #" << i << '\n';
		show_pbar(train.get_size(), correct, train.get_size(), time_taken);
		//std::cout << model.get_layers()[-1].get_weights().to_string() << '\n';
	}
}

/**
* Ã.À.É 52 271
*/

int main()
{
	/*
	ftype* arr = new ftype[1];
	arr[0] = -10;

	vector<ftype> train = generate_lin_function(0, 0, 1000, 2.0);
	matrix<ftype> train_features(train.transponse());
	vector<ftype> train_targets = generate_lin_function(1, 0, 1000, 2.0);

	data_loader loader(train_features, train_targets, 0.8);
	LinearRegression lin_regr(loader, false);
	lin_regr.fit(5);

	vector<ftype> test_val(arr, 1);
	ftype prediction = lin_regr.predict(test_val);
	std::cout << "Prediction of f(x) = 2 * x in point : a" << test_val.to_string() << "  = " << prediction << '\n';
	*/
	
	//uitype set_size = 100;

	//vector<ftype> vec1_features = generate_lin_function(1, 1, set_size, 1);
	//vector<ftype> vec1_targets = generate_lin_function(0, 1, set_size, 1);
	//linear lin2(2, 1);
	//
	//layers.push_back(lin2);
	//*/
	//linear lin1(1, 1);
	//array<linear> layers;
	//layers.push_back(lin1);
	//NeuralNetwork nn(layers, loss_functions::mse_loss_, 1e-4);
	////std::cout << vec1_features.to_string() << '\n';
	//vector<ftype> feature(1);
	//vector<ftype> target(1);
	//for (uitype epoch = 0; epoch < 1; ++epoch)
	//{
	//	for (uitype i = 0; i < set_size; ++i)
	//	{
	//		feature[0] = vec1_features[i];
	//		target[0] = vec1_targets[i];
	//		nn.forward(feature);
	//		nn.backward(target);
	//	}
	//	feature[0] = -10;
	//	target[0] = -10;
	//	std::cout << "Epoch# " << epoch << " | Loss: " << loss_functions::mse_loss_(nn(feature), target) << '\n';
	//}
	//feature[0] = -10;
	//std::cout << nn(feature).to_string() << '\n';

	/*uitype set_size = 100;

	vector<ftype> vec(2);
	vec[0] = vec[1] = 1;
	matrix<ftype> features = generate_two_var_lin_function(vec, set_size);
	//std::cout << features.to_string() << '\n';
	vector<ftype> targets = generate_two_var_lin_function(0, vec, set_size, 2, 1);
	//std::cout << targets.to_string() << '\n';
	linear lin1(2, 1);
	
	array<linear> layers;
	layers.push_back(lin1);
	NeuralNetwork nn(layers, loss_functions::mse_loss_, 1e-4);

	nn.set_logging_flag(true);

	vector<ftype> feature(2);
	vector<ftype> target(1);
	for (uitype epoch = 0; epoch < 1; ++epoch)
	{
		for (uitype i = 0; i < set_size; ++i)
		{
			//std::cout << i << '\n';
			feature = features[i];
			target[0] = targets[i];
			nn.forward(feature);
			nn.backward(target);
		}
		feature[0] = -10;
		feature[1] = -10;
		target[0] = -30;
		std::cout << nn(feature).to_string() << '\n';
		std::cout << "Epoch# " << epoch << " | Loss: " << loss_functions::mse_loss_(nn(feature), target) << '\n';
	}*/
	
	/*uitype set_size = 1000;

	vector<ftype> vec(3);
	vec[0] = vec[1] = vec[2] = 1;
	matrix<ftype> features = generate_three_var_lin_function(vec, set_size);
	//std::cout << features.to_string() << '\n';
	vector<ftype> targets = generate_three_var_lin_function(0, vec, set_size, 2, 1, 2);
	//std::cout << targets.to_string() << '\n';
	linear lin1(3, 2);
	linear lin2(2, 1);

	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	NeuralNetwork nn(layers, loss_functions::mse_loss_, 0.5e-5);

	nn.set_logging_flag(true);

	vector<ftype> feature(3);
	vector<ftype> target(1);
	for (uitype epoch = 0; epoch < 1; ++epoch)
	{
		for (uitype i = 0; i < set_size; ++i)
		{
			//std::cout << i << '\n';
			feature = features[i];
			target[0] = targets[i];
			nn.forward(feature);
			nn.backward(target);
		}
		feature[0] = -10;
		feature[1] = -10;
		feature[2] = -10;
		target[0] = -50;
		std::cout << nn(feature).to_string() << '\n';
		std::cout << "Epoch# " << epoch << " | Loss: " << loss_functions::mse_loss_(nn(feature), target) << '\n';
	}*/

	/*uitype num = 53;
	csv_handler pd("mnist_train.csv");
	array<array<std::string>> arr = pd.read_file(100);
	array<std::string> a = arr[num](1, 785);
	std::cout << a.get_size() << '\n';
	array<array<std::string>> ar = a.reshape(28, 28);
	matrix<std::string> matr(ar);
	//std::cout << matr[0][0] << '\n';
	array<std::string> label = arr[num](0, 1);
	matrix<ftype> fmatr = matr.from_string();
	fmatr = fmatr / 255;
	std::cout << "Label: " << label[0] << '\n';
	std::cout << draw_matrix(fmatr) << '\n';
	*/
	
	//learn_MNIST();

	//handle_learning_MNIST();

	//handle_learning_IRIS();

	learn_iris();

	/*vector<ftype> vec(3);
	vec[0] = 10;
	vec[1] = 10;
	vec[2] = 0;

	vector<ftype> der = activation_functions::derivative(vec, activation_functions::sigmoid);

	std::cout << "vector: " << vec.to_string() << '\n';
	std::cout << "ReLU: " << activation_functions::sigmoid(vec).to_string() << '\n';
	std::cout << "derivative: " << der.to_string() << '\n';
	*/

	/*vector<ftype> vec(3);
	vec[0] = 10;
	vec[1] = 10;
	vec[2] = 0;

	vector<ftype> der = activation_functions::derivative(vec, activation_functions::softmax);

	std::cout << "vector: " << vec.to_string() << '\n';
	std::cout << "softmax: " << activation_functions::softmax(vec).to_string() << '\n';
	std::cout << "derivative: " << der.to_string() << '\n';
	*/

	/*vector<ftype> vec(5);
	vec[0] = 1;
	vec[1] = 1;
	vec[2] = 0;
	vec[3] = 1;
	vec[4] = 1;
	std::cout << activation_functions::softmax(vec).to_string() << '\n';*/
	return 0;
}