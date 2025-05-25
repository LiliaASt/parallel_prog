#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <cstdlib>

using namespace std;

void readMatrix(const string& filename, vector<vector<int>>& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }
    for (auto& row : matrix) {
        for (auto& elem : row) {
            file >> elem;
        }
    }
}

void writeMatrix(const string& filename, const vector<vector<int>>& matrix) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл: " + filename);
    }
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            file << elem << " ";
        }
        file << "\n";
    }
}

void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int num_threads) {
    int n = A.size();
    int m = B[0].size();
    int p = A[0].size();

    #ifdef _OPENMP
    omp_set_num_threads(num_threads);
    #endif

    #pragma omp parallel for shared(A, B, C) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int sum = 0;
            for (int k = 0; k < p; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main(int argc, char* argv[]) {
    const int SIZE = 4000;
    vector<vector<int>> A(SIZE, vector<int>(SIZE));
    vector<vector<int>> B(SIZE, vector<int>(SIZE));
    vector<vector<int>> C(SIZE, vector<int>(SIZE));

    int num_threads = omp_get_max_threads();
    if (argc > 1) {
        num_threads = atoi(argv[1]);
        if (num_threads <= 0) {
            cerr << "Ошибка: число потоков должно быть > 0\n";
            return 1;
        }
    }

    cout << "Используется " << num_threads << " поток(ов)\n";

    try {
        readMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_matrix_A.txt", A);
        readMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_matrix_B.txt", B);

        auto start = chrono::high_resolution_clock::now();

        multiplyMatrices(A, B, C, num_threads);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        writeMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_result_matrix.txt", C);

        ofstream timeFile(to_string(SIZE) + "\\" + to_string(SIZE) + "_performance.txt");
        if (!timeFile.is_open()) {
            throw runtime_error("Не удалось открыть файл: performance.txt");
        }

        timeFile << "Время выполнения: " << elapsed.count() << " секунд\n";
        timeFile << "Использовано потоков: " << num_threads << "\n";
        timeFile << "Объем задачи: " << SIZE << " x " << SIZE << "\n";
        size_t total_elements = 2 * SIZE * SIZE;
        timeFile << "Объем задачи (элементов): " << total_elements << "\n";

        timeFile.close();

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
