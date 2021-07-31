# FormigaMecatronica
Projeto final da matéria Sistemas Embarcados do primeiro semestre de 2021. Matéria ministrada pelo *Professor Doutor Glauco Augusto de Paula Caurin* do Departamento de Engenharia Aeronáutica e pelo *Professor Daniel Varela Magalhães* do Departamento de Engenharia Mecânica, ambos da Escola de Engenharia de São Carlos/Universidade de São Paulo. <br/>

<img src="./img/logo_eesc.png" align="right"
     alt="Logo EESC" height="200">


## Alunos/Desenvolvedores

* **[Felipe Catuta Pécora Finco](https://github.com/FelipeFinco)** *n° USP: 4822582* - *EESC/USP*
* **[Lucas Cortes Oliveira](https://github.com/colollucas)** *n° USP: 10309170* - *EESC/USP*
* **[Felipe Avelar Vaz de Oliveira](https://github.com/Felipe-Avelar-vaz)** *n° USP: 10308798* - *EESC/USP*


# Introdução

TESTE Dentro da aeronautica, a determinação e controle de atitude (Attitude Detemination and Control - ADC) de uma aeronave é de fundamental importância para a existência do voo. Com a crescente utilização de Veículos Aéreos Não Tripulados (VANT's) para as mais variadas aplicações, iniciou-se um processo de barateamento da tecnologia de ADC, visando disseminar ainda mais a utilização desses veículos e tornar possível aplicações que não tenham capacidade de prover grandes investimentos. 


Este trabalho consiste na implementação de um sistema de fusão sensorial simplificado, o qual permite aquisitar sinais de uma unidade de medida inercial (*Inertial Measurement Unit - IMU*), calcular a atitude desse sensor - módulo que estaria presente na aeronave - e enviar esses sinais para um computador de monitoramento (*Ground Station*). Para isso, será utilizada uma BeagleBone Blue como dispositivo embarcado, uma placa de desenvolvimento com inúmeros recursos voltados para robótica (como uma *IMU*) além de um computador de propósito geral, que servirá como *Ground Station*. Na *Ground Station*, os dados de atitude serão visualizados através de um modelo 3D de uma aeronave, a qual seguirá a atitude da placa de desenvolvimento, assim como seria caso a placa estivesse presente em um *VANT*, além de exibir os gráficos de rolagem, arfagem e guinada em função do tempo. A parte embarcada do sistema é fundamental para o voo de qualquer aeronave autônoma, já a parte desenvolvida para a *Ground Station*, imagina-se como exemplo de utilização no desenvolvimento de aeronaves de pequeno porte, para monitorar as variáveis principais de atitude, conseguindo avaliar o funcionamento do modelo assim como o desempenho do projeto em manobras. 


# Descrição do sistema

De maneira geral, deseja-se sempre monitorar o estado de todas as variáveis possíveis de uma aeronave, principalmente em seu estado de desenvolvimento. Em uma aplicação real, o *link* entre a aeronave e a *Ground Station* não compõe a lista de funcionalidades mais relevantes para operação do sistema. No entanto, para fins didáticos e para que os conceitos desenvolvidos na disciplina pudessem ser aplicados sem complicações adicionais, propôs-se desenvolver algumas partes desse *link* apenas para as variáveis de atitude, de modo a iniciar uma arquitetura de funcionamento para esse sistema embarcado proposto e o desenvolvimento nessa área. Para estudantes futuros que tenham interesse em dar continuidade ao projeto, inúmeras possibilidades de integrações e desenvolvimentos futuros poderiam trazer aplicabilidade real ao sistema.

O *link* discutido envolve um conjunto de operações bastante complexas. Considerando o modelo *OSI*, tomando, por exemplo, a camada física da comunicação, inúmeros desafios já estariam presentes no desenvolvimento, como a determinação do nível de potência do sinal transmitido, tipo de modulação, antena e seus ganhos, apontamento e afins. O sistema proposto foca em desenvolver: 
* A aquisição de dados, considerando acesso direto ao barramento no qual o sensor inercial está presente
* O processamento deles e a transformação dos valores em ângulos de *Euler*/*Tait-Bryan*
* Envio, via *socket*, de um pacote de informação contendo os ângulos 

Vale ressaltar que em uma aplicação real, o pacote conteria redundâncias, checagem e correção de erros, além de mais informação que só os ângulos de atitude. Como a ideia neste trabalho foi criar familiaridade com o desenvolvimento de sistemas embarcados por parte dos membros, apenas conceitos mais triviais foram levados em conta. 
