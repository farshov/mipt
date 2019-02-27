import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
   
N=20
x=np.ones(N)
k=np.arange(N)
#x=np.cos(2*np.pi*k/5);
#x=np.sin(2*np.pi*k/5);
#x=np.cos(2*np.pi*k*8/64)+np.cos(2*np.pi*k*8.5/64);
#x=np.cos(2*np.pi*k*8/64)+np.cos(2*np.pi*k*10.5/64);

# Window 
window = signal.boxcar(x.size)
#window = signal.bartlett(x.size)
#window = signal.hann(x.size)
#window = signal.hamming(x.size)
#window = signal.blackman(x.size)
x=x*window


## DTFT part 
plt.figure(figsize=[6, 4], dpi=100)
M=2048
plt.plot(np.arange(M)/M, abs(np.fft.fft(x, M)), 'g'); 

# Zero Padding
# also we can use np.fft.fft(x, N+Nz)
Nz=0*N
x=np.hstack((x, np.zeros(Nz)))

## DFT part 
N=x.size
plt.stem(np.arange(N)/N, abs(np.fft.fft(x, N)), 'b', 'bo'); 
#plt.stem(np.arange(N)/N, abs(np.fft.fft(x, N)), 'b', 'b.'); 



plt.xticks(np.arange(0, 1.1, 0.1))
plt.axis(xmin=-0.02, xmax=0.5, ymin=-0.4)
#plt.axis(xmin=-0.02, xmax=1, ymin=-0.4)
plt.xlabel(u'$\\nu$')
plt.ylabel(u'$|X(\\nu)|$')
plt.grid()
plt.title(u'DTFT and DFT of $x(k)$')
#plt.savefig('graph1.png', format = 'png')
plt.show()



## Signal part
#plt.figure(figsize=[6, 4], dpi=100)
#plt.stem(np.arange(x.size), x, 'b', 'bo'); 
#plt.xticks(np.arange(0, x.size, max(1, x.size//5)))
#plt.title(u'Signal $x(k)$, N=%i' %(x.size));
#plt.xlabel(u'$k$');
#plt.ylabel(u'$x(k)$');
#plt.grid()
#plt.show()
