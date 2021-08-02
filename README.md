# FormigaMecatronica
Projeto final da matéria Sistemas Embarcados do primeiro semestre de 2021. Matéria ministrada pelo *Professor Doutor Glauco Augusto de Paula Caurin* do Departamento de Engenharia Aeronáutica e pelo *Professor Daniel Varela Magalhães* do Departamento de Engenharia Mecânica, ambos da Escola de Engenharia de São Carlos/Universidade de São Paulo. <br/>

<img src="./img/logo_eesc.png" align="right"
     alt="Logo EESC" height="200">


## Alunos/Desenvolvedores

* **[Felipe Catuta Pécora Finco](https://github.com/FelipeFinco)** *n° USP: 4822582* - *EESC/USP*
* **[Lucas Cortes Oliveira](https://github.com/colollucas)** *n° USP: 10309170* - *EESC/USP*
* **[Felipe Avelar Vaz de Oliveira](https://github.com/Felipe-Avelar-vaz)** *n° USP: 10308798* - *EESC/USP*


# Introdução

 Dentro da robótica, robôs hexapodes possuem uma ampla gama de utilização devido a sua habilidade de locomoção em terrenos acidentados, sendo utilizados principalmente em meios agrícolas, sendo empregado até mesmo em meios espaciais.

 Este trabalho consiste na implementação da lógica de movimentação do robô em um sistema de hardware já existente, o qual consiste em uma placa da Toradex (Colibri VF50 embarcada na Viola Carrier Board) que possui grande robustez, escalabilidade e compatibilidade com o Linux e duas MBEDs (com controlador LPC1768) para complementação devido seus recursos open source e modularização. A partir da saída PWM é possível controlar os 12 servo motores (2 servos por perna). Assim, pretende-se ao final do trabalho criar a lógica de movimento do robô na Toradex, esta enviando comandos para ambas MBEDs que por sua vez devem estar aptas a interpretar esse comando e comandar os seus respectivos motores.
 
# Proposta/objetivo
 Sabendo-se o contexto do projeto, devemos fechar seu escopo qual será a abordagem para os problemas expostos. Como imagina-se que o robô hexapode, aqui chamado de "formiga robótica", terá outras funcionalidades, pensaremos a estruturação do robô para que tanto o código, quanto o protocolo de comunicação seja versátil a adaptações e aprimoramentos.

 Assim buscamos fazer um software bem estruturado e comentado em inglês pensando que no futuro outras pessoas poderiam acessá-lo. Para abordar os protocolos de comunicação, usa-se PWM para informar as posições dos servos como padrão e escolhemos o protocolo CAN para realizar a comunicação entre os processadores. Este protocolo foi selecionado apesar de não ser tão facilmente implementável pois é um protocolo que possui uma robustez muito grande, além de ser determinístico e permitir que eventualmente outros dispositivos sejam "pendurados" na mesma rede. Como imagina-se o uso desse protocolo para outros sistemas, buscamos fazer a comunicação o mais eficiente possível e não necessáriamente da forma mais simples.

 Para tal, buscou-se caracterizar o movimento dos motores pelo menor número de variaveis através da integração do movimento das pernas, uma vez que imagina-se, para movimentos simples, o motor vertical e horizontal da mesma perna atrelados entre si, dá para reduzir o número de graus de liberdade. Toda esta lógica de movimentação foi feita na Toradex para que poucas mensagens fossem passadas para a MBED, sendo cargo desta processá-las e criar por conta o sinal de movimentação do motor. 

# Desenvolvimento
## Desenvolvimento da lógica
Tendo em vista a proposta do protótipo o movimento de formigas foi observado afim de formular uma lei de integração entre as pernas. Notou-se que as formigas executam simultaneamente movimentos rotacionais de subida e descida nas patas 1, 3 e 5 de forma a manter um plano de apoio no chão com as patas 2, 4 e 6 (Figura 1) a partir do momento em que as patas impares tocam o chão, o movimento se da nas patas pares. Logo podemos descrever o movimento de cada perna pela suas componentes longitudinais e verticais, para diminuir o número de mensagens na comunicação uma função integradora entre os movimentos das pernas foi criada possbilitando que os variaveis verticais e horizontais do movimento fossem substituidas apenas pela grandeza de fase do gráfico, diminuindo pela metade o número de variáveis como mostra o gráfico 1. Além disto as tarefas de processamento da MBED foram reduzidas uma vez que três pernas executarão mesmo movimento
<img src="./img/numeros_pernas.png" align="center"
     alt="Figura 1" height="200">

<img src="./img/movimentos.png" align="center"
     alt="Figura 2" height="200">
  
Tendo em vista tal lógica de movimentação, havia a possibilidade de executar todo o processamento apenas na MBED, deixando a placa Toradex apenas para recebimento de input do usuário, porém este método se torna desvantajoso pois sobrecarrega a rede de comunicação não permitindo implementações futuras além de facilitar ocorrência de erros e dificultar o debug. Portanto, escolheu-se executar a logica de movimentação toda na toradex enviando apenas duas mensagens (Fase e sentido) para a MBED, a qual processará tais informações afim de criar o PWM do motor como mostra o diagrama abaixo.

<img src="./img/Diagrama.jpg" align="center"
     alt="Figura 3" height="200">

Para essa implementação, portanto, foi utilizada a placa Colibri VF50, um computador em módulo com CPU Cortex A5 de limite de processamento de 400MHz, que se destaca principalmente pelo seu custo benefício, por ter diversas capabilidades que possibilitam uma ampla gama de aplicações, incluindo equipamentos de IHM e dispositivos industriais, equipamentos de segurança, infraestrutura e manufatura, aplicações de conversão de energia como drivers e inversores de frequência, dispositivos robustos de conectividade e sistemas operados por bateria como robôs e veículos industriais.

<img src="./img/colibri-vf50-front-view.png" align="center"
     alt="Figura 3" height="200">
     
Esse computador está embarcado em uma placa de suporte chamada Viola Carrier Board, uma single-board computer (SBC) de 74x74mm, que apresenta suporte a diferentes interfaces utilizadas no mercado, sendo importante para a comunicação CAN que será utilizada no projeto.

<img src="./img/viola-carrier-board-top-view.jpg" align="center"
     alt="Figura 4" height="200">
     
Para enviar os sinais em PWM, foram usadas as MBED da LPC 1768, que pode ser vista abaixo.

<img src="./img/mbed_LPC1768.jpg" align="center"
     alt="Figura 4" height="200"> <img src="./img/lpc1768_pinout.png" align="center"
     alt="Figura 4" height="200">    
     
Utilizou-se ainda do protocolo de comunicação CAN devido sua robustez, facilidade de se adaptar a condições de falha e fácil debugging.

Nos dois códigos, para facilitar a comunicação entre os módulos, utilizou-se de uma máquina de estados para tornar mais clara a definição da comunicação entre a Toradex e a MBED. Os diagramas da Toradex e da MBED estão expostos abaixo. 

Na Toradex, temos 3 estados: "SENDING_COMMAND", "WAITING_MOVEMENT", "EXIT", que são afetadas pela interação com o usuário, a comunicação com a MBED e o movimento da respectiva perna, havendo esse estado relativo à finalização do processo.  

<img src="./img/States diagrams.png" align="center"
     alt="Figura 4" height="200"> 
     

 Na MBED, mais simples, temos dois estados: "WAITING_COMMAND", "SENDING_MOVEMENT"   
     
<img src="./img/Mbed States diagrams (1).png" align="center"
     alt="Figura 4" height="200">   

## Desenvolvimento do código
## Desenvolvimento da comunicação

# Conclusão/Resultados


