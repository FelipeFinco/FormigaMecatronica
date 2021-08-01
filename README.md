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

 Este trabalho consiste na implementação da lógica de movimentação do robô em um sistema de hardware já existente, o qual consiste em uma placa Toradex que possui grande robustez, escalabilidade e compatibilidade com o Linux, duas MBEDS para complementação devido seus recursos open source e modularização, além dos 12 motores (2 por perna) do robô, caracterizando um sistema embarcado robusto, com grande facilidade de atualização e desenvolvimento. Assim, o trabalho passou pelas etapas de concepção da lógica de movimentação e integração dos motores, criação dos códigos, comunicação CAN entre as placas e implementação a distância do software no hardware descrito acima.
 
# Proposta/objetivo
 De maneira geral, deseja-se que o robô hexapode, aqui chamado de "formiga robótica", possa se movimentar nos dois sentidos em apenas uma direção utilizando-se da comunicação mais simples possível entre a toradex e ambas MBED buscando ainda a divisão de processamento entre elas.

 Para tal, buscou-se caracterizar o movimento de todos os motores apenas por duas variáveis - fase e sentido - através da integração do movimento das pernas, uma vez que três pernas se moverão juntas enquanto outras três permanecerão em repouso. Toda esta lógica de movimentação foi feita na Toradex para que apenas duas variaveis fossem passadas para a MBED e assim esta conseguisse processá-las e criar o sinal de movimentação do motor. 

# Desenvolvimento
Tendo em vista a proposta do protótipo o movimento de formigas foi observado afim de formular uma lei de integração entre as pernas. Notou-se que as formigas executam simultaneamente movimentos rotacionais de subida e descida nas patas 1, 3 e 5 de forma a manter um plano de apoio no chão com as patas 2, 4 e 6 (Figura 1) a partir do momento em que as patas impares tocam o chão, o movimento se da nas patas pares, logo, a partir da fase e sentido do movimento de uma pata é possível determinar o da outras 5 como mostra o gráfico abaixo.
<img src="./img/numeros_pernas.png" align="center"
     alt="Figura 1" height="200">

Além disso, pode-se visualizar a diferença entre os movimentos horizontal e vertical no gráfico abaixo, considerando que em cada intervalo de 90° há uma descrição do movimento de uma pata, considerando que uma está em fase com a outra.

<img src="./img/movimentos.png" align="center"
     alt="Figura 2" height="200">
     
Com essa lógica foi possível utilizar um input do usuário para a toradex e assim mandar apenas duas informações por motor para cada MBED.

Para essa implementação, portanto, foi utilizada a placa Colibri VF50, um computador em módulo com CPU Cortex A5 de limite de processamento de 400MHz, que se destaca principalmente pelo seu custo benefício, por ter diversas capabilidades que possibilitam uma ampla gama de aplicações, incluindo equipamentos de IHM e dispositivos industriais, equipamentos de segurança, infraestrutura e manufatura, aplicações de conversão de energia como drivers e inversores de frequência, dispositivos robustos de conectividade e sistemas operados por bateria como robôs e veículos industriais.

<img src="./img/colibri-vf50-front-view.png" align="center"
     alt="Figura 3" height="200">
     
Esse computador está embarcado em uma placa de suporte chamada Viola Carrier Board, uma single-board computer (SBC) de 74x74mm, que apresenta suporte a diferentes interfaces utilizadas no mercado, sendo importante para a comunicação CAN que será utilizada no projeto.

<img src="./img/viola-carrier-board-top-view.png" align="center"
     alt="Figura 4" height="200">
     
# Conclusão/Resultados
