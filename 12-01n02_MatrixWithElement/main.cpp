#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <utility>
#include <iomanip>

using namespace std;

class Matrix {
private:
    vector<vector<double>> matrix;		// Матрица из векторов
    int width, height;					// Ширина и высота матрицы
    
    // Проверить границы матрицы
    bool checkBounds(int i, int j) {
        return i > -1 && j > -1 && i < this->width&& j < this->height;
    }
    
    // Проверить можно ли перемножить матрицы
    bool checkMultiply(Matrix& data) {
        return this->height == data.getWidth();
    }
    
    // Функция возвращает строку возрастающих значений {1, 2, 3, 2} - {1, 2 ,3}
    vector<double> getIncreasingElements(vector<double> Array) {
        for (int i = 0; i < Array.size() - 1; i++)
            if (Array[i] > Array[i + 1])
                return vector<double>(Array.begin(), Array.begin() + i + 1);
        
        return Array;
    }
    
    // Функция проверяет строки, чтобы нижняя строка содержала большие значения
    vector<double> getAfterCompare(vector<double> First, vector<double> Second) {
        int size = First.size() > Second.size() ? Second.size() : First.size();
        for (int i = 0; i < size; i++)
            if (First[i] > Second[i])
                return vector<double>(Second.begin(), Second.begin() + i);
        
        return First.size() != 0 ? Second : vector<double>();
    }
    
    // Отбрасывает пустой вектор и последующие
    vector<vector<double>> getResultCompared(vector<vector<double>> compared) {
        for (int i = 0; i < compared.size(); i++)
            if (compared[i].size() == 0)
                return vector<vector<double>>(compared.begin(), compared.begin() + i);
        
        return compared;
    }
    
    public:
        Matrix(const int &width, const int &height) {
            this->width = width;
            this->height = height;
            this->clear();
        }
        
        Matrix(const Matrix& createFrom) {
            this->width = createFrom.width;
            this->height = createFrom.height;
            this->clear();
            this->fillFrom(createFrom.matrix);
        }
        
        Matrix(const Matrix* createFrom) {
            this->width = createFrom->width;
            this->height = createFrom->height;
            this->clear();
            this->fillFrom(createFrom->matrix);
        }
        
        Matrix(const vector<vector<double>>& createFrom) {
            this->width = createFrom.size();
            this->height = createFrom[0].size();
            this->clear();
            fillFrom(createFrom);
        }
        
        Matrix* clone() const {
            return new Matrix(this);
        }
        
        int getWidth() { return this->width; }
        int getHeight() { return this->height; }
        
        vector<vector<double>> getMatrix() { return this->matrix; }
        
        void fillFrom(vector<vector<double>> toFill) {
            for (int i = 0; i < this->width; i++) {
                for (int j = 0; j < this->height; j++) {
                    if (i < toFill.size() && j < toFill[i].size())
                        this->matrix[i][j] = toFill[i][j];
                    else
                        this->matrix[i][j] = 0;
                }
            }
        }
        
        // Вернуть значение из клетки матрицы
        double getAt(int i, int j) {
            if (!this->checkBounds(i, j))
                throw string("Index out of bounds exception at get " + to_string(i) + " : " + to_string(j) + '\n');
            
            return this->matrix[i][j];
        }
        
        // Установить значение в клетку матрицы
        void setAt(int i, int j, double data) {
            if (!this->checkBounds(i, j))
                throw string("Index out of bounds exception at set " + to_string(i) + " : " + to_string(j) + '\n');
            
            this->matrix[i][j] = data;
        }
        
        // Добавить в значение матрицы
        void addAt(int i, int j, double data) {
            if (!this->checkBounds(i, j))
                throw string("Index out of bounds exception at add " + to_string(i) + " : " + to_string(j) + '\n');
            
            this->matrix[i][j] += data;
        }
        
        //
        void divideAll(double data) {
            for(int i = 0; i < this->width; i++)
                for(int j = 0; j < this->height; j++)
                    this->matrix[i][j] /= data;
        }
        
        //
        void mulitplyAll(double data) {
            for(int i = 0; i < this->width; i++)
                for(int j = 0; j < this->height; j++)
                    this->matrix[i][j] *= data;
        }
        
        double findMaxElement() {
            double max = this->matrix[0][0];
            
            for(vector<double> i : this->matrix)
                for(double j : i)
                    if(j > max)
                        max = j;
            
            return max;
        }
        
        // Очистить матрицу, заполнить ее нулями
        void clear() {
            this->matrix = vector<vector<double>>();
            for (int i = 0; i < width; i++) {
                this->matrix.push_back(vector<double>());
                for (int j = 0; j < height; j++)
                    this->matrix[i].push_back(0);
            }
        }
        
        // Транспонировать матрицу
        void transpose() {
            vector<vector<double>> result = vector<vector<double>>();
            
            for (int i = 0; i < this->height; i++) {
                result.push_back(vector<double>());
                for (int j = 0; j < this->width; j++) {
                    result[i].push_back(this->matrix[j][i]);
                }
            }
            
            swap(this->width, this->height);
            this->matrix = result;
        }
        
        // Сформировать матрицу в строку
        string toString() {
            string result = "";
            for (vector<double> i : this->matrix) {
                for (double j : i)
                    result += to_string(j) + ' ';
                
                result += '\n';
            }
            
            return result;
        }
        
        /* ==== Перегрузка операторов ==== */
        Matrix operator+(const double& data) {
            vector<vector<double>> result = vector<vector<double>>();
            for (int i = 0; i < this->width; i++) {
                result.push_back(vector<double>());
                for (int j = 0; j < this->height; j++)
                    result[i].push_back(this->matrix[i][j] + data);
            }
            
            return Matrix(result);
        }
        
        void operator+=(const double& data) {
            for (int i = 0; i < this->width; i++)
                for (int j = 0; j < this->height; j++)
                    this->matrix[i][j] += data;
        }
        
        Matrix operator-(const double& data) {
            vector<vector<double>> result = vector<vector<double>>();
            for (int i = 0; i < this->width; i++) {
                result.push_back(vector<double>());
                for (int j = 0; j < this->height; j++)
                    result[i].push_back(this->matrix[i][j] - data);
            }
            
            return Matrix(result);
        }
        
        void operator-=(const double& data) {
            for (int i = 0; i < this->width; i++)
                for (int j = 0; j < this->height; j++)
                    this->matrix[i][j] -= data;
        }
        
        Matrix operator+(Matrix& data) {
            if (this->width != data.getWidth() || this->height != data.getHeight())
                throw string("Matrices have different sizes\n");
            
            Matrix result = Matrix(this->width, this->height);
            
            for (int i = 0; i < this->width; i++)
                for (int j = 0; j < this->height; j++)
                    result.setAt(i, j, this->matrix[i][j] + data.getAt(i, j));
            
            return result;
        }
        
        Matrix operator-(Matrix& data) {
            if (this->width != data.getWidth() || this->height != data.getHeight())
                throw string("Matrices have different sizes\n");
            
            Matrix result = Matrix(this->width, this->height);
            
            for (int i = 0; i < this->width; i++)
                for (int j = 0; j < this->height; j++)
                    result.setAt(i, j, this->matrix[i][j] - data.getAt(i, j));
            
            return result;
        }
        
        Matrix operator*(Matrix& data) {
            if (!this->checkMultiply(data))
                throw string("Matrices has different height and width\n");
            
            Matrix result = Matrix(this->width, data.getHeight());
            
            for (int i = 0; i < this->width; i++) {
                for (int j = 0; j < this->height; j++) {
                    for (int k = 0; k < data.getHeight(); k++) {
                        result.addAt(i, k, this->matrix[i][j] * data.getAt(j, k));
                    }
                }
            }
            
            return result;
        }
        
        vector<double>* operator[](const int &i) {
            if(!this->checkBounds(i, 0))
                throw string("Index of bounds exception");
            
            return &this->matrix[i];
        }
        
        // Вернуть строку матрицы по номеру
        vector<double> getRow(int i) {
            if (!checkBounds(i, 0))
                throw string("Index of bounds exception\n");
            
            return this->matrix[i];
        }
        
        // Вернуть колонку матрица по номеру
        vector<double> getColumn(int j) {
            if (!checkBounds(0, j))
                throw string("Index of bounds exception\n");
            
            vector<double> result = vector<double>();
            for (int i = 0; i < this->width; i++)
                result.push_back(this->matrix[i][j]);
            
            return result;
        }
        
        // Установить строку матрицы
        void setRow(int i, vector<double> row) {
            if (!checkBounds(i, 0))
                throw string("Index of bounds exception\n");
            
            for (int j = 0; j < this->height; j++) {
                if (j < row.size())
                    this->matrix[i][j] = row[j];
                else
                    this->matrix[i][j] = 0;
            }
        }
        
        // Установить колонку матрицы
        void setColumn(int j, vector<double> column) {
            if (!checkBounds(0, j))
                throw string("Index of bounds exception\n");
            
            for (int i = 0; i < this->width; i++) {
                if (i < column.size())
                    this->matrix[i][j] = column[i];
                else
                    this->matrix[i][j] = 0;
            }
        }
        
        // Поменять строки местами
        void swapRows(int first, int second) {
            if(!checkBounds(first, 0) || !checkBounds(second, 0))
                throw string("Index of bounds exception\n");
            
            vector<double> firstRow = this->getRow(first);
            vector<double> secondRow = this->getRow(second);
            
            this->setRow(first, secondRow);
            this->setRow(second, firstRow);
        }
        
        // Поменять колонки местами
        void swapColumns(int first, int second) {
            if(!checkBounds(0, first) || !checkBounds(0, second))
                throw string("Index of bounds exception\n");
            
            vector<double> firstColumn = this->getColumn(first);
            vector<double> secondColumn = this->getColumn(second);
            
            this->setColumn(first, secondColumn);
            this->setColumn(second, firstColumn);
        }
        
        // Создать подматрицу со смещением от левого верхнего угла
        Matrix getSubMatrix(int indentI, int indentJ) {
            if(!checkBounds(indentI, indentJ))
                throw string("Index of bounds exception\n");
            
            vector<vector<double>> result = vector<vector<double>>();
            
            for(int i = indentI; i < this->width; i++) {
                result.push_back(vector<double>());
                for(int j = indentJ; j < this->height; j++) {
                    result[i].push_back(this->matrix[i][j]);
                }
            }
            
            return Matrix(result);
        }
        
        // Вернуть диаграмму Юнга
        Matrix getYoungDiagram() {
            vector<vector<double>> temp = vector<vector<double>>();
            
            for (int i = 0; i < this->width; i++)
                temp.push_back(getIncreasingElements(this->matrix[i]));
            
            for (int i = 0; i < temp.size() - 1; i++)
                temp[i + 1] = getAfterCompare(temp[i], temp[i + 1]);
            
            temp = getResultCompared(temp);
            Matrix result = Matrix(temp);
            
            return result;
        }
        
        // Проверить является ли матрица пустой, то есть не содержит строк и колонок
		// Или содержит только нули.
        bool isNullable() {
            if(this->width == 0 || this->height == 0)
                return true;
            
            for(int i = 0; i < this->width; i++)
                for(int j = 0; j < this->height; j++)
                    if(this->matrix[i][j] != 0)
                        return false;
            
            return true;
        }
        
        // Найти позицию заданного элемента, либо элемента отличного от заданного (В зависимости от параметра match)
		// Со смещением startI, startJ от левого верхнего края матрицы. И вернуть позицию
        pair<int, int> searchPosition(double find, bool match, int startI = 0, int startJ = 0) {
            if(!this->checkBounds(startI, 0) || !this->checkBounds(0, startJ))
                throw string("Index of bounds exception\n");
            
            for(int i = startI; i < this->width; i++) {
                for(int j = startJ; j < this->height; j++) {
                    if(match) {
                        if(this->matrix[i][j] == find)
                            return pair<int, int>(i, j);
                    } else {
                        if(this->matrix[i][j] != find)
                            return pair<int, int>(i, j);
                    }
                }
            }
            
            return pair<int, int>(-1, -1);
        }
        
        // Вернуть определитель матрицы
        double getDeterminant() {
            if(this->width != this->height)
                throw string("Matrix must be square\n");
            
            int size = this->width;
            
            if(this->isNullable())
                return 0;
            
            if(size == 1)
                return this->matrix[0][0];
                
            if(size == 2)
                return (this->matrix[0][0] * this->matrix[1][1] - this->matrix[1][0] * this->matrix[0][1]);
            
            bool sign = false;
            double det = 1, temp;
            Matrix detMat = Matrix(this->matrix);
            
            for(int i = 0; i < size; i++) {
                if(detMat.getAt(i, i) == 0) {
                    pair<int, int> findElement = detMat.searchPosition(0, false, i, i);
                    if(findElement.first == -1)
                        return 0;
                    
                    if(findElement.first != i) {
                        detMat.swapRows(i, findElement.first);
                        sign = !sign;
                    }
                    
                    if(findElement.second != i) {
                        detMat.swapColumns(i, findElement.second);
                        sign = !sign;
                    }
                }
                
                det *= detMat.getAt(i, i);
                temp = detMat.getAt(i, i);
                
                for(int j = i; j < size; j++)
                    detMat.setAt(i, j, detMat.getAt(i, j) / temp);
                
                for(int j = i + 1; j < size; j++) {
                    temp = detMat.getAt(j, i);
                    for(int k = i; k < size; k++) {
                        detMat.addAt(j, k,  -(detMat.getAt(i, k) * temp));
                    }
                }
            }
            
            return sign ? -det : det;
        }
        
        // Создать обратную матрицу к текущей
        Matrix getInverse() {
            try {
                if(this->getDeterminant() == 0)
                    throw string("Determinant equals zero\n");
            } catch(string Exp) {
                throw Exp;
            }
            
            double temp;
            int size = this->width;
            Matrix augmented = Matrix::getAugmentedMatrix(this);
            
            for(int i = size - 1; i > 0; i--)
                if(augmented.getAt(i - 1, 0) < augmented.getAt(i, 0))
                    augmented.swapRows(i - 1, i);
            
            for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                    if(j != i) {
                        temp = augmented.getAt(j, i) / augmented.getAt(i, i);
                        for(int k = 0; k < augmented.getHeight(); k++)
                            augmented.addAt(j, k, -(augmented.getAt(i, k) * temp));
                    }
                }
            }
            
            for(int i = 0; i < size; i++) {
                temp = augmented.getAt(i, i);
                for(int j = 0; j < augmented.getHeight(); j++)
                    augmented.setAt(i, j, augmented.getAt(i, j) / temp);
            }
            
            return augmented.getSubMatrix(0, size);
        }
        
        //
        double getDeterminantByMaxElement() {
            Matrix result = Matrix(this->matrix);
            result.divideAll(result.findMaxElement());
            return result.getDeterminant();
        }
        
        //
        Matrix getInverseByMaxElement() {
            Matrix result = Matrix(this->matrix);
            result.divideAll(result.findMaxElement());
            return result.getInverse();
        }
        
        // Вернуть единичную матрицу
        static Matrix getIdentityMatrix(int size) {
            Matrix result = Matrix(size, size);
            
            for (int i = 0; i < size; i++)
                result.setAt(i, i, 1);
            
            return result;
        }
        
        // Вернуть расширенную матрицу
        static Matrix getAugmentedMatrix(Matrix from) {
            if(from.getWidth() != from.getHeight())
                throw string("Matrix must be square\n");
            
            Matrix result = Matrix(from.getWidth(), from.getWidth() * 2);
            
            for(int i = 0; i < from.getWidth(); i++)
                result.setRow(i, from.getRow(i));
            
            for(int i = 0; i < from.getWidth(); i++) {
                result.setAt(i, i + from.getWidth(), 1);
            }
            
            return result;
        }
};

int main() {
    //Matrix matrix = Matrix(5, 3);
    //Matrix matrix2 = Matrix(5, 3);
    
    //matrix.setAt(0, 2, 1);
    //matrix2.setAt(2, 0, 2);
    
    //matrix += 2;
    //matrix -= 1;
    
    //matrix2 += 4;
    
    //Matrix matrix3 = matrix + matrix2;
    //matrix3.transpose();
    
    //Matrix matrix4 = matrix2 * matrix3;
    
    //Matrix matrix5 = matrix4.getYoungDiagram();
    
    Matrix matrix6 = Matrix(3, 3);
    matrix6.setRow(0, vector<double>({5, 7, 9}));
    matrix6.setRow(1, vector<double>({4, 3, 8}));
    matrix6.setRow(2, vector<double>({7, 5, 6}));
    
    Matrix matrix7 = matrix6.getInverse();
    Matrix matrix8 = matrix6 * matrix7;
    
    Matrix matrixDivided = Matrix(matrix6.getMatrix());
    Matrix matrixDividedInverse = matrix6.getInverseByMaxElement();
    
    Matrix resultDivided = matrixDivided * matrixDividedInverse;
    
    cout << "Matrix determinant: " << matrix6.getDeterminant() << endl;
    //cout << matrix.toString();
    //cout << endl << matrix2.toString();
    //cout << endl << matrix3.toString();
    //cout << endl << matrix4.toString();
    //cout << endl << matrix5.toString();
    cout << endl << "Source matrix:";
    cout << endl << matrix6.toString();
    cout << endl << "Inverse matrix:";
    cout << endl << matrix7.toString();
    cout << endl << "Source divided:";
    cout << endl << matrixDivided.toString();
    cout << endl << "Inverse divided by max:";
    cout << endl << matrixDividedInverse.toString();
    cout << endl << "Multiply source on inverse:";
    cout << endl << matrix8.toString();
    cout << endl << "Multiply divided matrices:";
    cout << endl << resultDivided.toString();
    
    //cout << endl << matrix5[0][0] << " " << matrix5[1][0] << " " << matrix5[2][0] << endl;
    
    return 0;
}
