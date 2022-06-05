#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>

//FUNÇÕES

//LIMPA BUFFER DO GETS
void clear(){    char clear;    while((clear = getchar()) != '\n' && clear != EOF);}; //Inline
//ROLAR DADOS
int gera_dado(int,int);
//MENU INICIAL
void menu();
//SAVE
void carregar_save();
int salvar_jogo();
void apagar_save();
//CRIAÇÃO DO PERSONAGEM
void cria_personagem();
//COMEÇA O JOGO
void jogo();
//CRIA AS DUNGEONS ALEATORIAMENTE
void gera_dungeon();
//COMBATE
void combate();
void seu_turno();
void turno_inimigo(char[]);
//FIM DE JOGO
void fim_jogo();
//CREDITOS
void exibir_creditos();

//VARIAVEIS GLOBAIS

//NOME
char nome[61];
//PERSONAGEM
int i,dano=0,vida=0,vidamax=0,destreza=0,kills=0,classe=0,modificador=0, ca=3;
//JOGO
int historia,dungeon,batalha,dado, result, cont_dg=0, b_prob=0;
char inimigo[9];
FILE *save; //Arquivo de save
//INIMIGOS
int vida_inimigo, dano_inimigo, dado_dano_inim, ataque_inimigo;
unsigned short int boss_morto = 0;

//EXIBE OS STATUS
void status(){    printf("\nSuas estatísticas:\nNOME  = %s\nDANO  = %i\nVIDA  = %i / %i\nDESTREZA = %i\nKILLS = %i\n\n",nome,dano,vida,vidamax,destreza,kills);}; //Inline

int main(){

    setlocale(LC_ALL,"Portuguese"); //Coloca o padrão do encoding em português, permitindo o uso de acentuações

    menu(); // Chama o menu

    system ("PAUSE");
    return 0;
}

//ROLAR DADOS
int gera_dado(int a,int b){

    srand(time(NULL));
    if(a>b){
        int aux = b;
        b = a;
        a = aux;
    }
    return a + rand() % (b-a+1);
}

void menu(){

    system("CLS");

    int opcao_menu=0;

    printf("---------HIGHLANDER RPG---------\n\n");
    printf("1- Novo Jogo\n");
    printf("2- Carregar Jogo\n");
    printf("3- Excluir Save\n");
    printf("4- Creditos\n");
    printf("5- Sair\n");

    do{
        printf("\nSelecione a opção desejada:\n");
        scanf("%d", &opcao_menu);

        switch(opcao_menu){
        case 1:
            cria_personagem(); // Chama a função que cria o personagem
            break;
        case 2:
            carregar_save(); // Chama a função que carrega as informações do arquivo save
            break;
        case 3:
            apagar_save(); // Chama a função que apaga o arquivo save
            break;
        case 4:
            exibir_creditos(); // Chama a função que mostra os creditos do jogo
            break;
        case 5:
            system("taskkill /IM cb_console_runner.exe"); //"mata" o processo (esse codigo só funciona no Windows)
            break;
        default:
            printf ("Opção invalida!\n");
            break;
        }
    }while(opcao_menu<1 || opcao_menu>5);
}

void carregar_save(){

    save = fopen("save.txt", "rt"); //Abre o arquivo save.txt
    result = fscanf(save, "%s ,%d,%d,%d,%d,%d,%d", &nome, &dano, &vida,&vidamax,&destreza,&kills,&classe); //Guarda as informações do arquivo nas variaveis
    if(result<0){ //Verifica se leu algo do arquivo
        printf("Não existe jogo salvo!\n");
        system("PAUSE");
        menu();
    }
    else if(result<7 || result>7){ //Verifica se o arquivo continha todas as informações necessarias
        printf("Save corrompido! Por favor crie um novo jogo!\n");
        system("PAUSE");
        menu();
    }
    return jogo();
}

int salvar_jogo(){

    clear();
    save = fopen("save.txt", "wt");  // Cria um arquivo texto para gravação
    if (save == NULL){  // Verifica se nào conseguiu criar
        printf("\nProblemas na CRIACAO do arquivo\n");
        return 0; //Retorna 0 se não salvou
    }
    //Salva os valores importantes
    result = fprintf(save,"%s ,%d,%d,%d,%d,%d,%d", nome, dano, vida,vidamax,destreza,kills,classe);
    if (result == EOF) //Verifica se salvou algo
        printf("\nErro na Gravacao\n");
    fclose(save);//Fecha o arquivo

    return 1; //Retorna 1 se salvou corretamente
}

void apagar_save(){

    clear();
    int rmv = remove("save.txt"); //apaga o arquivo save.txt
    if(rmv == NULL) //verifica se realmente apagou
        printf("\nSave apagado!\n");
    else
        printf("\nNão existe save para apagar!\n");

    system("PAUSE");
    return menu();
}

void cria_personagem(){

    system("cls");
    clear();
    //CRIAÇÂO DE PERSONAGEM
    printf("Digite seu nome: ");
    gets(nome);

    //DEFINIR CLASSE
    do{
        printf("\nEscolha entre as classes:\n1-Guerreiro\n2-Mago\n3-Ladino\n");
        scanf("%i",&classe);

        if(classe>3)
            printf("VOCE INSERIU UMA OPCAO INVALIDA!");

    }while (classe>3);

    //GUERREIRO
    if (classe==1){
        dano  += 6;
        vida  += 29;
        destreza += 1;
    }

    //MAGO
    if (classe==2){
        dano  += 10;
        vida  += 20;
        destreza += 2;
    }

    //LADINO
    if (classe==3){
        dano  += 5;
        vida  += 25;
        destreza += 5;
    }

    //DEFINIR MODIFICADOR
    do{
        printf("\nVocê tem direito a um bônus de modificador, você deseja bônus de:\n1-Vida\n2-Dano\n3-Destreza\n");
        scanf("%i",&modificador);

        if(classe>3 || modificador<0)
            printf("VOCE INSERIU UMA OPCAO INVALIDA!");

    }while (modificador>3 || modificador<0);

    //DANO
    if(modificador==2)
        dano+=5;

    //DESTREZA
    if(modificador==3)
        destreza+=4;

    //VIDA
    if(modificador==1)
        vida+=3;

    vidamax=vida;

    system("cls");

    return jogo();
}

void jogo(){

    printf("\nBem vindo a sua aventura %s",nome);
    status();

    //INICIO DO JOGO
    do{
        printf("\nVocê quer:\n1-Continuar a aventura\n2-Salvar\n3-Mostrar estatisticas\n4-Sair do jogo\n");
        scanf("%i",&historia);

        if(historia>4)
            printf("VOCE INSERIU UMA OPCAO INVALIDA!");

        //SALVA O JOGO
        else if(historia==2){
            int verify = salvar_jogo();
            if(verify==1)
                printf("Jogo salvo!!");
            else
                printf("Não foi possivel salvar :(");
        }

        //MOSTRAR ESTATISTICAS
        else if(historia==3){
            system("cls");
            status();
        }

        //AVANÇA NA DUNGEON
        else if(historia==1){
            do{
                system("cls");
                printf("Voce adentra escombros de uma antiga masmorra na busca de seus segredos e possiveis riquezas.\nEscolha algum desses caminhos para proseguir:\n1- Caminho 1\n2- Caminho 2\n3- Caminho 3\n4- Caminho 4\n5- Caminho 5\n");
                scanf("%i",&dungeon);
            }while(dungeon>5);

            gera_dungeon(); //CHAMA A FUNÇÃO QUE GERA A PROXIMA SALA
        }
    }while(historia!=4);

    fim_jogo(); //CHAMA A EXIBIÇÃO DO FIM DO JOGO
    system("cls");
    return menu();
}

void gera_dungeon(){

    int d100 = gera_dado(1,100);

    //CAMPFIRE - Você se cura se estiver com vida menor q a vida maxima
    if((d100>30&&d100<40) || (d100>60&&d100<70)){
        system("cls");
        dado=gera_dado(2,12);
        printf("Voce encontra um local tranquilo e aconchegante na caverna, você consegue montar uma pequena fogueira e descansar por um tempo\nEla vai te curar em %i pontos de vida\n",dado);
        if (vida<vidamax){
            if(vida+dado>vidamax)
                vida=vidamax;
            else
                vida=vida+dado;
        }
        dado=0;
    }

    //BENÇÂO - Você ganha bonus de vida(podendo ultrapassar a sua vida maxima), dano ou destreza, á sua escolha
    else if(d100<5||(d100>50&&d100<55)) {
        system("cls");
        printf("Voce tateia uma parede que revela uma passagem secreta, nela você se depara com uma desgastada estatua de uma divindade. Mesmo não a conhecendo você realiza algumas preces. Você recebe uma bencao que permite aumentar algum atributo");

        do{
            printf("\nVoce quer uma benção de:\n1-Dano\n2-Vida\n3-Destreza\n");
            scanf("%i",&modificador);
            //DANO
            if(modificador==1)
                dano=dano+5;

            //DESTREZA
            if(modificador==3)
                destreza=destreza+4;

            //VIDA
            if(modificador==2)
            {
                vida=vida+3;
                vidamax=vidamax+3;
            }

            if(classe>3)
                printf("VOCE INSERIU UMA OPCAO INVALIDA!");

        }while(modificador>3);
    }

    else
        combate(); //CHAMA A FUNÇÃO QUE CONTROLA A GERAÇÃO DE INIMIGOS E O COMBATE

    cont_dg++;
}

void combate(){

    clear();
    system("cls");

    if(cont_dg==5){
        b_prob+=10;
        cont_dg = -1;
    }

    int dungeon = gera_dado(1,100) + b_prob;

    //CAMARA DO ESQUELETO
    if((dungeon<15)){

        strcpy(inimigo, "Esqueleto"); //Define a variavel 'inimigo' como Esqueleto
        vida_inimigo = 12+gera_dado(1,8);
        dano_inimigo = 3;
        dado_dano_inim = 6;
        printf("Voce entra numa camara com um cheiro desagradavel. Ao abrir a porta você é engorfado por um ar pútrido e alguns ratos passam pelas suas pernas. Entrando, a pouca iluminação revela uma sala repleta de ossadas. Não demora muito para sons de estalos ecoarem pela sala e os ossos levitarem e formarem um %s.\n", inimigo);

        //COMBATE
        do{
            //SEU ATAQUE
            seu_turno();

            //ESQUELETO ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //CAMARA DOS RATOS
    else if(dungeon<30){
        strcpy(inimigo, "Rato"); //Define a variavel 'inimigo' como Rato
        vida_inimigo = 8+gera_dado(1,4);
        dano_inimigo = 2;
        dado_dano_inim = 6;

        printf("Um cheiro de esgoto se eleva até as suas narinas vindo de uma porta adiante. Você a abre e vê um pequeno trecho de água que possui detritos em sua nascente justificando o cheiro. Acompanhando o canal voce ve uma criatura mórbida devorando restos de algo. Um grande rato repleto de feridas infeccionadas que se volta contra voce espumando de raiva\n");

        do{
            //VOCE ATACANDO
            seu_turno(inimigo);

            //RATO ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //CAMARA DO CARNIÇAL
    else if((dungeon<45)){

        strcpy(inimigo, "Carniçal"); //Define a variavel 'inimigo' como Carniçal
        vida_inimigo = 14 + gera_dado(1,8);
        dano_inimigo = 6;
        dado_dano_inim = 4;

        printf("Assim que voce entra numa sala, um frio percorre sua espinha o levando a analisar rapidamente o local, que aparenta estar vazio. Mais alguns passos adiante voce percebe no chao uma grande mancha de sangue no chao e gotas pingando do teto. Erguendo seu olhar voce se depara com uma criatura de pele albina com uma boca escancarada. O carniçal avança sobre você.\n");

        do{
            //VOCE ATACANDO
            seu_turno(inimigo);

            //CARNIÇAL ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //CAMARA DO GNOLL
    else if(dungeon<60){

        strcpy(inimigo, "Gnoll"); //Define a variavel 'inimigo' como Gnoll
        vida_inimigo = 14 + gera_dado(1,8);
        dano_inimigo = 4;
        dado_dano_inim = 6;

        printf("Barulhos macabros ecoam pelos corredores. Adentrando ainda mais na masmorra, voce percebe uma movimentacao na sala ao lado. Um bando de criaturas humanoides com caracteristicas de hiena, devorando um corpo enquanto gargalham sem parar. Voce acidentalmente pisa em falso num osso fazendo barulho e os alertandos. Os Gnolls agora sabem da sua presença.\n");

        do{
            //VOCE ATACANDO
            seu_turno(inimigo);

            //GNOLL ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //CAMARA DO MIMICO
    else if(dungeon<75){

        strcpy(inimigo, "Mímico"); //Define a variavel 'inimigo' como Mímico
        vida_inimigo = 10+gera_dado(1,6);
        dano_inimigo = 4;
        dado_dano_inim = 6;

        printf("Voce avista um bau ao fundo da sala e sua ganacia fala mais alto. Ignorando investigar a sala voce segue direto para o bau. Colocando suas maos nele, percebe uma substancia pegajosa sendo secretada pelo bau, que abre alguns olhos e uma grande boca no lugar da tampa. Um mimico se revelou.\n");

        do{
            //VOCE ATACANDO
            seu_turno(inimigo);

            //MIMICO ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //CAMARA DO LOBISOMEM
    else if(dungeon<90){

        strcpy(inimigo, "Lobisomem"); //Define a variavel 'inimigo' como Lobisomem
        vida_inimigo = 18+gera_dado(1,10);
        dano_inimigo = 5;
        dado_dano_inim = 4;

        printf("Gritos de agonia aparentam vir de dentro da sala. Ao centro, um homem se debatendo no chão em agonia clama por sangue estendendo o braço em sua direção. Sua fisionomia se distorce numa mescla de homem e lobo, seus gritos gradativamente se tornam rosnados ferozes. Um lobisomem acaba de surgir na sua frente e parte para o ataque.\n");

        do{
            //VOCE ATACANDO
            seu_turno(inimigo);

            //LOBISOMEM ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);

        }while(vida_inimigo>0 && vida>0);
    }

    //BOSS
    else if(dungeon>=90){

        strcpy(inimigo, "Minotauro"); //Define a variavel 'inimigo' como Minotauro
        vida_inimigo = 56;
        dano_inimigo = 6;
        dado_dano_inim = 12;

        printf("Adentrando ainda mais a masmorra, você se depara ao fundo da camara um grande minotauro sentado numa pilha de caraveres, afiando seu enorme machado. Ele ergue a cabeça percebendo sua presença e demonstra uma grande hostilidade.\n");

        do{
            seu_turno(inimigo);

            //BOSS ATACANDO
            turno_inimigo(inimigo);

            printf("\nVIDA: %i / %i\n",vida,vidamax);


        }while(vida_inimigo>0 && vida>0);

        if(vida_inimigo==0)
            boss_morto = 1;

        historia=4;
        return;
    }
}
void seu_turno(){
    //GUEREIRO
    if(classe==1){
        do{
            printf("Voce tem as seguintes opções:\n1-Espadada\n2-Recuperar folego\n");
            scanf("%i",&batalha);

            if(batalha>2)
                printf("\nVOCE INSERIU UMA OPCAO INVALIDA!\n");

        }while(batalha>2);

        if(batalha==1){
            dado=gera_dado(1,6);
            vida_inimigo-=(dano+dado);
            printf("Você levanta sua espada e consegue atingir o %s, dando %i de dano\n",inimigo,(dano+dado));
        }

        if(batalha==2){
            if(vida<vidamax){
                dado=11+gera_dado(1,10);
                if(vida+dado>vidamax){
                    printf("Você foge, tentando se afastar do %s, e consegue recuperar seu folego.\nVocê recupera %i pontos de vida.\n",inimigo,vidamax-vida);
                    vida=vidamax;
                }

                else{
                    vida=vida+dado;
                    printf("Você foge, tentando se afastar do %s, e consegue recuperar seu folego.\nVocê recupera %i pontos de vida.\n",inimigo,dado);
                }
            }
        }
    }

    //MAGO
    if(classe==2){
        do{
            printf("Voce tem as seguintes opções:\n1-Raio de fogo\n2-Oração de cura\n3-Raio acido\n");
            scanf("%i",&batalha);

            if(batalha>3)
                printf("\nVOCE INSERIU UMA OPCAO INVALIDA!\n");

        }while(batalha>3);

        if(batalha==1){
            dado=gera_dado(1,6);
            vida_inimigo-=(dano+dado);
            printf("Você junta todas as suas forças, criando e lançando um raio de fogo contra o %s, dando %i de dano.\n",inimigo,(dano+dado));
        }

        if(batalha==2){
            if(vida<vidamax){
                dado=5+gera_dado(1,8);
                if(vida+dado>vidamax){
                    printf("Voce foge e se ajoelha enquanto o %s te procura, sua oração cria uma luz na sala.\nVocê recupera %i pontos de vida.\n",inimigo,vidamax-vida);
                    vida=vidamax;
                }
                else{
                    vida=vida+dado;
                    printf("Voce foge e se ajoelha enquanto o %s te procura, sua oração cria uma luz na sala.\nVocê recupera %i pontos de vida.\n",inimigo,dado);
                }
            }
        }

        if(batalha==3){
            dado=gera_dado(1,10);
            vida_inimigo-=(dano+dado);
            printf("Você faz como se fosse uma dança e puxa do ceu um raio verde que vai direto na direção do %s, dando %i de dano\n",inimigo,(dano+dado));
        }
    }

    //LADINO
    if(classe==3){
        do{
            printf("Voce tem as seguintes opções:\n1-Rapieira\n2-Se esconder\n");
            scanf("%i",&batalha);

            if(batalha>2)
                printf("\nVOCE INSERIU UMA OPCAO INVALIDA!\n");

        }
        while(batalha>2);

        if(batalha==1){
            dado=8+gera_dado(1,8);
            vida_inimigo-=(dano+dado);
            printf("Você levanta sua rapieira e consegue atingir o %s, dando %i de dano\n",inimigo,(dano+dado));
        }

        if(batalha==2){
            if(vida<vidamax){
                dado=gera_dado(1,6);
                if(vida+dado>vidamax){
                    printf("Você foge, e se esconde do %s.\nVocê recupera %i pontos de vida.\n",inimigo,vidamax-vida);
                    vida=vidamax;
                }
                else{
                    vida=vida+dado;
                    printf("Você foge, e se esconde do %s.\nVocê recupera %i pontos de vida.\n",inimigo,dado);
                }
            }
        }
    }

    if(vida_inimigo<=0){
        printf("\nVOCE MATOU SEU INIMIGO!\n");
        vida_inimigo=0;
        kills++;
    }
}

void turno_inimigo(char inimigo[]){

    if(inimigo!='Minotauro'){ //Verifica se não é o boss
        if(vida>0 && vida_inimigo>0){ //Só ataca se você e ele ainda estiverem vivos

            printf("\nÉ A VEZ DELE TE ATACAR!\n");

            ataque_inimigo = gera_dado(1,20);

            if(ataque_inimigo>ca+destreza){

                dado=gera_dado(2,dado_dano_inim);
                vida-=(dano_inimigo+dado);

                //Mostra a descrição do ataque de cada inimigo
                if(inimigo=='Carniçal')
                    printf("\nO %s de forma desordenada e violenta avança com suas garras, causando %d de dano\n",inimigo,(dano_inimigo+dado));
                else if(inimigo=='Esqueleto')
                    printf("\nO %s vai até você e o ataca com sua espada, causando %d de dano\n",inimigo,(dano_inimigo+dado));
                else if(inimigo=='Gnoll')
                    printf("\nO %s corre em sua direção com uma lança rudimentar e estende ela contra uma abertura em sua defesa, causando %d de dano\n",inimigo,(dano_inimigo+dado));
                else if(inimigo=='Lobisomem')
                    printf("\nA besta avança contra você numa feroz investida de mordidas e arranhões, causando %d de dano\n",inimigo,(dano_inimigo+dado));
                else if(inimigo=='Mímico')
                    printf("\nAquela espécie de baú com vida própria vem saltitando em sua direção com sua enorme boca dentada, o mordendo e causando %d de dano\n",(dano_inimigo+dado));
                else if(inimigo=='Rato')
                    printf("\nA criatura pula em sua direcao com sua boca escancarada. Ela é mais agil que voce e o morde lhe causando %d de dano\n",(dano_inimigo+dado));
                else //Mostra uma descrição de ataque generico
                    printf("\nO %s avança contra você, lhe desferindo um golpe e causando %d de dano\n",inimigo,(dano_inimigo+dado));
            }

            else
                printf("\nO %s vai até você e o ataca, mas acaba errando\n",inimigo);

            if(vida<0){
                historia=4;
                vida=0;
            }
        }
    }

    else{
        if(vida>0 && vida_inimigo>0){ //Só ataca se você e ele ainda estiverem vivos

            printf("\nÉ A VEZ DELE TE ATACAR!");
            dado=gera_dado(1,2);

            ataque_inimigo = gera_dado(1,20);

            if(ataque_inimigo>ca+destreza){

                //MACHADO
                if(dado==1){
                    dado=gera_dado(2,dado_dano_inim);
                    vida-=(dano_inimigo+dado);

                    printf("\nO %s ergue o machado fazendo um movimento na diagonal te acertando causando %i de dano\n\n",inimigo,(dano_inimigo+dado));
                }

                //CHIFRADA
                if(dado==2){
                    dado=gera_dado(1,dado_dano_inim-2);
                    vida-=(dano_inimigo+dado+1);

                    printf("\nO %s abaixa sua cabeça e num solavanco avança contra você, te perfurando com seus chifres e causando %i de dano\n\n",inimigo,(dano_inimigo+dado));
                }

                if(vida<0){
                    historia=4;
                    vida=0;
                }
            }
            else
                printf("\n\nO %s vai até você e o ataca, mas acaba errando\n\n",inimigo);
        }
    }
}

void fim_jogo(){
    if(boss_morto==1 || vida==0){
        //SAIDA MATANDO BOSS
        if(boss_morto==1)
            printf("\nVocê finalizando o minotauro ve um brilho vindo da pilha de cadaveres, vasculhando voce acha uma pequena chave e segue seu caminho, finalizando a masmorra, voce acha um bau com a fechadura que parece ter encaixe para sua chave. Voce a usa e ve um bau repleto de terouros. Parabens Aventureiro\n");

        //SAIDA MORRENDO
        else
            printf("\nVOCÊ MORREU :(\n");

        printf("\nFIM DO JOGO!\n");
        status();
        dano=vida=vidamax=destreza=kills=classe=modificador=0;
        ca=3;
        system("pause");
    }
}

void exibir_creditos(){ //CREDITOS
    system("CLS");

    printf("Highlander RPG \n\nRPG de texto desenvolvido em C para a disciplina de Linguagem de Programação I \nCiência da Computação - UESC \n2021 \n\n");

    printf("Desenvolvedores/Discentes:\n");
    printf("- João Manoel A. Oliveira / John K. Parsec\n");
    printf("- João Victor Leita da Silva Almeida / Sir Duende\n");
    printf("- Arthur Marques Azevedo / Elon Musk\n");

    printf("\nDocente:\n-Helder Conceição Almeida\n\n");

    system("PAUSE");
    return menu();
}
