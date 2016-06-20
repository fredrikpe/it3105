#include "Matrix.h"
#include <iostream>
#include <math.h>
/*
   Recursive definition of determinate using expansion by minors.
*/
double Determinant(double *a,int n)
{
   int i,j,j1,j2;
   double det = 0;
   double *m = NULL;

   if (n < 1) { /* Error */

   } else if (n == 1) { /* Shouldn't get used */
      det = a[0];
   } else if (n == 2) {
      det = a[0] * a[3] - a[2] * a[1];
   } else {
      det = 0;
      for (j1=0;j1<n;j1++) {
         m = (double*)malloc((n-1)*(n-1)*sizeof(double));
         for (i=0;i<n-1;i++)
         for (i=1;i<n;i++) {
            j2 = 0;
            for (j=0;j<n;j++) {
               if (j == j1)
                  continue;
               m[(i-1)*(n-1) + j2] = a[i*n + j];
               j2++;
            }
         }
         det += pow(-1.0,j1+2.0) * a[0*n + j1] * Determinant(m,n-1);
         free(m);
      }
   }
   return(det);
}

/*
   Find the cofactor matrix of a square matrix
*/
void Matrix::CoFactor(Matrix &C)
{
   int i,j,ii,jj,i1,j1;
   double det;
   double *c;

   c = (double*)malloc((rows-1)*(rows-1)*sizeof(double));

   for (j=0;j<rows;j++) {
      for (i=0;i<rows;i++) {

         /* Form the adjoint a_ij */
         i1 = 0;
         for (ii=0;ii<rows;ii++) {
            if (ii == i)
               continue;
            j1 = 0;
            for (jj=0;jj<rows;jj++) {
               if (jj == j)
                  continue;
               c[i1*(rows-1) + j1] = data[ii*rows + jj];
               j1++;
            }
            i1++;
         }

         /* Calculate the determinate */
         det = Determinant(c,rows-1);

         /* Fill in the elements of the cofactor */
         C.data[i*rows + j] = pow(-1.0,i+j+2.0) * det;
      }
   }
   free(c);
}

/*
   Transpose of a square matrix, do it in place
*/
void Matrix::Transpose()
{
   int i,j;
   double tmp;
   for (i=1;i<rows;i++) {
      for (j=0;j<columns;j++) {
         tmp = data[i*columns + j];
         data[i*columns + j] = data[j*columns + i];
         data[j*columns + i] = tmp;
      }
   }
}

// We have chosen the second alternative to handle matrices without inverses.
bool Matrix::Inverse( Matrix & out ) 
{
    double det = Determinant(data, rows);

    // If singular return false,
    if (abs(det) < 1.0e-16 ) {
        std::cout << "Det = " << det << std::endl;
        return false;
    }

    Matrix C(rows);
    CoFactor(C);
    C.Transpose();


    for (int i=0; i<rows; i++) {
        for (int j=0; j<rows; j++) {
            double tmp = C.get(i, j);
            out.data[i*columns + j] = tmp / det;
        }
    }
    return true;
}

Matrix::Matrix() : rows(0), columns(0), data(nullptr) {
	// Ingenting her
}

Matrix::Matrix(unsigned int N) : Matrix( N, N, 0.0 )
{
	for ( unsigned int i = 0; i < N; ++i )
		set(i, i, 1.0); // Set equal to identity
}

Matrix::Matrix(unsigned int rows, unsigned int columns, const double fill)
	: rows(rows)
	, columns(columns)
	, data(nullptr)
{
	data = new double[rows*columns];
	for ( unsigned int i = 0; i < rows*columns; ++i)
		data[i] = fill;
}

Matrix::Matrix(const Matrix & rhs) 
	: rows(rhs.rows)
	, columns(rhs.columns)
	, data(0)
{
	// If rhs was not valid, we know that rhs.row and columns == 0, therefore we have an invalid matrix and can stop.
	if (!rhs.isValid())
		return;
	
	data = new double[rows*columns];
	for ( unsigned int i = 0; i < rows*columns; ++i)
		data[i] = rhs.data[i];
}

Matrix::~Matrix() {
	invalidate();
}

void Matrix::invalidate() {
	delete [] data;
	data = nullptr;
	rows = 0;
	columns = 0;

}

bool Matrix::isValid() const {
	return !!data; // Forcing pointer to bool
}

Matrix & Matrix::operator=(const Matrix & rhs)
{
	if (!rhs.isValid())
	{
		this->invalidate();
	}
	else {
		// If we already have the correct number of elements, don't reallocate.
		if (this->rows*this->columns != rhs.rows*rhs.columns)
		{
			// Exception safe allocation
			double * oldData = this->data;

			// Allocate using rhs' dimensions.
			this->data = new double[rhs.rows*rhs.columns];
			
			// Free old data.
			delete[] oldData;
		}

		// But still make sure the dimensions are set correctly.
		this->rows = rhs.rows;
		this->columns = rhs.columns;
		
		for ( unsigned int i = 0; i < rows*columns; ++i)
			this->data[i] = rhs.data[i];
	}
	return *this;
}



Matrix & Matrix::operator +=(const Matrix & rhs) {
	if ( this->rows == rhs.rows && this->columns == rhs.columns )
	{
		for ( unsigned int row = 0; row < rows; ++row )
			for ( unsigned int column = 0; column < columns; ++column )
				this->at(row,column) += rhs.at(row,column);
	}
	else
		invalidate();

	return *this;
}
	
Matrix Matrix::operator+(const Matrix & rhs) const {
    return Matrix(*this) += rhs;
}

std::istream & operator>>( std::istream & in, Matrix & elem )
{
    for ( unsigned int row = 0; row < elem.getRows(); ++row )
    {
        for ( unsigned int column = 0; column < elem.getColumns(); ++column ) {
            int tmp;
            in >> tmp;
            elem.set(row, column, tmp);
        }
    }
	return in;
}

std::ostream & operator<<( std::ostream & out, const Matrix & elem )
{
	if (!elem.isValid())
		out << "The matrix is not valid.";
	else
	{
		for ( unsigned int row = 0; row < elem.getRows(); ++row )
		{
			for ( unsigned int column = 0; column < elem.getColumns(); ++column )
				out << elem.get(row, column) << ' ';
            out << std::endl;
		}
	}
	return out;
}

// The code below contains additional functionality, not part of the exercise.

Matrix & Matrix::operator-=(const Matrix & rhs) {
	if ( this->rows == rhs.rows && this->columns == rhs.columns )
	{
		for ( unsigned int row = 0; row < rows; ++row )
			for ( unsigned int column = 0; column < columns; ++column )
				this->at(row,column) -= rhs.at(row,column);
	}
	else
		invalidate();

	return *this;
}

Matrix Matrix::operator-(const Matrix & rhs) const {
	if ( this->rows == rhs.rows && this->columns == rhs.columns )
		return Matrix(*this) -= rhs;
	else
		return Matrix();
}

Matrix Matrix::operator *(const Matrix & rhs) const {
	if (this->isValid() && this->columns == rhs.rows  )
	{
		Matrix temp(this->rows, rhs.columns);

		for ( unsigned int row = 0; row < this->rows; ++row )
			for ( unsigned int column = 0; column < rhs.columns; ++column )
			{
				temp.at(row,column) = 0.0;
				for ( unsigned int i = 0; i < this->columns; ++i )
					temp.at(row,column) += this->at(row,i) * rhs.at(i,column);
			}
		return temp;
	}
	else
		return Matrix();
}

Matrix & Matrix::operator *=(const Matrix & rhs) {
	return *this = *this * rhs;
}

Matrix Matrix::operator*(double rhs) const {
	return Matrix(*this) *= rhs;
}

Matrix operator*(double lhs, const Matrix & rhs) {
	return rhs*lhs;
}

Matrix & Matrix::operator*=(double rhs) {
	for ( unsigned int i = 0; i < this->rows * this->columns; ++i )
		this->data[i] *= rhs;
	return *this;
}

Matrix Matrix::operator-() const {
	Matrix temp(*this);
	
	for (unsigned int i = 0; i < this->rows * this->columns; ++i )
		temp.data[i] = -temp.data[i];

	return temp;
}
