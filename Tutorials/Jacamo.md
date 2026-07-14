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
