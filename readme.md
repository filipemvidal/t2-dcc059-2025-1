# Trabalho 2
**Universidade Federal de Juiz de Fora**
**Departamento de Ciência da Computação**
**DCC059 – Teoria dos Grafos – Turma A**
**Semestre 2025/1 - Profa Luciana Brugiolo - Tutor Rafael Freesz**

## Execução
Para compilar o programa utilize:
~~~
g++ *.cpp -o execGrupo09
~~~

E para executar:
~~~
./execGrupo09 <arquivo_entrada>
~~~
Onde *<arquivo_entrada>* é o nome do arquivo que contém as informações do grafo.

## Interface di programa
Para executar os algorítmos referentes a este trabalho, na tela inicial digite "2". Na tela seguinte, digite "a" para executar o algorítmo guloso, nesse caso o nosso também é adaptativo, digite "b" para executar o algorítmo guloso randomizado adaptativo e "c" para executar o algorítmo guloso randomizado adaptativo reativo.

## Parâmetros
O algorítmo guloso randomizado adaptativo itera apenas uma vez, enquanto o algorítmo guloso randomizado adaptativo reativo itera o número de vezes definido pelo usuário. 

A semente de randomização é o tempo, no momento da execução, em segundos. Ou seja, a cada execução a semente de randomização é diferente e, portanto, os resultados também.

O arquivo de saída vai para a pasta raiz do repositório seguindo o seguinte padrão:
    resultado_[nome do algorítmo].txt