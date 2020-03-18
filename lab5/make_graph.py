import numpy as np 
import matplotlib.pyplot as plt

x = np.array([1,
2,
3,
4,
5,
6,
7,
8,
9
])
# cbr = np.array([821,1641,2462,3282,4099,4586,4597,4604,4611,4615])
# ftp = np.array([4003,4000,3996,3992,3625,3144,3141,3139,3139,3137])

cbr = np.array([0,
4.04,
6.05,
8.07,
10.08,
12.09,
14.099,
16.09,
18.1
])
ftp = np.array([0,
4.04,
6.05,
8.07,
10.083,
12.089,
14.096,
16.1,
18.1
])

plt.plot(x,cbr)
plt.plot(x,ftp)
plt.show()