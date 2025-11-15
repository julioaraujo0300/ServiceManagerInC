#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

struct Servico {
    char cliente[200];
    float horas;
    char data[10];//String de 10 carateres para poder guardar a data em formato DD/MM/AAAA
    float custo;
    char estado; // S para sim(já foi pago), N para não
};


//função usada para ser mais facil usar esta tabela e para que seja mais prático alterar esta
void escreverTabela(){
        printf("\n---------------------------------------------------------------------------------------------------------------\n");
        printf("Cliente\t\t\tHoras\tData\t\tCusto\t\tEstado\n");
        printf("---------------------------------------------------------------------------------------------------------------\n");
}

void inserir() {
    struct Servico serv;   //Cria uma instancia da struct Serviço em RAM
    FILE *fpServico;        //Apontador para o ficheiro em que serão guardados os dados
    char repete = 'N';     //Variavel responsavel por repetir a função


    // Abre o ficheiro em modo de escrita e leitura
    fpServico = fopen("servicos.dat", "rb+");
    if (!fpServico) {   //Se o ficheiro não existe
        //Cria ficheiro
        fpServico = fopen("servicos.dat", "wb");
        //Fecha o apontador criado acima para abrir o ficheiro que foi criado
        fclose(fpServico);
        //abre o ficheiro em mode de escrita e leitura
        fpServico = fopen("servicos.dat", "rb+");0;
    }

    //função criada para reutilizar esta lógica caso seja necessário inserir vários registos de uma só vez
    void LerRegistos(){
        //Pede ao utilizador a informação e guarda esta na struct serv
        printf("Digite o nome do cliente: ");
        gets(serv.cliente);
        printf("Digite as horas de serviço: ");
        scanf("%f", &serv.horas);
        fgetc(stdin);
        printf("Digite a data do serviço(em formato DD/MM/AAAA):");
        gets(serv.data);
        printf("Digite o custo do serviço: ");
        scanf("%f", &serv.custo);
        fgetc(stdin);
        printf("O serviço já foi pago?(S para sim, N para não): ");
        scanf("%c", &serv.estado);
        //funciona com ou sem caps lock para evitar frustração ao utilizador
        //Código para garantir que o estado é guardado com um valor válido
        while((serv.estado != 'S' && serv.estado != 's') && (serv.estado != 'N' && serv.estado != 'n')){
            printf("Resposta inválida, por favor digite um carater válido!(S para sim, N para não): ");
            scanf("%c", &serv.estado);
            fgetc(stdin);
        }
    }

    LerRegistos();

    //Colocar apontador no final do ficheiro para garantir que não escrevemos por cima de registos anteriores
    fseek(fpServico, 0, SEEK_END);
    //Escrever a informação inserida dentro do ficheiro
    fwrite(&serv, sizeof(struct Servico), 1, fpServico);

    fgetc(stdin);
    printf("Deseja inserir outro registo de serviço?(S se sim, N se não): ");
    scanf("%c", &repete);
    fgetc(stdin);


    //funciona com ou sem caps lock para evitar frustração ao utilizador
    //código para continuar a criar registos enquanto o utilizador desejar
    while(repete == 'S' || repete == 's'){
        //Chama a função para pedir informação ao utilizador de novo
        LerRegistos();
        //Colocar apontador no final do ficheiro para garantir que não escrevemos por cima de registos anteriores
        fseek(fpServico, 0, SEEK_END);
        //Escrever a informação inserida dentro do ficheiro
        fwrite(&serv, sizeof(struct Servico), 1, fpServico);
        printf("Deseja inserir outro registo de serviço?(S se sim, N se não): ");
        scanf("%c", &repete);
        fgetc(stdin);

    }

    //Fechar o apontador após conclusão
    fclose(fpServico);
}

void listar() {
    struct Servico serv;
    FILE *fpservico;
    fpservico = fopen("servicos.dat", "rb");
    //verifica se chegou ao fim do ficheiro(atua como boolean)
    int fimFicheiro = 0;
    //varaiaveis para acumular os valores de serviços pagos e serviços em divida
    float acumuladorPago = 0;
    float acumuladorDivida = 0;


    //Ficheiro existe e está ok
    if (fpservico) {
        //coloca o apontador no inicio do ficheiro
        fseek(fpservico, 0, SEEK_SET);
        //Chamo a função para criar a tabela
        escreverTabela();
        while (fimFicheiro == 0) { //Ciclo de leitura dos registos
            //lê todos os registos no ficheiro
            fread(&serv, sizeof(struct Servico), 1, fpservico);
            if (feof(fpservico)){
                fimFicheiro = 1;
            }
            else {
                //Verifica o valor do estado do serviço e altera o que imprime e o que acumula conforme este
                if(serv.estado == 'S' || serv.estado == 's')
                {
                    printf("%s\t\t\t%.1f\t%s\t%.2f\t\tPago\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    acumuladorPago += serv.custo;
                }
                else
                {
                    printf("%s\t\t\t%.1f\t%s\t%.2f\t\tEm dívida\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    acumuladorDivida += serv.custo;
                }
            }
        }
        //fechamos o ficheiro
        fclose(fpservico);
        printf("Foram recebidos %.2f € pelos serviços prestados.\n", acumuladorPago);
        printf("Estão em dívida %.2f €.\n", acumuladorDivida);

        printf("'ENTER' para continuar:\n");
        //fgetc é usado aqui para garantir que o enter inserido ao chamar a função não é lido aqui
        fgetc(stdin);
    }
    //Código para informar o utilizador que não existem quaisquer registos
    else{
        printf("Não existem registos!\n");
        printf("'ENTER' para continuar:\n");
        fgetc(stdin);
    }
}

void pesquisar(){
    struct Servico serv;
    FILE *fpservico;
    fpservico = fopen("servicos.dat", "rb");
    //variavel para guardar o nome escrito pelo utilizador
    char cliente[200];
    int encontrou = 0; //variavel para verificar se existe algum registo ao qual corresponde o nome

    //Ficheiro existe e está ok
    if (fpservico) {
        //Pede um nome ao utilizador
        printf("Qual o nome do cliente que deseja pesquisar?: ");
        gets(cliente);
        escreverTabela();
        //coloca o ponteiro no inicio do ficheiro
        fseek(fpservico, 0, SEEK_SET);
        int fimFicheiro = 0;
        while (fimFicheiro == 0) { //Ciclo de leitura dos registos
            //lê todos os registos
            fread(&serv, sizeof(struct Servico), 1, fpservico);
            //Verifica se chegou ao fim do ficheiro para quebrar o ciclo
            if (feof(fpservico)){
                fimFicheiro = 1;
            }
            else {
                //Verifica se algum campo de cliente na struct é igual ao que o utilizador escreveu
                if(strcmp(cliente, serv.cliente) == 0){
                    encontrou = 1;
                    //Caso encontre algo devolve a informação de todos os registos com esse cliente
                    if(serv.estado == 'S' || serv.estado == 's')
                    {
                        printf("%s\t\t\t%.1f\t%s\t%.2f\t\tPago\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    }
                    else
                    {
                        printf("%s\t\t\t%.1f\t%s\t%.2f\t\tEm dívida\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    }
                }
            }
        }
    }
    //caso nao encontre nada devolve uma mensagem ao utilizador
    if(encontrou == 0){
        printf("Nao foi encontrado nenhum registo desse cliente, verifique se escreveu corretamente!\n");
    }
    fclose(fpservico);
    printf("'ENTER' para continuar:");
    fgetc(stdin);
}

void filtrarPagos(){
    struct Servico serv;
    FILE *fpservico;
    fpservico = fopen("servicos.dat", "rb");
    //varaivel para acumular o valor pago
    float acumulador;

    //Ficheiro existe e está ok
    if (fpservico) {
        //coloca o apontador no inicio do ficheiro
        fseek(fpservico, 0, SEEK_SET);
        int fimFicheiro = 0;
        //chamo a função para criar tabela
        escreverTabela();
        while (fimFicheiro == 0) {
            //lê todos os registos
            fread(&serv, sizeof(struct Servico), 1, fpservico);
            if (feof(fpservico)){
                fimFicheiro = 1;
            }
            else {
                //garante que só são impressos os registos pagos
                if(serv.estado == 'S' || serv.estado == 's'){
                    printf("%s\t\t\t%.1f\t%s\t%.2f\t\tPago\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    //acumula o valor dos serviços pagos
                    acumulador += serv.custo;
                }
            }
        }
    }

    printf("Total pago: %.2f\n", acumulador);
    fclose(fpservico);
    printf("'ENTER' para continuar:");
    fgetc(stdin);
}


void filtrarDividas(){
    struct Servico serv;
    FILE *fpservico;
    fpservico = fopen("servicos.dat", "rb");
    float acumulador;

    //Ficheiro existe e está ok
    if (fpservico) {
        //coloca o apontador no inicio do ficheiro
        fseek(fpservico, 0, SEEK_SET);
        int fimFicheiro = 0;
        //chamo a função para criar tabela
        escreverTabela();
        while (fimFicheiro == 0) { //Ciclo de leitura dos registos
            //lê todos os registos no ficheiro
            fread(&serv, sizeof(struct Servico), 1, fpservico);
            if (feof(fpservico)) fimFicheiro = 1;
            else {
                //garante que só são impressos os registos em divida
                if(serv.estado == 'N' || serv.estado == 'n'){
                    printf("%s\t\t\t%.1f\t%s\t%.2f\t\tEm dívida\n", serv.cliente, serv.horas, serv.data, serv.custo);
                    //acumula o valor de todos os registos em divida
                    acumulador += serv.custo;
                }
            }
        }
    }

    printf("Total em dívida: %.2f €\n", acumulador);
    fclose(fpservico);
    printf("'ENTER' para continuar:");
    fgetc(stdin);
}


void alterar(){
    struct Servico serv;
    FILE *fpservico;
    fpservico = fopen("servicos.dat", "rb+");

    //ficheiro existe e está ok
    if(fpservico){
        //variaveis para guardar ediçoes
        float horas = 0;
        float custo = 0;
        //variavel para guardar qual o registo a alterar
        int alteraServico = 0;
        printf("Digite o id do registo que deseja alterar: ");
        scanf("%d", &alteraServico);
        //variavel para guardar o tamanho do ficheiro
        long tamFicheiro = 0;
        //colocamos o ponteiro no fim do ficheiro
        fseek(fpservico, 0, SEEK_END);
        //guardamos a posiçao do ponteiro(fim do ficheiro, ou seja o tamanho do ficheiro)
        tamFicheiro = ftell(fpservico);
        //ao dividir o tamanho do ficheiro pelo tamanho da struct sabemos o numero de registos
        int nrServicos = tamFicheiro /  sizeof(struct Servico);
        //verifica que o numero inserido pelo utilizador é válido
        if(alteraServico >= 1 && alteraServico <= nrServicos){
            //decrementamos um valor devido ao inicio dos registos ser em 0
            alteraServico--;
            //colocamos o ponteiro no inicio do registo
            fseek(fpservico, alteraServico*sizeof(struct Servico), SEEK_SET);
            //vai ler o que esta neste registo para poder alterar(o apontador avança ate ao fim do registo)
            fread(&serv, sizeof(struct Servico), 1, fpservico);
            //andamos com o ponteiro para tras para este ficar no inicio do registo de novo
            fseek(fpservico, -sizeof(struct Servico), SEEK_CUR);
            //variavel para guardar a escolha do utilizador
            int escolha = 0;
            printf("Qual o campo que deseja alterar?(1 para horas, 2 para custo, 3 para estado): ");
            scanf("%d", &escolha);
            //uso um swtich case para ter codigo mais limpo do que com if's
            switch(escolha) {
                case 1:
                    printf("Digite quantas horas quer adicionar: ");
                    scanf("%f", &horas);
                    //incrementa o valor de horas que o utilizador adicionar
                    serv.horas += horas;
                    break;
                case 2:
                    printf("Digite o novo custo deste serviço: ");
                    scanf("%f", &custo);
                    //atualiza o custo do serviço
                    serv.custo = custo;
                    break;
                case 3:
                    //inverte a variavel de estado
                    if(serv.estado == 'S' || serv.estado == 's')
                    {
                        printf("Serviço alterado para: Em dívida\n");
                        serv.estado = 'N';
                    }
                    else
                    {
                        printf("Serviço alterado para: Pago\n");
                        serv.estado = 'S';
                    }
                    break;
                //dá feedback ao utilizador caso este insira um numero invalido
                default:
                    printf("Resposta inválida!");
            }
            //atualiza a informação do registo
            fwrite(&serv, sizeof(struct Servico), 1, fpservico);
        }
        //fecha o ficheiro
        fclose(fpservico);
    }else{
        printf("Não existem registos para alterar!");
    }

}


int main()
{
    //Criar um menu aplicacional
    setlocale(LC_ALL, "");      // Para carateres portugueses
    int opcao = 0;              //resultado da escolha da opção
    do {
        printf("1- Inserir\n2- Listar\n3- Pesquisar\n4- Filtrar serviços em dívida\n5- Filtrar serviços pagos\n6- Alterar\n7- Saír");
        scanf("%d", &opcao);
        fgetc(stdin);
        if (opcao == 1) {
            inserir();
        }
        if (opcao == 2) {
            listar();
        }
        if (opcao == 3) {
            pesquisar();
        }
        if (opcao == 4) {
            filtrarDividas();
        }
        if (opcao == 5) {
            filtrarPagos();
        }
        if (opcao == 6) {
            alterar();
        }
    }while (opcao != 7);
    printf("\nAté à próxima\n");

    return 0;
}
