# Guia de Implementação

## rm

O código para achar o arquivo será identico ao do mv/cat.

Após achar um arquivo, para remove-lo, deve-se:

1. Achar sua entrada(s) na FAT e seta-las como NULL
    * Para arquivos com mais de um cluster, será necessário seguir os ponteiros da FAT. Exemplo de como fazer em código na função `cat()`.
2. Setar o primeiro caractér do nome do arquivo na como DIR\_FREE\_ENTRY.

Ambas operações irão desalocar o arquivo, deletando-o.

## cp

O código para achar o arquivo será identico ao do mv/cat.

Após achar um arquivo, para copia-lo, deve-se;

1. Criar uma nova entrada de diretório, copiando a original.
    * O nome será diferente, veja o código do `mv()` como exemplo.
2. Copiar todos os clusters do arquivo.
    * Para isso, será necessário seguir os ponteiros do FAT. Veja `cat()`.
    * Para cada cluster, será necessário alocar uma entrada na FAT. Para fazer tal, simplimente ache uma entrada nula, e marque à como o fim do arquivo (`0xFFFF`).
    * Se seguida, vá nos clusters reais, na região de dados, e copie-los do original para o novo.
    * Caso seja necessário mais clusters para copiar o arquivo inteiro, ache uma nova entrada FAT nula, aloque-a do mesmo jeito que a anterior, e reescreva a anterio com o ponteiro desta. O resultado será uma lista encadeada.

## Dificuldade

O `rm()` é mais facil de implementar que o `cp()`.
