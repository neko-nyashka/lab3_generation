import matplotlib.pyplot as plt
import csv

filename = 'results.csv'

sizes = []
mid_square_times = []
lcg_times = []
xorshift_times = []
mt_times = []

# Чтение CSV-файла
with open(filename, newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        sizes.append(int(row['size']))
        # В файле время в секундах, переводим в микросекунды
        mid_square_times.append(float(row['mid_square']) * 1_000_000)
        lcg_times.append(float(row['lcg']) * 1_000_000)
        xorshift_times.append(float(row['xorshift']) * 1_000_000)
        mt_times.append(float(row['mt19937']) * 1_000_000)

plt.figure(figsize=(10, 6))
plt.plot(sizes, mid_square_times, label='mid_square', marker='o')
plt.plot(sizes, lcg_times, label='lcg', marker='o')
plt.plot(sizes, xorshift_times, label='xorshift', marker='o')
plt.plot(sizes, mt_times, label='mt19937', marker='o')

plt.xlabel('Размер выборки (кол-во элементов)')
plt.ylabel('Время генерации (микросекунды)')
plt.title('Зависимость времени генерации от объема выборок')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')

plt.show()
