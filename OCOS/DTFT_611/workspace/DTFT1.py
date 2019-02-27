import numpy as np
import matplotlib.pyplot as plt

def DTFT_abs(x, M):
    return (-0.5+(np.arange(M)/M), 
            abs(np.fft.fftshift(np.fft.fft(x, M))))
    
N=10
nu0=0.1
x=np.ones(N)
k=np.arange(x.size)

#w2=np.linspace(-0.5, 0.5, 11)

#x=np.cos(2*np.pi*k/5);
#x=np.sin(2*np.pi*k/5);
#x=np.cos(2*np.pi*k*8/64)+np.cos(2*np.pi*k*8.5/64);
#x=np.cos(2*np.pi*k*8/64)+np.cos(2*np.pi*k*10.5/64);


#x=x*k
#x=x*np.exp(1j*2*np.pi*k*nu0)

#u=0
#x1=x.copy()
#L=3
#Q=(N-1)*(L-1)+N
#x=np.zeros(Q)
#for w in range(0, Q, L):
#    x[w]=x1[u];
#    u=u+1;


#plt.figure(dpi=200)
#plt.stem(k, x)
#plt.show()

plt.figure(figsize=[8, 6], dpi=200)
plt.subplot(2,1,1)
plt.stem(np.arange(x.size), x)
#plt.stem(np.arange(x.size), x.real, 'b', markerfmt='bo', label='Re x(k)')
#plt.stem(np.arange(x.size), x.imag, 'y', markerfmt='yo', label='Im x(k)')
#plt.legend(loc='best')
#plt.axis(ymin=-1.2, ymax=1.2*max(abs(x)))
#plt.xticks(np.arange(0, x.size, max(1, x.size//5)))
plt.axis(ymin=0, ymax=1.2*max(x))
plt.axis(xmin=-0.2, xmax=(x.size)+0.2)
plt.title(u'Signal $x(k)$, N=%i' %N);
plt.xlabel(u'$k$');
plt.ylabel(u'$x(k)$');
plt.grid()


M=1024;
plt.subplot(2,1,2)
f, Xn = DTFT_abs(x, M)
plt.plot(f, Xn, 'g'); 
plt.axis(xmin=-0.5, xmax=0.5, ymin=0)
plt.xticks(np.arange(-0.5, 0.6, 0.1))
plt.title(u'DTFT of $x(k)$')
plt.xlabel(u'$\\nu$')
plt.ylabel(u'$|X(\\nu)|$')
plt.grid()

plt.tight_layout()
plt.savefig('graph1.png', format = 'png')
plt.show()