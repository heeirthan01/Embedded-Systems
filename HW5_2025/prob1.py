import numpy as np
import matplotlib.pyplot as plt

samples = np.arange(50)
Ts = 0.001

freqs = [250, 333, 667, 750, 1250, 1333]

waves = [np.sin(2 * np.pi * freq * samples) for freq in freqs]

for i in range(len(freqs)):
    wave = np.sin(2 * np.pi * freqs[i] * samples * Ts)
    t = samples * Ts
    plt.xlabel('Time (s)')
    plt.ylabel('Amplitude')
    plt.title('Plot for Frequency '+str(freqs[i]))
    plt.scatter(t, wave)
    plt.plot(t, wave)
    plt.savefig('HW5_2025/plots/plotfreq'+str(freqs[i])+'.png')
    plt.close()