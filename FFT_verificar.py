import numpy as np

# Define la frecuencia de muestreo
frecuencia_muestreo = 1024  # Hz

# Carga los datos del archivo de texto
try:
    signal = np.loadtxt('fo.txt')
except IOError:
    print("Error al leer el archivo.")
    exit()

# Calcula la FFT
n = len(signal)
fft_signal = np.fft.fft(signal)
frequencies = np.fft.fftfreq(n, 1/frecuencia_muestreo)

# Calcula la magnitud del espectro (la mitad, debido a la simetría de la FFT)
magnitudes = np.abs(fft_signal)[:n // 2] * (2.0 / n)

# Encuentra la magnitud más alta y su correspondiente frecuencia
indice_pico = np.argmax(magnitudes)  # Índice del pico más alto
frecuencia_pico = frequencies[indice_pico]  # Frecuencia del pico más alto
magnitud_pico = magnitudes[indice_pico]  # Magnitud del pico más alto

print(f"La frecuencia con la magnitud más alta es {frecuencia_pico} Hz")
print(f"La magnitud de esta frecuencia es {magnitud_pico}")
