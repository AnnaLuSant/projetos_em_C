#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 256

typedef struct
{
    int id;
    char nome[50];
    char categoria[30];
    float valor;
    int qtd;
} produtos;

// func pra ler e ve o ID
int proximoId(const char *arquivo)
{
    FILE *leitura = fopen(arquivo, "r");
    if (leitura == NULL)
    {
        return 1;
    }

    char linha[MAX_LINHA];
    int ultimo_id = 0;
    int id;

    while (fgets(linha, sizeof(linha), leitura))
    {
        if (sscanf(linha, "%d", &id) == 1)
        {
            ultimo_id = id; // id é onde o valor esta salvo
        }
    }

    fclose(leitura);
    return ultimo_id + 1;
}

void transformaEVerificaEmMinuscula(char nomes[])
{
    // percorre enquanto
    for (int i = 0; nomes[i] != '\0'; i++)
    {
        nomes[i] = tolower(nomes[i]);
    }
}

// verifica se o nome esta no arquivo e para
int nomejaExisteNoArquivo(const char *arquivo, char b[])
{
    FILE *leitura = fopen(arquivo, "r"); // r de leitura em ingles
    if (leitura == NULL)
    {
        return 0;
    }

    char linha[MAX_LINHA];
    char nomeLido[50];

    while (fgets(linha, sizeof(linha), leitura))
    {
        if (sscanf(linha, "%*d,%49[^,]", nomeLido) == 1) // esse cod é para ele ler a linh ate 50 de tamanho
        {
            transformaEVerificaEmMinuscula(nomeLido); // transfomra emm minuscula para verificar o conteudo, se n chamasse ia dar como diferente por ter 1 maiuscula
            if (strcmp(nomeLido, b) == 0)
            {
                fclose(leitura);
                return 1;
                // printf("pau no seu cu! nao ppode te 2 nomes iguas ");
            }
        }
    }
    fclose(leitura);
    return 0;
}

// essa func aq verifica se a pessoa foi adicionar mais de 1 nome e vai escrever 2 vezes o msm nome
int nomeJaEscrito(produtos produto[], int tamanho, const char *nome)
{
    for (int i = 0; i < tamanho; i++)
    {
        if (strcmp(produto[i].nome, nome) == 0)
        {
            return 1;
        }
    }
    return 0;
}

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

    int id_atual = proximoId(arquivo);

    for (int i = 0; i < n; i++)
    {
        printf("\n--- Produto %d ---\n", i + 1);

        // if (n > 1) {
        //             ultimo_id = produto[i].id + 1;
        // } else {
        //     produto[i].id =ultimo_id;
        // }

        printf("Nome: ");
        fgets(produto[i].nome, sizeof(produto[i].nome), stdin);
        produto[i].nome[strcspn(produto[i].nome, "\n")] = '\0'; // troca o \n por fim de texto

        transformaEVerificaEmMinuscula(produto[i].nome);

        if (produto[i].nome[0] == '\0') // \0 é o final da string, se ele tiver igual a \0, esta vazio
        {
            printf("Nome invalido, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        if (strchr(produto[i].nome, ',') != NULL)
        {
            printf("Nome nao pode ter virgula");
            i--;
            continue;
        }

        if (nomejaExisteNoArquivo(arquivo, produto[i].nome) || nomeJaEscrito(produto, i, produto[i].nome))
        {
            printf("Nao pode adicionar mais de 1 nome igual");
            i--;
            continue;
        }

        printf("Categoria: ");
        fgets(produto[i].categoria, sizeof(produto[i].categoria), stdin);
        produto[i].categoria[strcspn(produto[i].categoria, "\n")] = '\0'; // troca o \n por fim de texto

        if (produto[i].categoria[0] == '\0') // categoria vazia nao pode
        {
            printf("Categoria invalida, por favor digite algo");
            i--;
            continue;
        }

        if (strchr(produto[i].categoria, ',') != NULL)
        {
            printf("Categoria nao pode ter virgula");
            i--;
            continue;
        }

        printf("Valor: ");
        scanf("%f", &produto[i].valor);
        getchar();

        if (produto[i].valor <= 0)
        { // nao pode aceitar valor 0 e nem negativo
            printf("valor invalido, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        printf("Quantidade em estoque: ");
        scanf("%d", &produto[i].qtd);
        getchar();

        if (produto[i].qtd < 0)
        { // nao pode aceitar valor 0 e nem negativo
            printf("quantidade invalida, por favor digite algo");
            i--; // i-- serve para voltar uma açao, se deu erro o loop continua, mas com o i-- ele volta
            continue;
        }

        produto[i].id = id_atual++;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d,%s,%s,%.2f,%d\n",
                produto[i].id, produto[i].nome,
                produto[i].categoria, produto[i].valor, produto[i].qtd);
    }

    fclose(fp);
    printf("Produtos gravados com sucesso!\n");
}

// func de ler os produtos do csv
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
        // ADAPTADO: so conta a linha se os 5 campos foram lidos.
        // Antes uma linha em branco virava um produto "lixo", e agora que
        // o arquivo passa a ser reescrito (modificar/excluir), esse lixo seria gravado.
        if (sscanf(linha, "%d,%49[^,],%29[^,],%f,%d",
                   &p[n].id,
                   p[n].nome,
                   p[n].categoria,
                   &p[n].valor,
                   &p[n].qtd) == 5)
        {
            n++; // vai adicionando ao array
        }
    }

    fclose(fp);
    return n;
}

// NOVO: reescreve o csv inteiro com o array (usada por modificar e excluir)
// "w" apaga o conteudo antigo e grava de novo, no mesmo formato do gravarProdutos
// reescreve o csv usando um arquivo temporario (evita perder dados se o programa cair no meio)
int reescreverProdutos(const char *arquivo, produtos p[], int n)
{
    const char *temporario = "Produtos.tmp";

    // 1. grava tudo no temporario, o original continua intacto
    FILE *fp = fopen(temporario, "w");
    if (fp == NULL)
    {
        printf("Erro ao criar arquivo temporario!\n");
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d,%s,%s,%.2f,%d\n",
                p[i].id, p[i].nome,
                p[i].categoria, p[i].valor, p[i].qtd);
    }

    fclose(fp);
    remove(arquivo); // no Windows o rename falha se o destino ja existe
    if (rename(temporario, arquivo) != 0)
    {
        printf("Erro ao substituir o arquivo original!\n");
        return 0;
    }

    return 1;
}
// funcao de printa a tabela
void formatarTabela(produtos p[], int n)
{
    printf("\n----------------------------------------------------------------------\n");
    printf("ID | Nome                | Categoria      | Valor | Quantidade |\n");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d | %-19s | %-14s | %.2f | %d |\n",
               p[i].id,
               p[i].nome,
               p[i].categoria,
               p[i].valor,
               p[i].qtd);
    }
    printf("----------------------------------------------------------------------\n");
}

// Função de buscar produtos pelo ID
void buscarProduto(const char *arquivo)
{
    produtos p[100];
    int id_busca;
    int p_encontrado = 0;

    int n = lerProdutos(arquivo, p);

    printf("\nDigite o ID do produto: ");
    scanf("%d", &id_busca);

    for (int i = 0; i < n; i++)
    {
        if (p[i].id == id_busca)
        {
            printf("\nProduto encontrado!\n");
            formatarTabela(&p[i], 1);

            p_encontrado = 1;
            break;
        }
    }

    if (p_encontrado == 0)
    {
        printf("\nProduto nao encontrado!\n");
    }
}

// NOVO: le um ID digitado e devolve a posicao dele no array (-1 se nao achar ou se digitou errado)
int pedirIdEBuscarPosicao(produtos p[], int n, const char *mensagem)
{
    char linha[MAX_LINHA];
    int id_busca;

    printf("\n%s", mensagem);
    fgets(linha, sizeof(linha), stdin);

    if (sscanf(linha, "%d", &id_busca) != 1)
    {
        return -1;
    }

    for (int i = 0; i < n; i++)
    {
        if (p[i].id == id_busca)
        {
            return i;
        }
    }
    return -1;
}

// NOVO: modificar um produto pelo ID (mesmas validacoes do cadastro)
void modificarProduto(const char *arquivo)
{
    produtos p[100];
    char linha[MAX_LINHA];

    int n = lerProdutos(arquivo, p);
    if (n == 0)
    {
        printf("Nenhum produto encontrado!\n");
        return;
    }

    getchar(); // consome o Enter que sobrou do scanf do menu

    int pos = pedirIdEBuscarPosicao(p, n, "Digite o ID do produto que deseja modificar: ");
    if (pos == -1)
    {
        printf("\nProduto nao encontrado!\n");
        return;
    }

    printf("\nProduto atual:\n");
    formatarTabela(&p[pos], 1);

    produtos novo = p[pos]; // trabalha numa copia, so salva no final (o id continua o mesmo)

    // NOME
    while (1)
    {
        printf("Novo nome: ");
        fgets(novo.nome, sizeof(novo.nome), stdin);
        novo.nome[strcspn(novo.nome, "\n")] = '\0';

        transformaEVerificaEmMinuscula(novo.nome);

        if (novo.nome[0] == '\0')
        {
            printf("Nome invalido, por favor digite algo\n");
            continue;
        }

        if (strchr(novo.nome, ',') != NULL)
        {
            printf("Nome nao pode ter virgula\n");
            continue;
        }

        // compara com todos os OUTROS produtos (o proprio produto pode manter o nome)
        int repetido = 0;
        for (int i = 0; i < n; i++)
        {
            if (i != pos && strcmp(p[i].nome, novo.nome) == 0)
            {
                repetido = 1;
                break;
            }
        }
        if (repetido)
        {
            printf("Nao pode adicionar mais de 1 nome igual\n");
            continue;
        }
        break;
    }

    // CATEGORIA
    while (1)
    {
        printf("Nova categoria: ");
        fgets(novo.categoria, sizeof(novo.categoria), stdin);
        novo.categoria[strcspn(novo.categoria, "\n")] = '\0';

        if (novo.categoria[0] == '\0')
        {
            printf("Categoria invalida, por favor digite algo\n");
            continue;
        }

        if (strchr(novo.categoria, ',') != NULL)
        {
            printf("Categoria nao pode ter virgula\n");
            continue;
        }
        break;
    }

    // VALOR
    while (1)
    {
        printf("Novo valor: ");
        fgets(linha, sizeof(linha), stdin);

        if (sscanf(linha, "%f", &novo.valor) != 1 || novo.valor <= 0)
        {
            printf("valor invalido, por favor digite algo\n");
            continue;
        }
        break;
    }

    // QUANTIDADE
    while (1)
    {
        printf("Nova quantidade em estoque: ");
        fgets(linha, sizeof(linha), stdin);

        if (sscanf(linha, "%d", &novo.qtd) != 1 || novo.qtd < 0)
        {
            printf("quantidade invalida, por favor digite algo\n");
            continue;
        }
        break;
    }

    p[pos] = novo;

    if (reescreverProdutos(arquivo, p, n))
    {
        printf("Produto modificado com sucesso!\n");
    }
}

// exclui um produto pelo ID
void excluirProduto(const char *arquivo)
{
    produtos p[100];

    int n = lerProdutos(arquivo, p);
    if (n == 0)
    {
        printf("Nenhum produto encontrado!\n");
        return;
    }

    getchar(); // consome o Enter que sobrou do scanf do menu

    int pos = pedirIdEBuscarPosicao(p, n, "Digite o ID do produto que deseja excluir: ");
    if (pos == -1)
    {
        printf("\nProduto nao encontrado!\n");
        return;
    }

    printf("\nProduto a ser excluido:\n");
    formatarTabela(&p[pos], 1);

    // puxa todo mundo depois dele uma posicao pra tras, tampando o buraco
    for (int i = pos; i < n - 1; i++)
    {
        p[i] = p[i + 1];
    }
    n--;

    if (reescreverProdutos(arquivo, p, n))
    {
        printf("Produto excluido com sucesso!\n");
    }
}

// Ordenar produtos em ordem algabética
void mariaJoaquina(produtos v[], int inicio, int fim)
{
    int i = inicio, j = fim;
    produtos pivo = v[(inicio + fim) / 2];
    produtos aux;

    while (i <= j)
    {
        // strcmp compara 2 strings
        //  comparando letra por letra
        while (strcmp(v[i].nome, pivo.nome) < 0)
            i++;
        while (strcmp(v[j].nome, pivo.nome) > 0)
            j--;
        if (i <= j)
        {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;

            i++;
            j--;
        }
    }
    if (inicio < j)
        mariaJoaquina(v, inicio, j);

    if (i < fim)
        mariaJoaquina(v, i, fim);
}

// busca de Z para A -> mesma coisa do quickSort MariaJoaquina, só muda o sinal na verificacao
void cirilo(produtos v[], int inicio, int fim)
{
    int i = inicio, j = fim;
    produtos pivo = v[(inicio + fim) / 2];
    produtos aux;

    while (i <= j)
    {
        // Vai avançando enquanto estiver dps do meio
        while (strcmp(v[i].nome, pivo.nome) > 0)
            i++;

        // Vai voltando enquanto estiver antes do pivô
        while (strcmp(v[j].nome, pivo.nome) < 0)
            j--;

        if (i <= j)
        {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;

            i++;
            j--;
        }
    }

    if (inicio < j)
        cirilo(v, inicio, j);

    if (i < fim)
        cirilo(v, i, fim);
}

// func de ordenar preco do menor pro maior
void ordenarPrecoMenorMaior(produtos v[], int inicio, int fim)
{
    int i = inicio, j = fim;
    produtos pivo = v[(inicio + fim) / 2];
    produtos aux;

    while (i <= j)
    {
        // Avança enquanto o preço for menor que o o meio
        while (v[i].valor < pivo.valor)
            i++;

        // Volta enquanto o preço for maior que o o meio
        while (v[j].valor > pivo.valor)
            j--;

        if (i <= j)
        {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;

            i++;
            j--;
        }
    }

    if (inicio < j)
        ordenarPrecoMenorMaior(v, inicio, j);

    if (i < fim)
        ordenarPrecoMenorMaior(v, i, fim);
}

// mesma coisa da funcao acima, so inverte o sinal na verificacao
void ordenarPrecoMaiorMenor(produtos v[], int inicio, int fim)
{
    int i = inicio, j = fim;
    produtos pivo = v[(inicio + fim) / 2];
    produtos aux;

    while (i <= j)
    {
        // Avança enquanto o preço for maior que o meio
        while (v[i].valor > pivo.valor)
            i++;

        // Volta enquanto o preço for menor que o o meio
        while (v[j].valor < pivo.valor) // mudou só o sinal
            j--;

        if (i <= j)
        {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;

            i++;
            j--;
        }
    }

    if (inicio < j)
        ordenarPrecoMaiorMenor(v, inicio, j);

    if (i < fim)
        ordenarPrecoMaiorMenor(v, i, fim);
}

// busca pelo nome, ignora maiuscula e minuscula
void buscarPorNome(const char *arquivo)
{
    produtos p[100];
    produtos achados[100];
    char busca[50];
    char nomeMin[50];
    int qtd = 0;

    int n = lerProdutos(arquivo, p);
    if (n == 0)
    {
        printf("\nNenhum produto cadastrado!\n");
        return;
    }

    getchar(); // consome o Enter que sobrou do scanf do menu

    printf("\nDigite o nome: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0'; // troca o \n por fim de texto
    transformaEVerificaEmMinuscula(busca);

    for (int i = 0; i < n; i++)
    {
        strcpy(nomeMin, p[i].nome); // copia pra n alterar o nome original
        transformaEVerificaEmMinuscula(nomeMin);

        // strstr acha o texto dentro do outro, se n achar devolve NULL
        if (strstr(nomeMin, busca) != NULL)
        {
            achados[qtd] = p[i];
            qtd++;
        }
    }

    if (qtd == 0)
    {
        printf("\nProduto nao encontrado!\n");
    }
    else
    {
        formatarTabela(achados, qtd);
    }
}

// busca pela categoria, mesma logica da busca por nome
void buscarPorCategoria(const char *arquivo)
{
    produtos p[100];
    produtos achados[100];
    char busca[30];
    char catMin[30];
    int qtd = 0;

    int n = lerProdutos(arquivo, p);
    if (n == 0)
    {
        printf("\nNenhum produto cadastrado!\n");
        return;
    }

    getchar(); // consome o Enter que sobrou do scanf do menu

    printf("\nDigite a categoria: ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';
    transformaEVerificaEmMinuscula(busca);

    for (int i = 0; i < n; i++)
    {
        strcpy(catMin, p[i].categoria);
        transformaEVerificaEmMinuscula(catMin);

        if (strstr(catMin, busca) != NULL)
        {
            achados[qtd] = p[i];
            qtd++;
        }
    }

    if (qtd == 0)
    {
        printf("\nNenhum produto nessa categoria!\n");
    }
    else
    {
        formatarTabela(achados, qtd);
    }
}

int main()
{
    const char *arquivo = "Produtos.csv";

    produtos p[100];

    while (1)
    {
        printf("\n------------------------\n ");
        printf("Bem vindo a loja \n");
        printf("------------------------\n");
        printf("1. Cadastrar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto por ID\n");
        printf("4. Ordenar por nome (A-Z)\n");
        printf("5. Ordenar por nome (Z-A)\n");
        printf("6. Ordenar por preco (menor -> maior)\n");
        printf("7. Ordenar por preco (maior -> menor)\n");
        printf("8. Modificar produto\n");
        printf("9. Excluir produto\n");
        printf("10. Buscar produto por nome\n");
        printf("11. Buscar produto por categoria\n");
        printf("0. Sair da loja\n");
        printf("------------------------\n");
        printf("Escolha uma Opcao: \n");

        int opcao;
        scanf("%d", &opcao);

        int n;
        switch (opcao)
        {
        case 1:
            gravarProdutos(arquivo);
            break;

        case 2:
            n = lerProdutos(arquivo, p);
            formatarTabela(p, n);
            break;

        case 3:
            buscarProduto(arquivo);
            break;

        case 4: // nome A-Z
            n = lerProdutos(arquivo, p);
            if (n > 0)
            {
                mariaJoaquina(p, 0, n - 1);
                formatarTabela(p, n);
            }
            else
            {
                printf("Nenhum produto encontrado!\n");
            }
            break;

        case 5: // nome Z-A
            n = lerProdutos(arquivo, p);
            if (n > 0)
            {
                cirilo(p, 0, n - 1);
                formatarTabela(p, n);
            }
            else
            {
                printf("Nenhum produto encontrado!\n");
            }
            break;

        case 6: // preco menor -> maior
            n = lerProdutos(arquivo, p);
            if (n > 0)
            {
                ordenarPrecoMenorMaior(p, 0, n - 1);
                formatarTabela(p, n);
            }
            else
            {
                printf("Nenhum produto encontrado!\n");
            }
            break;

        case 7: // preco maior -> menor
            n = lerProdutos(arquivo, p);
            if (n > 0)
            {
                ordenarPrecoMaiorMenor(p, 0, n - 1);
                formatarTabela(p, n);
            }
            else
            {
                printf("Nenhum produto encontrado!\n");
            }
            break;

        case 8:
            modificarProduto(arquivo);
            break;

        case 9:
            excluirProduto(arquivo);
            break;

        case 10:
            buscarPorNome(arquivo);
            break;

        case 11:
            buscarPorCategoria(arquivo);
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
