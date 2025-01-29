#pragma once
#ifndef MATRIX_H
#define MATRIX_H
#include"vector.h"
template<typename T>
class matrix
{
private:
	vector<T>* rows = nullptr;
	uitype rows_num = 0;
	uitype cols_num = 0;

	matrix<T> createSubmatrix(uitype excludeRow, uitype excludeCol) const 
	{
		assert(rows_num > 1);
		matrix<T> submatrix(this->rows_num - 1, this->cols_num - 1);
		uitype subRow = 0;
		for (uitype row = 0; row < this->rows_num; ++row) 
		{
			if (row == excludeRow)
			{
				continue;
			}

			uitype subCol = 0;
			for (uitype col = 0; col < this->cols_num; ++col) 
			{
				if (col == excludeCol) 
				{
					continue;
				}
				submatrix(subRow, subCol) = rows[row][col];
				++subCol;
			}
			++subRow;
		}
		return submatrix;
	}

	T determinantHelper() const {
		assert(this->rows_num == this->cols_num);

		if (this->rows_num == 1) 
		{
			return rows[0][0];
		}
		else if (this->rows_num == 2)
		{
			return this->rows[0][0] * this->rows[1][1] - this->rows[0][1] * this->rows[1][0];
		}
		else 
		{
			T determinant = 0;
			for (uitype col = 0; col < cols_num; col++) 
			{
				matrix<T> sub = createSubmatrix(0, col);
				determinant += (col % 2 == 0 ? 1 : -1) * this->rows[0][col] * sub.determinantHelper();
			}
			return determinant;
		}
	}

public:
	matrix() {}

	matrix(itype rows_num_)
	{
		assert(rows_num_ > 0);
		this->rows_num = rows_num_;
		this->rows = new vector<T>[rows_num_];
	}

	matrix(itype rows_num, vector<T>* rows)
	{
		assert(rows_num > 0);
		assert(rows != nullptr);
		this->rows_num = rows_num;
		this->cols_num = rows[0].get_size();
		this->rows = new vector<T>[rows_num];
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			if (i)
			{
				assert(rows[i].get_size() == rows[i - 1].get_size());
			}
			this->rows[i] = rows[i];
		}
	}

	matrix(vector<T> row, bool transponse)
	{
		assert(row.get_size() > 0);
		if (!transponse)
		{
			this->rows_num = 1;
			this->cols_num = row.get_size();
			this->rows = new vector<T>[this->rows_num];
			this->rows[0] = row;
		}
		else
		{
			this->cols_num = 1;
			this->rows_num = row.get_size();
			this->rows = new vector<T>[this->rows_num];
			for (uitype i = 0; i < this->rows_num; ++i)
			{
				this->rows[i] = vector<T>(this->cols_num);
				this->rows[i][0] = row[i];
			}
		}
	}

	matrix(itype rows_num, itype cols_num)
	{
		assert(rows_num > 0);
		assert(cols_num > 0);
		this->cols_num = cols_num;
		this->rows_num = rows_num;
		this->rows = new vector<T>[rows_num];
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			this->rows[i] = vector<T>(cols_num);
		}
	}

	matrix(const array<array<T>>& arr)
	{
		this->rows_num = arr.get_size();
		this->cols_num = arr[0].get_size();
		this->rows = new vector<T>[this->rows_num];
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			this->rows[i] = vector<T>(arr[i]);
		}
	}

	matrix(const matrix<T>& other)
	{
		if (this->rows != nullptr && this->rows_num != 0 && this->cols_num != 0)
		{
			for (uitype i = 0; i < this->rows_num; ++i)
			{
				this->rows[i].~vector();
			}
			delete[] this->rows;
		}
		this->rows_num = other.rows_num;
		this->cols_num = other.cols_num;
		if (other.rows_num == 0 || other.cols_num == 0) 
		{
			this->rows = nullptr;
			return;
		}
		this->rows = new vector<T>[this->rows_num];
		for (uitype i = 0; i < this->rows_num; ++i) 
		{
			this->rows[i] = other.rows[i];
		}
	}

	~matrix() 
	{
		if (this->rows != nullptr)
		{
			delete[] this->rows;
			this->rows = nullptr;
			this->rows_num = 0;
			this->cols_num = 0;
		}
	}

	uitype get_rows_num() const 
	{
		return this->rows_num;
	}

	uitype get_cols_num() const
	{
		return this->cols_num;
	}

	vector<T>& operator[](uitype row)
	{
		assert(row < this->rows_num);
		return this->rows[row];
	}

	const vector<T>& operator[](uitype row) const
	{
		assert(row < this->rows_num);
		return this->rows[row];
	}

	T& operator()(uitype row, uitype col) 
	{
		assert(row < this->rows_num);
		assert(col < this->cols_num);
		return this->rows[row][col];
	}

	const T& operator()(uitype row, uitype col) const 
	{
		assert(row < this->rows_num);
		assert(col < this->cols_num);
		return this->rows[row][col];
	}

	matrix<T> operator+(const matrix<T>& matr_) 
	{
		assert(this->rows_num != 0);
		assert(matr_.rows_num != 0);
		assert(this->rows_num == matr_.rows_num && this->cols_num == matr_.cols_num);
		matrix<T> result(this->rows_num, this->cols_num);
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < matr_.cols_num; ++j)
			{
				result(i, j) = this->rows[i][j] + matr_.rows[i][j];
			}
		}
		return result;
	}

	const matrix<T> operator+(const matrix<T>& matr_) const 
	{
		assert(this->rows_num != 0);
		assert(matr_.rows_num != 0);
		assert(this->rows_num == matr_.rows_num && this->cols_num == matr_.cols_num);
		matrix<T> result(this->rows_num, this->cols_num);
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < matr_.cols_num; ++j)
			{
				result(i, j) = this->rows[i][j] + matr_.rows[i][j];
			}
		}
		return result;
	}

	matrix<T> operator-(const matrix<T>& matr_)
	{
		return this->operator+(matr_ * (-1));
	}

	const matrix<T> operator-(const matrix<T>& matr_) const
	{
		return this->operator+(matr_ * (-1));
	}

	matrix<T> operator*(const T scale)
	{
		assert(this->rows_num != 0);
		matrix<T> result(this->rows_num, this->cols_num);
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < this->cols_num; ++j)
			{
				result(i, j) = this->rows[i][j] * scale;
			}
		}
		return result;
	}

	const matrix<T> operator*(const T scale) const
	{
		assert(this->rows_num != 0);
		matrix<T> result(this->rows_num, this->cols_num);
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < this->cols_num; ++j)
			{
				result(i, j) = this->rows[i][j] * scale;
			}
		}
		return result;
	}

	matrix<T> operator/(const T scale)
	{
		return this->operator*(1 / scale);
	}

	const matrix<T> operator/(const T scale) const
	{
		return this->operator*(1 / scale);
	}

	matrix<T> operator*(const matrix<T>& matr_)
	{
		assert(this->rows_num != 0);
		assert(matr_.rows_num != 0);
		assert(this->cols_num == matr_.rows_num);
		matrix<T> result(this->rows_num, matr_.cols_num);
		for (uitype i = 0; i < rows_num; ++i) 
		{
			for (uitype j = 0; j < matr_.cols_num; j++)
			{
				T sum = 0;
				for (uitype k = 0; k < this->cols_num; k++) 
				{
					sum += rows[i][k] * matr_.rows[k][j];
				}
				result(i, j) = sum;
			}
		}
		return result;
	}

	const matrix<T> operator*(const matrix<T>& matr_) const
	{
		assert(this->rows_num != 0);
		assert(matr_.rows_num != 0);
		assert(this->rows_num == matr_.cols_num);
		matrix<T> result(this->rows_num, this->cols_num);
		for (uitype i = 0; i < rows_num; ++i)
		{
			for (uitype j = 0; j < matr_.cols_num; j++)
			{
				T sum = 0;
				for (uitype k = 0; k < this->cols_num; k++) {
					sum += rows[i][k] * matr_.rows[k][j];
				}
				result(i, j) = sum;
			}
		}
		return result;
	}

	vector<T> operator*(const vector<T>& vec)
	{
		assert(this->cols_num == vec.get_size());
		vector<T> result(this->rows_num);
		for (size_t i = 0; i < this->rows_num; i++)
		{
			T sum = 0;
			for (size_t k = 0; k < this->cols_num; ++k)
			{
				sum += rows[i][k] * vec[k];
			}
			result[i] = sum;
		}
		return result;
	}

	const vector<T> operator*(const vector<T>& vec) const
	{
		assert(this->cols_num == vec.get_size());
		vector<T> result(this->rows_num);
		for (size_t i = 0; i < this->rows_num; i++)
		{
			T sum = 0;
			for (size_t k = 0; k < this->cols_num; ++k)
			{
				sum += rows[i][k] * vec[k];
			}
			result[i] = sum;
		}
		return result;
	}

	matrix<T> transpose() const 
	{
		matrix<T> result(this->cols_num, this->rows_num);
		for (uitype i = 0; i < rows_num; ++i) 
		{
			for (uitype j = 0; j < cols_num; ++j)
			{
				result(j, i) = rows[i][j];
			}
		}
		return result;
	}

	T determinant() const
	{
		return this->determinantHelper();
	}

	void operator=(const matrix<T>& other) 
	{
		if (this->rows != nullptr) 
		{
			delete[] this->rows;
		}
		this->rows_num = other.rows_num;
		this->cols_num = other.cols_num;
		if (rows_num == 0 || cols_num == 0) 
		{
			this->rows = nullptr;
			return;
		}
		this->rows = new vector<T>[this->rows_num];
		for (size_t i = 0; i < this->rows_num; ++i) 
		{
			this->rows[i] = other.rows[i];
		}
	}

	vector<T> to_vector()
	{
		assert(this->cols_num == 1 && this->rows_num >= 1 || this->cols_num >= 1 && this->rows_num == 1);
		if (this->cols_num == 1 && this->rows_num > 1)
		{
			vector<T> vec(this->rows_num);
			for (uitype i = 0; i < this->rows_num; ++i)
			{
				vec[i] = this->rows[i][0];
			}
			return vec;
		}
		return this->rows[0];
	}

	matrix<ftype> from_string()
	{
		matrix<ftype> matr(this->rows_num, this->cols_num);
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < this->cols_num; ++j)
			{
				matr[i][j] = std::stold(this->rows[i][j]);
			}
		}
		return matr;
	}


	std::string to_string() const
	{
		std::string matr_str = "| ";
		for (uitype i = 0; i < this->rows_num; ++i)
		{
			for (uitype j = 0; j < this->cols_num; ++j)
			{
				matr_str += std::to_string(this->rows[i][j]) + " ";
			}
			matr_str += (i != this->rows_num - 1? "|\n| ": "|\n");
		}
		return matr_str;
	}
};
#endif //MATRIX_H