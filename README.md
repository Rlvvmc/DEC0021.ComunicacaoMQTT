# DEC0021 - Trabalho comunicação entre ESP32, React e Agentes Jason via MQTT

## Introdução
Este projeto foi desenvolvido para a materia de Projeto de Sistemas Ubiquos e Embarcados na UFSC. O trabalho possui o intuito de permitir a comunicação simples entre sistemas compostos de microcontroladores, agentes inteligentes e frontends ou UIs para um usuario. Em sistemas distribuidos, este tipo de comunicação é desenvolvida com dois pontos especificos como foco(Por exemplo, uma interface escrita em Javascript que se comunica com um agente escrito em Jason), ou "point-to-point", o que leva estas implementações a serem fortemente acopladas aos pontos escolhidos, uma dependencia das estruturas e frameworks utilizadas pelos pontos escolhidos, de tal forma que torna dificil a adição de novos pontos ou substituição de tecnologias. Com este problema em mente, o projeto visa utilizar MQTT como meio para troca de mensagens, onde estas utilizam um modelo padrão que todos os pontos entendem.

Para isto, são utilizados microcontroladores ESP32 programados com base no sistema operacional Zephyr, uma interface de usuario programada  React, Agentes Jason configurados através de JaCaMo, Servidor programado em Node e utilizando o broker de mensagens Eclipse Mosquitto.

## Tabela de Conteudos

- [Hardware Utilizado](#hardware-utilizado)
- [Instalação](#instalação)
   1. [Mosquitto](#eclipse-mosquitto)
   2. [JaCaMo](#jacamo)
   3. [Zephyr](#zephyr)
- [Configuração do Hardware](#configuração-do-hardware)
- [Exemplo de Funcionamento](#exemplo-de-funcionamento)
- [Diagnostico de Problemas e Soluções](#diagnostico-de-problemas-e-soluções)

---
## Hardware Utilizado
#### - Esp32
O microcontrolador escolhido para o projeto. O chip desta placa em especifico é o ESP32-D0WD-V3, com funcionalidade Wi-Fi e Dual Core + LP Core. 

#### - Sensor HC-SR04
Utilizado para gerar uma medida de distancia que é transmitida periodicamente.

#### - Led
Utilizado como mecanismo de verificação de comunicação entre o frontend e o microcontrolador.

### Esquema de Conexões
![Conexões do circuito](Images/ExemploCircuito.png)

---
## Instalação
As instalações mencionadas nesse topico são realizada para o Windows 10.
### Eclipse Mosquitto
1. Baixar e utilizar instalador do Eclipse Mosquitto do site oficial: https://mosquitto.org/download/
> Não habilite a opção de executar Mosquitto como serviço do Windows. Para este projeto, o broker será iniciado manualmente com uma configuração especifica.
2. Adicionar a pasta de instalação do Mosquitto para variavel "PATH" nas variaveis de ambiente do sistema.
> Isso e necessario para que possa ser utilizado em qualquer pasta do sistema. Para acessar as variaveis do sistema, vá para o painel de controle -> Sistema e Segurança -> Sistema -> Configurações avançadas do Sistema -> Variaveis do Ambiente.
3. Criar um arquivo de configuração `myConf.conf` (Você utilizar qualquer nome). Este arquivo será utilizado posteriormente quando iniciarmos o broker, por isos é incluido na pasta do projeto. Estes são os parametros que devem ser escritos neste arquivo:
```
listener 1883
protocol mqtt

listener 9001
protocol websockets

allow_anonymous true
```
Estas configurações permitem que qualquer dispositive se conecte ao broker, e permite que o navegador se conecte atraves de websockets. Estas configurações devem ser utilizadas apenas para debugging.
### JaCaMo
Referencia para instalação: https://jacamo-lang.github.io/getting-started
Pre-requisitos:
- Java versão 21 ou acima (SDKs de varias versões podem ser adquiridos neste site: https://openjdk.org/)
- ! A pasta onde o SDK for instalado deve ser incluida na variavel PATH das variaveis do ambiente.
> Dependendo da versão atual do JaCaMo, pode ser necessario utilizar um SDK especifico.

1. Baixar: http://jacamo-lang.github.io/jacamo/nps/np1.3.zip
2. Descompactar
3. Executar gradlew.bat. Esset executavel vai então pedir um nome do projeto, coloque um nome de sua escolha.
4. por linha de comando, mude para a pasat com o nome que escolheu e inicie o projeto:
```
cd .../meuProjetoJaCaMo/
gradlew run
```
> "./gradlew" é o comando para Linux. Para Windows, utiliza-se gradlew run.

Isto deve criar um agente com padrão "Hello World". 
### Zephyr
Referencia: https://docs.zephyrproject.org/latest/develop/getting_started/index.html
Zephyr e um sistema operacional embarcado, e a programação, build e flash dele são realizadas em linux. Para utiliza-lo no Windows, usaremos um substema Linux para Windows (WSL).
1. Abrir o Windows Powershell como administrador.
2. Execute:
```
wsl --install
```
3. Abra um painel de linha de comando (CMD), e escreva wsl. Isso irá iniciar o subsistema. Na primeira inicialização, terá que definir um nome de usuario e senha. Guarde esta senha, pois não há como recupera-la.
4. Mude para a pasta raiz do sistema Linux, usando `cd` (sem nenhum diretorio)
5. Atualiza o subsistema:
```
sudo apt update
sudo apt upgrade
```
6. Instale as dependencias do Zephyr:
```
sudo apt install git python3 python3-pip python3-venv cmake ninja-build gperf ccache dfu-util device-tree-compiler wget xz-utils file make gcc gcc-multilib g++-multilib
```
7. Crie um ambiente virtual para seu projeto e o ative:
```
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
```
Lembre-se de guardar o path para este arquivo de ambiente virtual, será necessario executar o comando source sempre que for realizar build ou flash de projetos que utilizam Zephyr.
8. Instale west:
```
pip install west
```
9. Instale o codigo fonte do Zephyr (Isto cria um workspace e clona e repositorio do Zephyr)
```
west init -m https://github.com/zephyrproject-rtos/zephyr ~/zephyrproject
cd ~/zephyrproject
west update
```
10. Exporte um pacote de CMake:
```
west zephyr-export
```
11. Instale as dependencias de Python:
```
west packages pip --install
```
12. Instale o SDK:
```
cd ~/zephyrproject/zephyr
west sdk install
```
13. Realize o Build do codigo Hello world:
```
cd ~/zephyrproject/zephyr
west build -p always -b <your-board-name> samples/hello_world
```
> `<your-board-name>` seria o modelo de sua placa. A placa que foi utilizada neste projeto utiliza `esp32_devkitc/esp32/procpu` como modelo. Cada versão de esp32 utiliza modelos diferentes. Caso este modelo não seja compativel com sua placa, podem ser visualizados todos os modelos suportados pelo Zephyr com o comando: `west boards | grep esp32`, ou se não estiver utilizando esp32, `west boards` para todas as placas compativeis com este SO.


- Antes de prosseguir ao flash, precisamos mapear a porta serial do windows ao subsistema linux. Para isso, podemos utilizar o `usbipd`. Para instalar essa ferramenta, baixe e instale a versão mais nova em https://github.com/dorssel/usbipd-win. Depois, execute o Windows Powershell como administrador e, com o esp32 conectado a seu computador, execute:
```
usbipd list
```
Isso irá mostrar uma lista de portas seriais sendo utilizadas. Você deve localizar algo escrito como:
```
BUSID   VID:PID    DEVICE
1-4     10C4:EA60  CP210x USB to UART Bridge
```
Com este busid, e com o WSL ainda rodando, utilize o comando:
```
usbipd attach --wsl --busid X-Y
```
onde `X-Y`é o numero de busid que aparecer pro seu computador.
Com isso feito, agora no WSL, use o comando:
```
ls /dev/ttyUSB*
```
ou
```
ls /dev/ttyACM*
```
Se um destes obter retorno, a porta serial estara configurada corretamente e poderá prosseguir com o Flash.

14. Realize o Flash:
```
west flash
```
15. Visualizar prints do Zephyr
16. 
Precisaremos de um monitor serial. O que escolhi foi o `Tio`, podemos instalar este com:
```
sudo apt install tio
```
e utilizar com:
```
tio -b 115200 /dev/ttyUSB0
```
Dependendo do codigo sendo executado pelo esp32, pode ser necessario pressionar o botão de reset fisico na placa para que obtenhamos algo legivel.


---
## Configuração do Hardware
As configurações de pinos estão definidas em `./boards/esp32_devkitc_procpu.overlay` na pasta de projeto do zephyr, definidas como:

| Pino | Alias | Funcionalidade|
|------|------|--------|
| `GPIO23` | led0 | Acionamento do led. |
| `GPIO18` | distance0 | Pino de Trigger do sensor HC-SR04. |
| `GPIO19` | distance0 | Pino de Echo do sensor HC-SR04. |

> Zephyr utiliza um conceito de "Alias" ou "Node" para definir o acesso aos pinos de hardware. Os pinos são associados a uma "Alias", e então podem ser referenciados no codigo. Para os pinos de Trigger e Echo do HC-SR04, é utilizado uma mesma Alias, sendo que o seu acesso e feito atraves de uma função interna do Zephyr.

---
## Exemplo de Funcionamento
O sistema é baseado em MQTT, logo, os integrantes do sistema, ESP32, Agentes e a Interface se comunicam publicando informações para topicos especificos, e todos os integrantes que estiverem subscritos nestes topicos recebem estas informações. Cada integrante neste projeto possui um topico `/pub` pro qual publicam: `ui/pub`,`agent/pub` e `mcu/pub`. Um integrante está subscrito nos topicos `/pub` dos outros dois integrantes. Isto, em pratica, permite que cada integrante veja todas as mensagens dos outros integrantes.
Os dados transmitidos no sistema são emitidos em um formato JSON padrão que todos os integrantes utilizam:
```
{id: ..., sender: ..., text: ....}
```
cada integrante deve tratar dados recebidos por este formato para poder utiliza-lo.
Exemplos de comunicação:
1. Envio de dados de sensores do ESP32 aos Agentes e Interfaces
o Esp está programado para periodicamente enviar a leitura do sensor de distancia:

![Conexões do circuito](Images/EnvioDeDados.png)
3. Envio do comando "ledOn" para o ESP32
A interface(ou agentes) podem publicar a mensagem "ledOn". O ESP32 ve esta mensagem e interpreta como o comando de ligar o led. Da mesma maneira, interpreta "ledOff" como o comando de desligar o led.

![Conexões do circuito](Images/RecebimentoDeDados.png)

## Diagnostico de Problemas e Soluções
- Conexão ao broker MQTT
Inicialmente, o ESP32 não conseguia se conectar ou broker MQTT que estava em exeução na maquina principal. Foi verificado que qualquer outra maquina conseguia se conectar, e o problema se tratava de uma configuração na maioria dos roteadores que bloqueia qualquer aparelho não identificado de se conectar por LAN a outro aparelho. Para resolver este problema, foi utilizado o hotspot do celular como rede, que não possui essa configuração de bloqueio de acesso a LAN. Outra solução seria definir um IP fixo ao esp32, mas isso não erá uma solução viavel para este projeto.
- Leitura de mensagens JSON no Zephyr
O formato padrão decidido para o sistema foi de um JSON. Zephyr possui metodos para extrair dados de um formato JSON especificado, mas não foi obtido sucesso em utiliza-lo. Logo, foi desenvolvido a função `get_text_field`, que procura a substring de `\"text\":` dentro da mensagem em JSON, e extrai o texto entre aspas duplas, guardando este em um buffer que é então usado pelo sistema.
