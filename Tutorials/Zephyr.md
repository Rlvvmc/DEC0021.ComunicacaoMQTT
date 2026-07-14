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

