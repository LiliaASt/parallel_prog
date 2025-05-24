import numpy as np

rows = cols = 4000

random_matrix_1 = np.random.randint(0, 101, size=(rows, cols))
random_matrix_2 = np.random.randint(0, 101, size=(rows, cols))

np.savetxt(f'{rows}\\{rows}_matrix_A.txt', random_matrix_1, fmt='%d')
print(f"Матрица A {rows} на {cols} сохранена")

np.savetxt(f'{rows}\\{rows}_matrix_B.txt', random_matrix_2, fmt='%d')
print(f"Матрица B {rows} на {cols} сохранена")

