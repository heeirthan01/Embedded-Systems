import numpy as np
import matplotlib.pyplot as plt



def calc_cdf(arr):
    
    unique_values, counts = np.unique(arr, return_counts=True)
    cum_cts = np.cumsum(counts)
    cdf_vals = cum_cts / len(arr)

    return unique_values, cdf_vals

def confidence_interval(arr, no_runs):
    means = []

    for i in range(no_runs):
        sample = np.random.choice(arr, size = len(arr), replace=True)
        means.append(np.mean(sample))
    
    overall_mean = np.mean(means)
    conf_95pc = np.percentile(means, [2.5, 97.5])

    return overall_mean, conf_95pc


f = np.loadtxt('part_b_nocache.txt')
arr = np.asarray(f)

no_runs = 1000
mean_tot, cfdnce = confidence_interval(arr, no_runs)

print(f'Mean: {mean_tot} and 95% confidence interval: {cfdnce}')

#Comment out to plot cdf

x, cdf = calc_cdf(arr)
print(f'x values : {x}, cdf values : {cdf}')
# x = np.insert(x, 0, 0)
# cdf = np.insert(cdf, 0, 0)

# print(x)
# print(cdf)
# #plt.plot(x,cdf)
# plt.step(x, cdf, where = 'post')
# plt.title('CDF part b with cache')
# plt.xlabel('Value')
# plt.ylabel('CDF')
# plt.xlim([0, 19])
# plt.xticks(range(0, 21))
# plt.ylim([0, 1])
# plt.grid(True)
# plt.savefig("plots/part_b_wcache.png", dpi=300, bbox_inches="tight")
# plt.show()