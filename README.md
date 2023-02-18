### Programa em OpenGL - Computação Gráfica - Trabalho1: Campo de visão e testes de colisões

## Autores: Eduardo Machado Martins & Nathan dos Reis Ramos de Mello

# Introdução ao trabalho:
Neste trabalhos, foi desenvolvido um programa para testar a inclusão de pontos em um triângulo definido como a representação de um campo de visão, a partir de três algorítmos diferentes, sendo eles o de força bruta, o de envelope e o da quadtree. Para a criação desse projeto, foi utilizado um código base fornecido pelo professor Marcio Sarroglia Pinho.

# Compilação e Execução:
A principal dependencia para esse programa é possuir o OpenGL instalado. Além disso, a aplicação foi desenvolvida para funcionar em um sistema operacional Linux. Com isso, primiramente é necessário dar o comando 'make', no diretório do arquivo Makefile. Após a compilação, para executar o código, basta usar no terminal o comando './exe'.

# Introdução à interface:
Este programa foi desenvolvido para ser intuitivo e fácil de operar. Após a execução, uma pequena interface é impressa no terminal, indicando o titulo e nome dos autores. Logo abaixo, nota-se uma sequência de comandos que podem ser utilizados durante a execução, para controlar o programa, conforme abaixo:
    - Tecla 'Esc' para sair do programa;
    - Teclas '+' e '-' para aumentar ou diminuir o campo de visão;
    - Tecla 'Espaço' para mostrar/ocultar detalhes;
    - Tecla 'c' para ativar/desativar as cores dos pontos;
    - Tecla 'x' para trocar o algoritmo operante;
    - Tecla 'z' para trocar a informação que está sendo apresentada sobre o algoritmo em questão;
    - Tecla 'n' para trocar o valor N de geração da quadtree (N=quantidade minima de pontos para dividir).
Em seguida, com as regras do jogo em mente, a primira coisa que o programa solicita do usuário é um nome de arquivo para gerar os pontos. Nesta etapa, pode-se apertar 'Enter' para gerar pontos aleatórios, neste caso, ainda será solicitado a quantidade de pontos aleatórios para gerar.

# Controle:
Ao carregar a tela, ela mostrará o triangulo que representa o campo de visão, dentro de um cenário com os pontos criados a partir dos dados informados. Ao utilizar as setas do teclado, pode-se andar com o campo de visão sobre o cenário, e utilizando os comandos descritos, pode-se obter as informações desejadas sobre cada algoritmo de otimização.

# Detalhes da solução:
Para deixar a solução mais eficiente, foram utilizadas anotações do tipo pragma ao longo do código, para que através da biblioteca do OpenMP, algumas regiões com iterações mais pesadas fossem paralelizadas. Definiu-se no método 'main' a utilização de 8 threads para a paralelização e a divisão de carga para cada thread foi definida como dinâmica. Nota-se que algumas regiões dentro desses processos foram definidas como regiões críticas para serem executadas de maneira sequncial, e que existe uma variável que ao final das iterações sofre redução. A paralelização foi feita nos métodos: forcaBruta(), envelope() e quad();
Para o algoritmo da quadtree, foi desenvolvido uma estrutura recursiva de grafo percorrido em profundidade, e inicilaiza com 'N=10' e limitada a uma altura máxima de 'altura<7'. Isso ocorre em uma 'struct' chamada de Nodo que possui o necessário para o algoritmo funcionar, dessa forma o método 'GeraQuadTree()' faz a construção da estrutura. A metodologia utilizada, foi guardar em cada folha dela, um vetor que gurada os idexes dos pontos da variável PontosCenario que estão na folha em questão. Assim, após coletar todos os indexes, é possível solucionar casos onde um mesmo ponto é conciderado duas vezes (quando está exatamente na divisão entre duas folhas), pois basta remover os indexes que se repetem no memento das comparações finais.
Outro fator que é váido mencionar foi o ajuste da escala do movimento, através da parametrização do valor de avanço com a variável 'moveScale'. A partir disso, quando a escala do campo de visão é ajustada para pontos extremamente próximos, e movimento de avanço do triangulo é suavizado conforme o tamanho da tela.
Por fim, é valido mencionar que todo o progrma é executado com base nos estados de algumas variáveis de estado, sendo elas: 'colorStatus', 'lineStatus', 'modeStatus' e 'infoStatus'. As demais soluções utilizadas foram baseadas no que foi trabalhado em aula.
Contudo, com a finalização deste trabaho, ficou evidente a importância do estudo da computação grafica e de seus princípios para um profissional da ciência da computação. Por fim, a solução foi conciderada satisfatório, visto o problema apresentado.