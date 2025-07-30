import pandas as pd
import matplotlib.pyplot as plt

# Nome do arquivo CSV
csv_file = "exemplo.csv"  # Altere para o nome do seu arquivo, se necessário

# Lê o arquivo CSV
df = pd.read_csv(csv_file)

# Parâmetros de tamanho de fonte grandes
titulo_fontsize = 24
eixo_label_fontsize = 20
ticks_fontsize = 16

# Gráfico de aceleração (em g)
plt.figure(1)
plt.plot(df['numero_amostra'], df['accel_x'], 'o--', label='Accel X (g)')
plt.plot(df['numero_amostra'], df['accel_y'], 'o--', label='Accel Y (g)')
plt.plot(df['numero_amostra'], df['accel_z'], 'o--', label='Accel Z (g)')
plt.title('Aceleração (g = 9.81 m/s²)', fontsize=titulo_fontsize)
plt.xlabel('Amostras', fontsize=eixo_label_fontsize)
plt.ylabel('Aceleração (g = 9.81 m/s²)', fontsize=eixo_label_fontsize)
plt.legend(fontsize=14)
plt.grid(True)
plt.tick_params(axis='both', which='major', labelsize=ticks_fontsize)
plt.tight_layout()

# Gráfico de giroscópio (em °/s)
plt.figure(2)
plt.plot(df['numero_amostra'], df['giro_x'], 'o--', label='Giro X (°/s)')
plt.plot(df['numero_amostra'], df['giro_y'], 'o--', label='Giro Y (°/s)')
plt.plot(df['numero_amostra'], df['giro_z'], 'o--', label='Giro Z (°/s)')
plt.title('Velocidade Angular (°/s)', fontsize=titulo_fontsize)
plt.xlabel('Amostras', fontsize=eixo_label_fontsize)
plt.ylabel('Velocidade Angular (°/s)', fontsize=eixo_label_fontsize)
plt.legend(fontsize=14)
plt.grid(True)
plt.tick_params(axis='both', which='major', labelsize=ticks_fontsize)
plt.tight_layout()

# Mostra os dois gráficos
plt.show()

# Pausa o script no final (opcional)
input("Pressione Enter para sair...")
