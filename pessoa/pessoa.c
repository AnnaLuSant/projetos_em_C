#include <stdio.h>
#include <string.h>

typedef struct
{
    int id;
    char nome[50];
    float saldo;
} usuario;

void gravarProdutos(const char *arquivo)
{
    FILE *fp = fopen(arquivo, "a"); // "a" para não apagar o que já existe
    if (fp == NULL)
    {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    int n;

    printf("Quantos usuarios deseja cadastrar? ");
    scanf("%d", &n);
    getchar(); // consome o Enter que sobrou

    if (n > 100)
    {
        printf("mt gnt mn, morra... \n");
        fclose(fp);
        return;
    }

    usuario usuario[100];

    for (int i = 0; i < n; i++)
    {
        printf("\n--- usuarios %d ---\n", i + 1);

        printf("ID: ");
        scanf("%d", &usuario[i].id);
        getchar();

        if (usuario[i].id <= 0) {  // nao pode aceitar valor 0 e nem negativo
            printf("valor invalido, por favor digite algo");
            i--;
            continue;
        }

        printf("Nome: ");
        fgets(usuario[i].nome, sizeof(usuario[i].nome), stdin);
        usuario[i].nome[strcspn(usuario[i].nome, "\n")] = '\0'; // troca o \n por fim de texto

        if (usuario[i].nome[0] == '\0') // \0 é o final da string, se ele tiver igual a \0, esta vazio
        {
            printf("Nome invalido, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        printf("Valor: ");
        scanf("%f", &usuario[i].saldo);
        getchar();

        if (usuario[i].saldo <= 0) // nao pode aceitar valor 0 e nem negativo 
        {
            printf("valor invalido, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d;%s;%.2f\n",
                usuario[i].id, usuario[i].nome,
                usuario[i].saldo);
    }

    fclose(fp);
    printf("Usuarios gravados com sucesso!\n");
}

//func para Ordenar 

//func de ler os produtos do .txt
int lerProdutos(const char *arquivo, usuario u[])
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
               &u[n].id,
               &u[n].nome,
               &u[n].saldo);
        n++; // vai adicionando ao array
    }
    fclose(fp);

    return n;
}

//funcao de printa a tabela
void formatarTabela(usuario u[], int n)
{
    printf("\n-----------------------------------------------------\n");
    printf("ID | Nome                | saldo |\n");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < n; i++) // percorre cada item do array de struct de Produtos
    {
        printf("%d | %-19s | %.2f |\n", // %-19 formata para a linha ficar na msm posicao
               u[i].id,
               u[i].nome,
               u[i].saldo);
    }
    printf("-----------------------------------------------------\n");
}

int main()
{
    // oq falta: ordenar usuarios e produtos
    // logica de compra (saldo tem q ser maior q (valor * qtd), tem q ter a qtd certa, diminuir o saldo e a qtd no estoque)
    const char *arquivo = "Pessoas.txt";
    usuario u[100];
    while (1)
    {

        printf("\n------------------------\n ");
        printf("Bem vindo a loja \n");
        printf("------------------------\n");
        printf("1. cadastrar Pessoa \n");
        printf("2. Listar Pessoa\n");
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
            n = lerProdutos(arquivo, u);
            formatarTabela(u, n);    
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