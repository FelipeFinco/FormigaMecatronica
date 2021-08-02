# FormigaMecatronica
Projeto final da matéria Sistemas Embarcados do primeiro semestre de 2021. Matéria ministrada pelo *Professor Doutor Glauco Augusto de Paula Caurin* do Departamento de Engenharia Aeronáutica e pelo *Professor Daniel Varela Magalhães* do Departamento de Engenharia Mecânica, ambos da Escola de Engenharia de São Carlos/Universidade de São Paulo. <br/>

<img src="./img/logo_eesc.png" align="right"
     alt="Logo EESC" height="200">


## Alunos/Desenvolvedores

* **[Felipe Catuta Pécora Finco](https://github.com/FelipeFinco)** *n° USP: 4822582* - *EESC/USP*
* **[Lucas Cortes Oliveira](https://github.com/colollucas)** *n° USP: 10309170* - *EESC/USP*
* **[Felipe Avelar Vaz de Oliveira](https://github.com/Felipe-Avelar-vaz)** *n° USP: 10308798* - *EESC/USP*


# Introdução

 Dentro da robótica robos hexapodes possuem uma ampla gama de utilização devido a facilidade de locomoção em terrenos acidentados, sendo utilizados principalmente em meios agricolas, mas até mesmo em espaciais.

 Este trabalho consiste na implementação da lógica de movimentação do robô em um sistema de hardware já existente, o qual consiste em uma placa Toradex (com computador Colibri VF50) que possui grande robustez, escalabilidade e compatibilidade com o Linux, duas MBEDS (com controlador LPC1768) para complementação devido seus recursos open source e modularização, além dos 12 motores (2 por perna) do robô, caracterizando um sistema embarcado robusto, com grande facilidade de atualização e desenvolvimento. Assim, o trabalho passou pelas etapas de concepção da lógica de movimentação e integração dos motores, criação dos códigos, comunicação CAN entre as placas e implementação a distância do software no hardware descrito acima.
 
# Proposta/objetivo
 De maneira geral, deseja-se que o robô hexapode, aqui chamado de "formiga robótica", possa se movimentar nos dois sentidos em apenas uma direção utilizando-se o protocolo de comunicação CAN entre as placas devido a sua robustez, buscou-se ainda uma comunicação entre as placas visado diminuir o trafégo de informações e possibilitar implementações no futuro além de dividir a tarefa de processamento entre a MBED e a toradex.

 Para tal, buscou-se caracterizar o movimento dos motores pelo menor número de variaveis através da integração do movimento das pernas, uma vez que três pernas se moverão juntas enquanto outras três permanecerão em repouso. Toda esta lógica de movimentação foi feita na Toradex para que poucas mensagens fossem passadas para a MBED e assim esta conseguisse processá-las e criar o sinal de movimentação do motor. 

# Desenvolvimento
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
     
Utilizou-se ainda do protocolo de comunicação CAN devido sua robustez, facilidade de se adaptar a condições de falha e facil debug
# Conclusão/Resultados
