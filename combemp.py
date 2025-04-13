import numpy as np
import matplotlib.pyplot as plt

# 입력 크기
n_values = np.array([1000, 10000, 100000, 1000000])

# 실행 시간
comb_sort = np.array([0.1, 2.2, 25.6, 317.0])
merge_sort = np.array([0.0, 1.2, 19.0, 210.6])
heap_sort  = np.array([0.2, 2.0, 28.9, 399.8])

# n log n 곡선 정규화
nlogn = n_values * np.log2(n_values)
nlogn = nlogn / nlogn[-1] * comb_sort[-1]

# log2(n) 축
log2_n = np.log2(n_values)

plt.figure(figsize=(10, 6))
plt.plot(log2_n, comb_sort, 'o-', label='Comb Sort', linewidth=2)
plt.plot(log2_n, merge_sort, 's--', label='Merge Sort', linewidth=2)
plt.plot(log2_n, heap_sort, 'd-.', label='Heap Sort', linewidth=2)
plt.plot(log2_n, nlogn, 'k:', label=r'$O(n \log n)$ (normalized)', linewidth=2)

# 축 설정
plt.xlabel(r'$\log_2(n)$', fontsize=12)
plt.ylabel('Time (ms)', fontsize=12)
plt.title('Runtime vs. log₂(n)', fontsize=14)
plt.xticks(log2_n, [f'{int(n):,}' for n in n_values])  # n을 직접 보여줌
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
