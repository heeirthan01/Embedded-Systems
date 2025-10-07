import numpy as np
import matplotlib.pyplot as plt
from stats import calc_cdf

exercises = ['1', '2', '3', '4', '5', '6']
num_trials = ['5', '15', '25', '40']
for exercise in exercises:
    num_trials_arr = []
    means = []
    means_per_op = []
    for num_trial in num_trials:
        with open('lab_data/'+'e'+exercise+'_'+num_trial+'.csv', 'r') as f:
            i = 0
            els = []
            for line in f.readlines():
                if i < 8:
                    i += 1
                    continue
                if line[0] == '-' or line[0] == 'a':
                    i += 1
                    continue

                arr_el = float(line.split(',')[-1])
                els.append(arr_el)
                i += 1
            for j in range(1000):
                sample = np.random.choice(els, size = len(els), replace=True)
                mean = np.mean(sample)
                mean_per_op = mean / int(num_trial)
                means_per_op.append(mean_per_op)
                means.append(mean)
                num_trials_arr.append(int(num_trial))
            
            mean_tot = np.mean(means_per_op)
            conf_95pc = np.percentile(means_per_op, [2.5, 97.5])
            print(f'Exercise {exercise} with {num_trial} trials has mean {mean_tot} and confidence interval {conf_95pc}')
            x, cdf = calc_cdf(means_per_op)
            x = np.insert(x, 0, 0)
            cdf = np.insert(cdf, 0, 0)
            #plt.plot(x,cdf)
            plt.step(x, cdf, where = 'post')
            plt.title('CDF for Exercise '+exercise+' with '+num_trial+' trials')
            plt.xlabel('Value')
            plt.ylabel('CDF')
            plt.grid(True)
            plt.show()

    # linear regression
    means = np.array(means)
    m, b = np.polyfit(num_trials_arr, means, 1)
    print(f'Exercise {exercise} has slope {m} and intercept {b}')
    plt.title(f'Exercise {exercise} Mean Access Cycles vs Number of Trials')
    plt.xlabel('Number of Trials')
    plt.ylabel('Mean Cycles')
    plt.plot(num_trials_arr, m*np.array(num_trials_arr) + b, color='red', label='Linear Fit')
    plt.scatter(num_trials_arr, means)
    plt.savefig("plots/exercise_"+exercise+"_mean_vs_trials.png", dpi=300, bbox_inches="tight")
    plt.show()
    plt.hist(means, bins=30)
    plt.title(f'Exercise {exercise} Mean Access Cycles Distribution')
    plt.show()