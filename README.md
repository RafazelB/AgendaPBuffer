# Rafael Lopes Bermudes
# AgendaPBuffer

## Objetivo

Criar uma agenda com o seguinte menu:

- 1 - Adicionar pessoa
- 2 - Remover pessoa
- 3 - Buscar pessoa
- 4 - Listar todas as pessoas
- 5 - Sair

O desafio é implementar toda a estrutura usando apenas um buffer de memória (`void *pBuffer`), reproduzindo manualmente a organização de memória que o sistema operacional normalmente faz para o programa.

## Regras

### 1. Não é permitido declarar variáveis no programa, apenas ponteiros

Todos os dados do programa devem ser armazenados dentro do `pBuffer`.

- 1.1 Nem mesmo como parâmetro de função. Só ponteiros que apontam para dentro do `pBuffer`.
- 1.2 Exemplo permitido: `int *ptr = (int *)pBuffer`
- 1.3 Exemplo não permitido: `int c;`, `char a;`, `int v[10];`, `void Funcao(int parametro)`
- 1.4 Todas as strings que ficarem dentro do `pBuffer` deve ter somente o tamanho necessário, somente um buffer para receber o que o usuário digita no scanf pode ter um tamanho fixo.

### 2. Não pode usar `struct` em todo o programa.

## Motivação

O exercício tem como objetivo praticar alocação dinâmica, organização manual de memória e compreensão mais profunda do hardware onde programamos.
