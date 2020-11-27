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
            
            for (int j = 0; j < this->width; j++) {
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
            
            for (int i = 0; i < this->height; i++) {
                if (i < column.size())
                    this->matrix[i][j] = column[i];
                else
                    this->matrix[i][j] = 0;
            }
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
        
        // Вернуть единичную матрицу
        static Matrix getIdentityMatrix(int size) {
            Matrix result = Matrix(size, size);
            
            for (int i = 0; i < size; i++)
                result.setAt(i, i, 1);
            
            return result;
        }
};

int main() {
    Matrix matrix = Matrix(5, 3);
    Matrix matrix2 = Matrix(5, 3);
    
    matrix.setAt(0, 2, 1);
    matrix2.setAt(2, 0, 2);
    
    matrix += 2;
    matrix -= 1;
    
    matrix2 += 4;
    
    Matrix matrix3 = matrix + matrix2;
    matrix3.transpose();
    
    Matrix matrix4 = matrix2 * matrix3;
    
    Matrix matrix5 = matrix4.getYoungDiagram();
    
    cout << matrix.toString();
    cout << endl << matrix2.toString();
    cout << endl << matrix3.toString();
    cout << endl << matrix4.toString();
    cout << endl << matrix5.toString();
    
    return 0;
}
