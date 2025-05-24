import matplotlib.pyplot as plt
import numpy as np

matrix_sizes = [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000]
times = [0.00555312, 0.0445903, 0.7347004, 6.550742, 24.59438,
         113.46978, 220.356, 462.0974, 1017.085]

plt.figure(figsize=(10, 6))
plt.plot(matrix_sizes, times, 'o-', label='Среднее время выполнения')

plt.xlabel('Размер матрицы (N x N)', fontsize=12)
plt.ylabel('Время выполнения (секунды)', fontsize=12)
plt.title('Зависимость времени выполнения от размера матрицы', fontsize=14)
plt.grid(True, which="both", ls="-", alpha=0.5)

for i, (size, time) in enumerate(zip(matrix_sizes, times)):
    if i % 2 == 0:
        plt.annotate(f"{time:.2f} с", (size, time), textcoords="offset points",
                     xytext=(0,10), ha='center')

plt.legend(fontsize=12)

plt.tight_layout()
plt.savefig('matrix_time_plot.png', dpi=300)
plt.show()
