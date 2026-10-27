#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    char nome[50];
    float valor;
    int qtd;
} produtos;

void gravarProdutos(const char *arquivo)
{
    FILE *fp = fopen(arquivo, "a"); // "a" para não apagar o que já existe
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    int n;

    printf("Quantos produtos deseja gravar? ");
    scanf("%d", &n);
    getchar(); // consome o Enter que sobrou

    if (n > 100)
    {
        printf("\nSeu estoque nao tem capacidade para tanta coisa\n");
        fclose(fp);
        return;
    }

    produtos produto[100];

    for (int i = 0; i < n; i++)
    {
        printf("\n--- Produto %d ---\n", i + 1);

        printf("ID: ");
        scanf("%d", &produto[i].id);
        getchar();

        if (produto[i].id <= 0) {  // nao pode aceitar valor 0 e nem negativo
            printf("valor invalido, por favor digite algo");
            i--;
            continue;
        }

        printf("Nome: ");
        fgets(produto[i].nome, sizeof(produto[i].nome), stdin);
        produto[i].nome[strcspn(produto[i].nome, "\n")] = '\0'; // troca o \n por fim de texto

        if (produto[i].nome[0] == '\0') // \0 é o final da string, se ele tiver igual a \0, esta vazio
        { 
            printf("Nome invalido, por favor ddigite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        printf("Valor: ");
        scanf("%f", &produto[i].valor);
        getchar();

        if (produto[i].valor <= 0) { // nao pode aceitar valor 0 e nem negativo
            printf("valor invalido, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        printf("Quantidade em estoque: ");
        scanf("%d", &produto[i].qtd);
        getchar();
    
        if (produto[i].qtd <= 0) {  // nao pode aceitar valor 0 e nem negativo
            printf("quantidade invalida, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d;%s;%.2f;%d\n",
                produto[i].id, produto[i].nome,
                produto[i].valor, produto[i].qtd);
    }

    fclose(fp);
    printf("Produtos gravados com sucesso!\n");
}

//func de ler os produtos do .txt
int lerProdutos(const char *arquivo, produtos p[])
{
    FILE *fp = fopen(arquivo, "r");

    if (fp == NULL)
    {
        printf("Arquivo nao encontrado.\n");
        return 0;
    }

    char linha[300];
    int n = 0; // comeca o array em 0

    while (fgets(linha, sizeof(linha), fp) && n < 100) // vai percorrer 
    {
        sscanf(linha, "%d;%49[^;];%f;%d",
               &p[n].id,
               p[n].nome,
               &p[n].valor,
               &p[n].qtd);

        n++; // vai adicionando ao array
    }

    fclose(fp);

    return n;
}

//funcao de printa a tabela
void formatarTabela(produtos p[], int n)
{
    printf("\n-----------------------------------------------------\n");
    printf("ID | Nome                | Valor | Quantidade |\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < n; i++) // percorre cada item do array de struct de Produtos
    {
        printf("%d | %-19s | %.2f | %d |\n", // %-19 formata para a linha ficar na msm posicao
               p[i].id,
               p[i].nome,
               p[i].valor,
               p[i].qtd);
    }
    printf("-----------------------------------------------------\n");
}

int main()
{
    const char *arquivo = "Produtos.txt";
    
    produtos p[100];

    while (1)
    {

        printf("\n------------------------\n ");
        printf("Bem vindo a loja \n");
        printf("------------------------\n");
        printf("1. cadastrar produto \n");
        printf("2. Listar produto\n");
        printf("0. Sair da loja\n");
        printf("------------------------\n");
        printf("Escolha uma Opcao: \n");
        
        int opcao;
        scanf("%d" , &opcao);

        int n;
        switch (opcao) {
        case 1:
            gravarProdutos(arquivo);
            break;
        case 2:
            n = lerProdutos(arquivo, p);
            formatarTabela(p, n);    
            break;
        case 0:
            printf("saindo da loja...");
            return 0;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    }
    
    return 0;
}