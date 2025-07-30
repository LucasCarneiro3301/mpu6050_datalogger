import pandas as pd
import matplotlib.pyplot as plt

# Nome do arquivo CSV
csv_file = "arq.csv"  # Altere para o nome do seu arquivo, se necessário

# Lê o arquivo CSV
df = pd.read_csv(csv_file)

# Gráfico de aceleração (em g)
plt.figure(1)
plt.plot(df['numero_amostra'], df['accel_x'],'o--', label='Accel X (g)')
plt.plot(df['numero_amostra'], df['accel_y'],'o--', label='Accel Y (g)')
plt.plot(df['numero_amostra'], df['accel_z'],'o--', label='Accel Z (g)')
plt.title('Aceleração (g = 9.81 m/s²)')
plt.xlabel('Amostras')
plt.ylabel('Aceleração (g = 9.81 m/s²)')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Gráfico de giroscópio (em °/s)
plt.figure(2)
plt.plot(df['numero_amostra'], df['giro_x'],'o--', label='Giro X (°/s)')
plt.plot(df['numero_amostra'], df['giro_y'],'o--', label='Giro Y (°/s)')
plt.plot(df['numero_amostra'], df['giro_z'], 'o--',label='Giro Z (°/s)')
plt.title('Velocidade Angular (°/s)')
plt.xlabel('Amostras')
plt.ylabel('Velocidade Angular (°/s)')
plt.legend()
plt.grid(True)
plt.tight_layout()

# Mostra os dois gráficos
plt.show()

# Pausa o script no final (opcional)
input("Pressione Enter para sair...")
