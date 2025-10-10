import numpy as np
import matplotlib.pyplot as plt


if __name__ == '__main__':

    histdata = []
    with open('histhw2partc.txt', 'r') as file:
        cnt = 0
        for line in file:
            if line[0] == 'H':
                if cnt == 0:
                    data = []
                    cnt += 1
                    continue
                else:
                    histdata.append(data)
                    data = []
                    continue
            x = line.split(',')   
            data.append((float(x[0]), float(x[1]))) 
        histdata.append(data)

    means = []
    i = 0
    for intr in histdata:
    
    
        vals = 0.0
        counts = 0
        x = []
        cdf = []
        arrtot = np.array([], dtype=int)
        for data in intr:
            x.append(data[0])
            cdf.append(data[1])
            cycle = int(data[0])
            count = data[1]
            counts += count
            vals += cycle * count
            a = np.ones(int(count), dtype=int) 
            arrtot = np.concatenate((arrtot, a * cycle))
        means.append(vals / counts)
        
        #Confidence interval
        cfd = np.percentile(arrtot, [2.5, 97.5])
        print(f' Mean service time: {vals / counts} and 95% confidence interval: {cfd} for interrupt {i}')
        #CDF stuff
        cum_cts = np.cumsum(cdf)
        cdf_vals = cum_cts / counts
        plt.step(x, cdf_vals, where='post')
        plt.title('CDF of Service Time for interrupt '+str(i))
        plt.xlabel('Service Time (cycles)')
        plt.ylabel('CDF')
        #plt.savefig("plots/cdf_for_interrupt"+str(i)+".png", dpi=300, bbox_inches="tight")
        plt.show()
        i+=1
    



        