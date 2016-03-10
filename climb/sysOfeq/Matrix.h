#pragma once
#include <iostream>

class Matrix {
	unsigned int rows, columns;
public:
	double * data;
	
	Matrix();
	explicit Matrix(unsigned int N); // The explicit is *NOT* part of the cirriculum, but we need it here to get the behaviour we want.
	Matrix(unsigned int rows, unsigned int columns, const double fill = 0.0 );
	Matrix(const Matrix & rhs);
	~Matrix();

	Matrix & operator=(const Matrix & rhs);

	void invalidate();
	bool isValid() const;

	unsigned int getRows() const { return rows; }
	unsigned int getColumns() const { return columns; }
	
	double & at(unsigned int row, unsigned int column) {
		return data[row*columns + column];
	}
	
	const double & at(unsigned int row, unsigned int column) const {
		return data[row*columns + column];
	}

	void set(unsigned int row, unsigned int column, double value) {
		at(row,column) = value;
	}

	double get(unsigned int row, unsigned int column) const {
		return at(row,column);
	}

    void CoFactor(Matrix &C);
    void Transpose();
    bool Inverse( Matrix & out );

	Matrix & operator +=(const Matrix & rhs);
	Matrix & operator -=(const Matrix & rhs);

	Matrix operator -(const Matrix & rhs) const;
	Matrix operator +(const Matrix & rhs) const;

	Matrix operator *(const Matrix & rhs) const;
	Matrix & operator *=(const Matrix & rhs);

	// Additional functionality, not part of the exercise.
	Matrix operator-() const;				// Unary -(minus)
	Matrix operator*(double rhs) const;		// Multiply on the righthandside with a double.
	Matrix & operator*=(double rhs);		// Multiply-assign on the righthandside with a double.
};


double Determinant(double *a,int n);
std::ostream & operator<<( std::ostream & out, const Matrix & elem );
std::istream & operator>>( std::istream & in, Matrix & elem );

// Additional functionality, not part of the exercise.
Matrix operator*(double lhs, const Matrix & rhs); // Multiply on the lefthandside with a double.
