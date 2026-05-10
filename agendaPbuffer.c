#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_INICIAL (sizeof(int) * 4 + sizeof(char) * 100)

int main() {
    void *pBuffer = malloc(sizeof(int) * 4 + sizeof(char) * 100);
    // Buffer vai ser [ Menu | Contador1 | Contador2 | Tamanho Total do Buffer | Buffer para ler Strings | Pessoa1 | Pessoa2 | ... ]
    // Cada pessoa vai ter [ TamanhodaStringdoNome | Nome | Idade | TamanhodaStringdoEmail | Email ]
    if ( pBuffer == NULL ) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    *(int *)pBuffer = 0; // define o menu pra 0
    *((int *)pBuffer + 1) = 0; // define Contador1 pra 0
    *((int *)pBuffer + 2) = 0; // define Contador2 para 0
    *((int *)pBuffer + 3) = 4 * sizeof(int) + sizeof(char) * 100; // define Tamanho Total do Buffer para 116 bytes (4 ints + 100 chars)

    for ( ;; ) {
        printf("Menu:\n\t1. Adicionar Pessoa\n\t2. Remover Pessoa\n\t3. Buscar Pessoa\n\t4. Listar Todos\n\t5. Sair do Programa.\nO que deseja fazer? (1-5) ");
        scanf("%d", &(*(int *)pBuffer));
        getchar();
        switch( *(int *)pBuffer ) {
            case 1: { // Add
                printf("Digite o nome da pessoa a ser adicionada: ");
                fgets( (char *)((char *)pBuffer + sizeof(int) * 4 ), 100, stdin ); // Bota o nome da pessoa no buffer

                printf("Digite o email da nova pessoa: ");
                fgets( (char *)((char *)pBuffer + sizeof(int) * 4 + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1 ), 100 - strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) - 1, stdin ); // Bota o email da pessoa no buffer após o nome

                void *temp = realloc( pBuffer, *(int *)((char *)pBuffer + sizeof(int) * 3) + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1 + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1 ) ) + 1 + sizeof(int) * 3 ); 
                // realoca memória baseado no tamanho do email e do nome + os terminadores + idade + 2 int pra guardar o tamanho das strings
                if ( temp == NULL ) {
                    printf("Erro na alocação de memória.\n");
                    exit(1);
                }
                pBuffer = temp;

                *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ) = strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1;
                memmove( (char *)((char *)pBuffer + *((int *)pBuffer + 3) + sizeof(int) ), (char *)((char *)pBuffer + sizeof(int) * 4 ), *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ) ); 
                // move o nome até o seu espaço designado
                *((int *)pBuffer + 3) += sizeof(int) + *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ) * sizeof(char);

                printf("Digite a idade da nova pessoa: ");
                scanf("%d", (int *)((char *)pBuffer + *((int *)pBuffer + 3) ) ); // bota a idade após o nome
                getchar(); // tira o espaço do scanf para não atrapalhar o input no menu
                *((int *)pBuffer + 3) += sizeof(int);
                // atualiza o tamanho em bytes do buffer

                *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ) = strlen( (char *)((char *)pBuffer + sizeof(int) * 4 + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1 ) ) + 1;
                memmove( (char *)((char *)pBuffer + *((int *)pBuffer + 3) + sizeof(int) ), (char *)((char *)pBuffer + sizeof(int) * 4 + strlen( (char *)((char *)pBuffer + sizeof(int) * 4 ) ) + 1), *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ) ); 
                // move o email até o seu espaço designado

                *((int *)pBuffer + 3) += sizeof(int) + *(int *)( (char *)pBuffer + *(int *)( (char *)pBuffer + sizeof(int) * 3 ) ); 
                // atualiza o tamanho em bytes do buffer
                *(char *)((char *)pBuffer + sizeof(int) * 4 ) = '\0';
                // bota um terminador no inicio do buffer de leitura

                break;
            }
            case 2: // Remove 
                // Verifica se a Agenda possui alguma pessoa a ser removida
                if (  *((int *)pBuffer + 3) == TAM_INICIAL ) {
                    printf("Agenda vazia. \n");
                    break;
                } 

                // pega o nome da pessoa para apagar
                printf("Digite o nome da pessoa a ser removida: ");
                fgets( (char *)((char *)pBuffer + sizeof(int) * 4 ), 100, stdin );

                *((int *)pBuffer + 2) = TAM_INICIAL;

                // percorre todo buffer ate achar a pessoa
                while ( *((int *)pBuffer + 2) != *((int *)pBuffer + 3) ) {
                    if ( strcmp( (char *)( (char *)pBuffer + sizeof(int) * 4 ), (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) ) == 0 ) {
                        // guarda o valor de qual numero tem que somar para chegar ate o endereço da pessoa removida
                        *((int *)pBuffer + 1) = *((int *)pBuffer + 2);
                        // passa para a próxima 
                        *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                        *((int *)pBuffer + 2) += sizeof(int);
                        *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));

                        // pega todos bytes e shifta para esquerda
                        memmove( (char *)((char *)pBuffer + *((int *)pBuffer + 1) ), (char *)((char *)pBuffer + *((int *)pBuffer + 2) ), *((int *)pBuffer + 3) - *((int *)pBuffer + 2) );
                        
                        // faz a conta pra ter o novo tamanho total após apagar a pessoa
                        *((int *)pBuffer + 3) -= ( *((int *)pBuffer + 2) - *((int *)pBuffer + 1) );

                        // realloca espaço pra ficar igual o numero de pessoas atual
                        void *temp = realloc(pBuffer, *((int *)pBuffer + 3) );
                        if ( temp == NULL ) {
                            printf("Erro na alocação de memória.");
                            free(pBuffer);
                            exit(1);
                        }
                        pBuffer = temp;

                        printf("Pessoa encontrada e removida com sucesso.\n");
                        *((int *)pBuffer + 2) = 1;
                        break;
                    }
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                    *((int *)pBuffer + 2) += sizeof(int);
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                }
                if ( *((int *)pBuffer + 2) != 1 ) {
                    printf("Pessoa não encontrada. \n");
                }
                // define os contadores para 0 novamente
                *((int *)pBuffer + 1) = 0;
                *((int *)pBuffer + 2) = 0; 
                // bota um terminador no inicio do buffer de leitura
                *(char *)((char *)pBuffer + sizeof(int) * 4 ) = '\0';
                
                break;
            case 3: // Procurar
                // Verifica se a Agenda possui alguma pessoa a ser procurada
                if (  *((int *)pBuffer + 3) == TAM_INICIAL ) {
                    printf("Agenda vazia. \n");
                    break;
                } 
                // pega o nome da pessoa para procurar
                printf("Digite o nome da pessoa a ser procurada: ");
                fgets( (char *)((char *)pBuffer + sizeof(int) * 4 ), 100, stdin );

                *((int *)pBuffer + 2) = TAM_INICIAL;

                // compara o nome dado com todos nomes do buffer ate achar o certo e parar
                while ( *((int *)pBuffer + 2) != *((int *)pBuffer + 3) ) {
                    if ( strcmp( (char *)( (char *)pBuffer + sizeof(int) * 4 ), (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) ) == 0 ) { 
                        printf("Pessoa encontrada:\n");

                        printf("\tNome: %s", (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) );
                        *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));

                        printf("\tIdade: %d\n", *(int *)((char *)pBuffer + *((int *)pBuffer + 2) ) ); 
                        *((int *)pBuffer + 2) += sizeof(int);

                        printf("\tEmail: %s", (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) );
                        *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));

                        *((int *)pBuffer + 2) = 1;

                        break;
                    }
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                    *((int *)pBuffer + 2) += sizeof(int);
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                }
                if ( *((int *)pBuffer + 2) != 1 ) {
                    printf("Pessoa não encontrada. \n");
                }
                // define os contadores para 0 novamente
                *((int *)pBuffer + 1) = 0;
                *((int *)pBuffer + 2) = 0; 
                // bota um terminador no inicio do buffer de leitura
                *(char *)((char *)pBuffer + sizeof(int) * 4 ) = '\0';

                break;
            case 4: // Listar todos
                // Verifica se a Agenda possui alguma pessoa a ser listada
                if (  *((int *)pBuffer + 3) == TAM_INICIAL ) {
                    printf("Agenda vazia. \n");
                    break;
                } 
                *((int *)pBuffer + 2) = TAM_INICIAL;

                while ( *((int *)pBuffer + 2) != *((int *)pBuffer + 3) ) {
                    printf("Pessoa nº%d\n", ++(*((int *)pBuffer + 1)));

                    printf("\tNome: %s", (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) );
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));

                    printf("\tIdade: %d\n", *(int *)((char *)pBuffer + *((int *)pBuffer + 2) ) ); 
                    *((int *)pBuffer + 2) += sizeof(int);

                    printf("\tEmail: %s", (char *)((char *)pBuffer + *((int *)pBuffer + 2) + sizeof(int) ) );
                    *((int *)pBuffer + 2) += sizeof(int) + sizeof(char) * *(int *)((char *)pBuffer + *((int *)pBuffer + 2));
                }

                // define os contadores para 0 novamente
                *((int *)pBuffer + 1) = 0;
                *((int *)pBuffer + 2) = 0; 

                break;
            case 5: // Sair
                // libera toda memória antes de sair
                free(pBuffer);
                exit(0);
                break;

            default: // Opção inválida
                printf("\nEscolha inválida.\n");
                break;
        }
    }
}
