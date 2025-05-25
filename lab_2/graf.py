import matplotlib.pyplot as plt
import numpy as np

data = {
    "2 потока": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.00297406, 0.0163028, 0.238626, 2.225948, 9.620178,
                 26.60344, 56.62016, 119.6356, 266.4938]
    },
    "5 потоков": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.00233548, 0.01418254, 0.1485558, 1.101212, 4.317358,
                 13.64298, 26.5193, 61.85082, 165.7624]
    },
    "10 потоков": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.00210108, 0.0115744, 0.1289502, 0.8819496, 3.029588,
                 11.6883, 23.81652, 53.57852, 150.2994]
    },
    "15 потоков": {
        "sizes": [100, 200, 500, 1000, 1500, 2000, 2500, 3000, 4000],
        "times": [0.00250444, 0.0105127, 0.09892888, 0.8125626, 2.738422,
                 10.381544, 21.8557, 51.19552, 158.6822]
    }
}

plt.figure(figsize=(12, 8))

# Цвета для разных линий
colors = ['b', 'g', 'r', 'c']

# Рисуем кривые для каждого количества потоков
for i, (threads, values) in enumerate(data.items()):
    sizes = values["sizes"]
    times = values["times"]
    plt.plot(sizes, times, 'o-', color=colors[i], label=threads, linewidth=2)

    # Добавляем подписи точек
    for size, time in zip(sizes, times):
        if size in [100, 500, 1000, 2000, 3000, 4000]:  # Подписываем только некоторые точки
            plt.text(size, time, f"{time:.3f} с", fontsize=8,
                    ha='center', va='bottom', color=colors[i])

# Настройки графика
plt.title('Зависимость времени умножения матриц от размера и количества потоков', fontsize=14)
plt.xlabel('Размер матрицы (N x N)', fontsize=12)
plt.ylabel('Время выполнения (секунды)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend(fontsize=12)


plt.tight_layout()
plt.savefig('matrix_times.png', dpi=300, bbox_inches='tight')
plt.show()
