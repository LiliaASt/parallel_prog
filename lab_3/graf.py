import matplotlib.pyplot as plt
import numpy as np

data = {
    "2 процесса": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.000362832, 0.00238521, 0.0361243, 0.370138, 1.56183,
                  6.00893, 12.449, 31.9655, 66.3468]
    },
    "5 процессов": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.000212184, 0.0014972, 0.0194731, 0.167477, 0.770571,
                  2.25597, 4.49865, 9.0901, 23.3257]
    },
    "10 процессов": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.00017901, 0.00163199, 0.0230381, 0.221024, 0.705775,
                  2.0836, 4.00801, 9.7946, 29.0264]
    },
    "15 процессов": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.000272316, 0.00152061, 0.0205649, 0.188315, 0.647608,
                  2.42113, 4.42007, 10.3891, 31.2816]
    }
}

plt.figure(figsize=(14, 8))

# Цвета и маркеры
colors = ['b', 'g', 'r', 'c']
markers = ['o', 's', '^', 'D']

# Создаем график
for i, (processes, values) in enumerate(data.items()):
    sizes = values["sizes"]
    times = values["times"]
    plt.plot(sizes, times, marker=markers[i], color=colors[i],
             label=processes, linewidth=2, markersize=8, linestyle='-')

    # Подписи для ключевых точек
    label_sizes = [100, 500, 1000, 2000, 3000, 4000]
    for size, time in zip(sizes, times):
        if size in label_sizes:
            plt.text(size, time, f"{time:.3f} с", fontsize=9,
                    ha='center', va='bottom', color=colors[i],
                    bbox=dict(facecolor='white', alpha=0.7, edgecolor='none', pad=1))

# Настройки графика
plt.title('Зависимость времени умножения матриц от размера и количества процессов на суперкомпьютере',
          fontsize=14, pad=20)
plt.xlabel('Размер матрицы (N x N)', fontsize=12)
plt.ylabel('Время выполнения (секунды)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=12, loc='upper left')

plt.tight_layout()
plt.savefig('matrix_sc_times_log.png', dpi=300, bbox_inches='tight')
plt.show()
