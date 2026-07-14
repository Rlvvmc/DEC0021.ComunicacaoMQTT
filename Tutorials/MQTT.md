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
