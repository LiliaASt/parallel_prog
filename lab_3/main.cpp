#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <mpi.h>
#include <numeric>

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

void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C,
                     int start_row, int end_row) {
    int n = A.size();
    int m = B[0].size();
    int p = A[0].size();

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < m; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void processMatrixMultiplication(int SIZE, int rank, int size) {
    vector<vector<int>> A(SIZE, vector<int>(SIZE));
    vector<vector<int>> B(SIZE, vector<int>(SIZE));
    vector<vector<int>> C(SIZE, vector<int>(SIZE, 0));

    if (rank == 0) {
        readMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_matrix_A.txt", A);
        readMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_matrix_B.txt", B);
    }

    for (int i = 0; i < SIZE; ++i) {
        MPI_Bcast(B[i].data(), SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int rows_per_process = SIZE / size;
    int remainder = SIZE % size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;
    if (rank == size - 1) end_row += remainder;

    if (rank == 0) {
        for (int dest = 1; dest < size; ++dest) {
            int dest_start = dest * rows_per_process;
            int dest_end = dest_start + rows_per_process;
            if (dest == size - 1) dest_end += remainder;

            for (int i = dest_start; i < dest_end; ++i) {
                MPI_Send(A[i].data(), SIZE, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        for (int i = start_row; i < end_row; ++i) {
            MPI_Recv(A[i].data(), SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    vector<double> execution_times;
    bool result_written = false;

    for (int i = 0; i < 5; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        auto start = chrono::high_resolution_clock::now();

        multiplyMatrices(A, B, C, start_row, end_row);

        if (rank == 0) {
            for (int src = 1; src < size; ++src) {
                int src_start = src * rows_per_process;
                int src_end = src_start + rows_per_process;
                if (src == size - 1) src_end += remainder;

                for (int j = src_start; j < src_end; ++j) {
                    MPI_Recv(C[j].data(), SIZE, MPI_INT, src, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }

            if (!result_written) {
                writeMatrix(to_string(SIZE) + "\\" + to_string(SIZE) + "_result_matrix.txt", C);
                result_written = true;
            }
        } else {
            for (int j = start_row; j < end_row; ++j) {
                MPI_Send(C[j].data(), SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        auto end = chrono::high_resolution_clock::now();
        double elapsed = chrono::duration<double>(end - start).count();

        if (rank == 0) {
            execution_times.push_back(elapsed);
        }

        for (auto& row : C) {
            fill(row.begin(), row.end(), 0);
        }
    }

    if (rank == 0) {
        ofstream timeFile(to_string(SIZE) + "\\" + to_string(SIZE) + "_performance.txt");
        if (!timeFile.is_open()) {
            throw runtime_error("Не удалось открыть файл: performance.txt");
        }

        timeFile << "Результаты измерений для матрицы " << SIZE << "x" << SIZE << ":\n";
        for (size_t i = 0; i < execution_times.size(); ++i) {
            timeFile << "Измерение " << i + 1 << ": " << execution_times[i] << " сек\n";
        }

        double average_time = accumulate(execution_times.begin(), execution_times.end(), 0.0) / execution_times.size();
        timeFile << "Среднее время: " << average_time << " сек\n";
        timeFile << "Количество процессов: " << size << "\n";

        timeFile.close();
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const vector<int> SIZES = {100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000};

    try {
        if (rank == 0) {
            for (int SIZE : SIZES) {
                MPI_Bcast(&SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD);
                processMatrixMultiplication(SIZE, rank, size);
            }
            int end_flag = -1;
            MPI_Bcast(&end_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);
        } else {
            while (true) {
                int SIZE;
                MPI_Bcast(&SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD);
                if (SIZE == -1) break;
                processMatrixMultiplication(SIZE, rank, size);
            }
        }
    } catch (const exception& e) {
        cerr << "Error [" << rank << "]: " << e.what() << endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Finalize();
    return 0;
}
