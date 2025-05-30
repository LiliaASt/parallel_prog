import numpy as np

def read_matrix(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        matrix = [list(map(int, line.strip().split())) for line in lines]
        return np.array(matrix, dtype=int)

size = 100
A = read_matrix(f"{size}\\{size}_matrix_A.txt")
B = read_matrix(f"{size}\\{size}_matrix_B.txt")
C_computed = read_matrix(f"{size}\\{size}_result_matrix.txt")

C_expected = np.dot(A, B)

if np.array_equal(C_computed, C_expected):
    print("Результаты совпадают. Умножение выполнено корректно.")
else:
    print("Результаты не совпадают. Умножение выполнено некорректно.")
