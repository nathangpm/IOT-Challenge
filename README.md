# PawCare Monitor

Monitoramento inteligente para pets em recuperação — FIAP Challenge 2026 | CLYVO VET


# Sobre o Projeto
O PawCare Monitor é uma solução IoT desenvolvida para resolver um problema crítico na jornada de saúde animal: a falta de monitoramento contínuo de pets em recuperação pós-cirúrgica ou em tratamento de doenças crônicas.
Hoje, após uma consulta ou cirurgia, o tutor vai para casa sem nenhuma forma de acompanhar se o ambiente está adequado para a recuperação do animal. O veterinário só terá acesso às informações na próxima consulta. O PawCare Monitor resolve isso com sensores conectados que monitoram o ambiente e o comportamento do pet em tempo real, enviando os dados para um dashboard acessível pelo tutor e pela clínica.

# Problema que Resolve

Tutores sem orientação após cirurgias e durante tratamentos
Ausência de monitoramento entre consultas
Dificuldade em identificar sinais de agravamento em casa
Falta de continuidade no cuidado entre o pet, o tutor e a clínica


# Solução
Uma estação de monitoramento posicionada próxima ao local de descanso do pet, composta por:

Sensor de temperatura e umidade — garante que o ambiente está adequado para a recuperação
Sensor de movimento (PIR) — detecta se o pet está em repouso ou se movimentando excessivamente (risco de rasgar pontos, por exemplo)

Os dados são enviados via MQTT para um dashboard Node-RED, acessível em tempo real.

Tecnologias Utilizadas
TecnologiaUsoESP32Microcontrolador principalDHT22Sensor de temperatura e umidadePIR (HC-SR501)Sensor de movimentoMQTTProtocolo de comunicação IoTHiveMQBroker MQTT públicoNode-REDDashboard de visualização dos dadosWokwiSimulação do circuitoArduino (C++)Linguagem de programação do ESP32

# Circuito
SensorPino ESP32FunçãoDHT22 — DATAGPIO 15Leitura de temperatura e umidadeDHT22 — VCC3.3VAlimentaçãoDHT22 — GNDGNDTerraPIR — OUTGPIO 14Detecção de movimentoPIR — VCC3.3VAlimentaçãoPIR — GNDGNDTerra

# Tópicos MQTT
TópicoDadospawcare/temperaturaTemperatura em °C (float)pawcare/umidadeUmidade relativa em % (float)pawcare/movimento1 = movimento detectado / 0 = repousopawcare/statusStatus de conexão do dispositivo

# Dashboard — Node-RED
O dashboard exibe em tempo real:

Gauge de Temperatura — com alertas visuais acima de 35°C e abaixo de 15°C
Gauge de Umidade — com alerta acima de 80%
Gráfico histórico de temperatura
Status do pet — "Pet em repouso" ou "MOVIMENTO DETECTADO!"
Painel de alertas — exibe automaticamente quando algum valor sai do normal


# Como Rodar o Projeto
Pré-requisitos

Node.js instalado
Node-RED instalado (npm install -g --unsafe-perm node-red)
Pacote de dashboard: node-red-dashboard
Conta no Wokwi (para simulação)

1. Simulação (Wokwi)

Acesse wokwi.com e crie um novo projeto ESP32
Importe os arquivos sketch.ino, diagram.json e libraries.txt
Clique em Play para iniciar a simulação
Interaja com os sensores clicando neles durante a simulação

2. Dashboard (Node-RED)
bash# Instalar Node-RED
npm install -g --unsafe-perm node-red

# Instalar dashboard
cd ~/.node-red
npm install node-red-dashboard

# Iniciar Node-RED
node-red

Acesse http://localhost:1880
Vá em Menu (☰) → Import e importe o arquivo flows.json
Clique em Deploy
Acesse o dashboard em http://localhost:1880/ui


# Estrutura do Repositório
pawcare-monitor/
├── sketch.ino       # Código do ESP32
├── diagram.json     # Circuito para simulação no Wokwi
├── libraries.txt    # Bibliotecas necessárias (Wokwi)
├── flows.json       # Flow completo do Node-RED
└── README.md        # Documentação do projeto

# Resultados Parciais

Circuito simulado com sucesso no Wokwi
Leitura de temperatura e umidade via DHT22
Detecção de movimento via sensor PIR
Publicação dos dados via protocolo MQTT
Dashboard Node-RED com gauges, gráfico e alertas automáticos
Integração com plataforma Clyvo Vet (próximas sprints)


# Integrantes
Nathan G.P. Mendes - Rm564666
Gustavo Araujo Da Silva - Rm566526
Guilherme Santos Fonseca - Rm564232
Anthony De Sousa Henrique - Rm566188

# Licença
Projeto acadêmico desenvolvido para o FIAP Challenge 2026 em parceria com a CLYVO VET.
