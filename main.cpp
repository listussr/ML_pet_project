#include "LinearRegression.h"
#include"Linear.h"
#include"NeuralNetwork.h"
#include"CSVHandler.h"
#include<algorithm>
#include"Optimizer.h"
#include"Utils.h"

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
	//model.backward(labels);

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
		//model.backward(labels);
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
	linear lin1(2, 24, activation_functions::ReLU);
	linear lin2(24, 12, activation_functions::ReLU);
	linear lin3(12, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	return layers;
}

array<linear> create_linear_model_iris_2()
{
	linear lin1(2, 64, activation_functions::ReLU);
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
	linear lin1(2, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	return layers;
}

array<linear> create_linear_model_iris_4()
{
	linear lin1(2, 12, activation_functions::ReLU);
	linear lin2(12, 6, activation_functions::ReLU);
	linear lin3(6, 3, activation_functions::softmax);
	array<linear> layers;
	layers.push_back(lin1);
	layers.push_back(lin2);
	layers.push_back(lin3);
	return layers;
}

void learn_iris_()
{
	uitype num = 0, amount = 149, epochs = 30;
	itype correct = 0;
	std::cout << "File reading" << '\n';
	csv_handler pd("iris.csv");
	system("cls");
	array<array<std::string>> arr = pd.read_file(amount + 1);

	array<array<ftype>> f_arr(arr.get_size());
	for (uitype i = 0; i < arr.get_size(); ++i)
	{
		vector<ftype> labels = get_iris_labels(arr[i][4]);
		arr[i].pop_back();
		f_arr[i] = from_string_array(arr[i]);
		for (auto label : labels) f_arr[i].push_back(label);
	}

	array<array<ftype>> train = f_arr(1, amount + 1);
	array<ftype> a(5);
	for (uitype i = 0; i < train.get_size(); ++i)
	{
		a[0] = train[i][0];
		a[1] = train[i][3];
		a[2] = train[i][4];
		a[3] = train[i][5];
		a[4] = train[i][6];
		train[i] = a;
	}

	array<pair<vector<ftype>, vector<ftype>>> data(train.get_size());
	for (uitype i = 0; i < train.get_size(); ++i)
	{
		data[i] = pair<vector<ftype>, vector<ftype>>(train[i](0, 2), train[i](2, 5));
		vector<ftype> vec = data[i].get_first();
		vec.normalize_by_max();
		data[i].set_first(vec);
	}
	
	data_loader loader(data);
	array<pair<vector<ftype>, vector<ftype>>> l = loader.get_data();

	NeuralNetwork model(create_linear_model_iris_3(), loss_functions::cross_entropy_loss_, 0.005);
	GD gradient_descent;
	model.set_logging_flag(true);
	gradient_descent.set_Net(model);

	for (uitype i = 0; i < epochs; ++i)
	{
		num = 0;
		correct = 0;

		std::random_shuffle(&l[0], &l[l.get_size() - 1]);

		loader = l;
		data_loader train_loader(loader(0, data.get_size() * 0.8));
		data_loader test_loader(loader(data.get_size() * 0.8, data.get_size()));

		array<data_loader> batches = create_batches(train_loader, 18);
		clock_t start = clock(), end;
		for (data_loader batch: batches)
		{
			gradient_descent.backward(batch);
		}
		for (size_t i = 0; i < test_loader.get_size(); ++i)
		{
			vector<ftype> features = test_loader[i].get_first();
			vector<ftype> labels = test_loader[i].get_second();
			vector<ftype> probabilities = model(features);
			int pred = find_max(probabilities);
			int target = find_max(labels);
			correct += (pred == target);
		}
		end = clock();
		ftype time_taken = ftype(end - start) / ftype(CLOCKS_PER_SEC);
		std::cout << "Epoch #" << i << '\n';
		show_pbar(test_loader.get_size(), correct, test_loader.get_size(), time_taken);
	}

	std::cout << "Ended file reading" << '\n';
}

/**
* Г.А.Й 52 271
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

	learn_iris_();
	return 0;
}
