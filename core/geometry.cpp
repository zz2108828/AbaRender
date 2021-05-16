#include "geometry.h"
#include <cassert>

namespace aba {

Matrix::Matrix(uint32_t r, uint32_t c) :
	matrix_(std::vector<std::vector<float> >(r, std::vector<float>(c, 0.f))),
	rows_(r), cols_(c) {}

Matrix::Matrix(const Vec3f& v) :
matrix_(std::vector<std::vector<float> >(4, std::vector<float>(1, 0.f))),
rows_(4), cols_(1)
{
	matrix_[0][0] = v.x;
	matrix_[1][0] = v.y;
	matrix_[2][0] = v.z;
	matrix_[3][0] = 1;
}

uint32_t Matrix::getRows()
{
	return rows_;
}

uint32_t Matrix::getCols()
{
	return cols_;
}

std::vector<float> Matrix::operator[](const uint32_t i) const
{
	return matrix_[i];
}

Matrix Matrix::operator*(const Matrix& b)
{
	assert(cols_ == b.rows_);
	Matrix result(rows_, b.cols_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < b.cols_; j++) {
			float t = 0;
			for (int k = 0; k < b.rows_; k++) {
				t += matrix_[i][k] * b[k][j];
			}		
			result[i][j] = t;
		}
	}
	return result;
}

Matrix Matrix::operator*(const int b)
{
	Matrix result(rows_, cols_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			result[i][j] = b * matrix_[i][j];
		}
	}
	return result;
}

Matrix Matrix::transpose()
{
	Matrix result(cols_, rows_);
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < cols_; j++) {
			result[j][i] = matrix_[i][j];
		}
	}
	return result;
}

Matrix Matrix::inverse()
{
	return Matrix();
}

Matrix Matrix::identity(int dimensions)
{
	Matrix E(dimensions, dimensions);
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			E[i][j] = (i == j ? 1.0f : 0.0f);
		}
	}
	return E;
}

}// aba

