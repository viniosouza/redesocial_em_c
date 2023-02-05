#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct filaRequisicoes{
    int id;                    //identificaçăo do usuário;
    struct filaRequisicoes *proximo;

};
typedef struct filaRequisicoes filaReq;

struct perfil{
    int id;
    char* nome;
 };
typedef struct perfil Perfil;

struct user{
    struct perfil *perfilDoUsuario;
    struct user *listaDeAmigos;            // é struct user
    struct filaRequisicoes *amigosPendentes;
    struct user *proximoUser;
};
typedef struct user User;

struct listaInicio{
    User *inicio;
};
typedef struct listaInicio Lista;

int alocaUser(User **novo){
    if(((*novo) = (User*)malloc(sizeof(User))) == NULL)return -1;
    if(((*novo)->perfilDoUsuario = (Perfil*)malloc(sizeof(Perfil))) == NULL)return -1;
    if(((*novo)->perfilDoUsuario->nome = (char*)malloc(sizeof(char)*30)) == NULL)return -1;
    //if(((*novo)->listaDeAmigos = (User*)malloc(sizeof(User))) == NULL)return -1;
    (*novo)->listaDeAmigos = NULL;
    //if(((*novo)->amigosPendentes = (filaReq*)malloc(sizeof(filaReq))) == NULL)return -1;
    (*novo)->amigosPendentes = NULL;
    return 0;
}

void criarNovo(Lista *lista, char *name){
    User *novo, *aux;

    if((alocaUser(&novo)) != 0)printf("erro de alocacao");
    else{

        if(lista->inicio == NULL){
            novo->perfilDoUsuario->id = 1;
            strcpy(novo->perfilDoUsuario->nome, name);
            novo->proximoUser = NULL;
            lista->inicio = novo;
        }
        else{
            aux = lista->inicio;
            while(aux->proximoUser!=NULL){
                aux = aux->proximoUser;
            }
            novo->perfilDoUsuario->id = aux->perfilDoUsuario->id;
            novo->perfilDoUsuario->id++;
            strcpy(novo->perfilDoUsuario->nome, name);
            novo->proximoUser = NULL;
            aux->proximoUser = novo;
        }
    }
}

User* buscarPorId(Lista *lista, int id){
    User *aux;
    aux = lista->inicio;
    while(aux!=NULL){
        if(aux->perfilDoUsuario->id == id){
            return aux;
        }
        aux = aux->proximoUser;
    }
    printf("Nao foi possivel encontrar o id informado\n");
    return NULL;

}

User* buscarPorNome(Lista *lista, char* nome){
    User *aux;
    int i=1;
    aux = lista->inicio;
    while(aux!=NULL){
        i = strcmp(aux->perfilDoUsuario->nome, nome);
        if(i == 0){
            printf("%p\n", aux);
            return aux;
        }
        aux = aux->proximoUser;
    }
    printf("Nao foi possivel encontrar o nome informado\n");
    return NULL;
}

void desalocaUser(User *userRemov){
    free(userRemov->perfilDoUsuario->nome);
    free(userRemov->perfilDoUsuario);
    free(userRemov->listaDeAmigos);
    free(userRemov->amigosPendentes);
    free(userRemov);
}

int removerPorId(Lista *lista, int id){
    User* user_remov, *anterior;

    anterior = NULL;
    user_remov = lista->inicio;
    while(user_remov!=NULL){
        if(user_remov->perfilDoUsuario->id == id){
            if(anterior == NULL){

                lista->inicio = user_remov->proximoUser;

                desalocaUser(user_remov);

                return 1;
            }
            else if(user_remov->proximoUser == NULL){
                anterior->proximoUser = NULL;

                desalocaUser(user_remov);

                return 1;
            }
            else{
                anterior->proximoUser = user_remov->proximoUser;

                desalocaUser(user_remov);

                return 1;
            }
        }
        else{
            anterior = user_remov;
            user_remov = user_remov->proximoUser;
        }
    }

    return 0;
}

int removerPorNome(Lista *lista, char* nome){
    User* user_remov, *anterior;
    int i=1;
    anterior = NULL;
    user_remov = lista->inicio;
    while(user_remov!=NULL){

        i = strcmp(user_remov->perfilDoUsuario->nome, nome);

        if(i == 0){
            if(anterior == NULL){

                lista->inicio = user_remov->proximoUser;

                desalocaUser(user_remov);

                return 1;
            }
            else if(user_remov->proximoUser == NULL){
                anterior->proximoUser = NULL;

                desalocaUser(user_remov);

                return 1;
            }
            else{
                anterior->proximoUser = user_remov->proximoUser;

                desalocaUser(user_remov);

                return 1;
            }
        }
        else{
            anterior = user_remov;
            user_remov = user_remov->proximoUser;
        }
    }

    return 0;
}

void inserirfila(filaReq **fila, int num){
    filaReq *aux, *novo = malloc(sizeof(filaReq));
    if(novo){
        novo->id = num;
        novo->proximo = NULL;
        if(*fila == NULL){
            *fila = novo;
        }
        else{
            aux = *fila;
            while(aux->proximo){
                aux = aux->proximo;
            }
            aux->proximo = novo;
        }
    }
    else
        printf("erro de alocacao");
}

void solicitarAmizade(Lista *listaUser, int idPerfilSolicitante, int idPerfilAmigo){
    User *aux;
    filaReq *aux_fila;
    aux = listaUser->inicio;

    if((buscarPorId(listaUser, idPerfilAmigo)) && (buscarPorId(listaUser, idPerfilSolicitante)) != NULL){

        if(idPerfilSolicitante != idPerfilAmigo){

            while(aux != NULL){

                if(idPerfilAmigo == aux->perfilDoUsuario->id){
                    aux_fila = aux->amigosPendentes;

            //esse while vai verificar se ja existe a mesma solicitaçao na fila

                    while(aux_fila != NULL){
                        if(idPerfilSolicitante == aux_fila->id)
                            return;
                        aux_fila = aux_fila->proximo;
                    }
                    inserirfila(&aux->amigosPendentes, idPerfilSolicitante);
                    break;
                }
                aux = aux->proximoUser;
            }
        }
        else
            printf("nao pode pedir amizade para voce mesmo");
    }
    else
        printf("ids nao existem na lista\n");
}

void mostraFila(Lista *lista){
    filaReq *aux;
    User *aux2;
    aux2 = lista->inicio;
    while(aux2 != NULL){
        printf("Lista de amigos pendentes de %s", aux2->perfilDoUsuario->nome);
        if(aux2->amigosPendentes == NULL){
            printf("O usuario nao possui amigos pendentes\n");
        }
        else{
            aux = aux2->amigosPendentes;
            while(aux != NULL){
                printf("%d\n", aux->id);
                aux = aux->proximo;
            }
        }
        aux2 = aux2->proximoUser;
    }

}

void mostra(Lista *lista){
    User *aux;
    aux =lista->inicio;
    while(aux!=NULL){
        printf("%d\n", aux->perfilDoUsuario->id);
        printf("%s\n", aux->perfilDoUsuario->nome);
        aux = aux->proximoUser;
    }
}

void mostraAmigos(Lista *lista){
    User *aux, *aux2;
    aux = lista->inicio;
    while(aux!=NULL){
        printf("Lista de amigos de %s", aux->perfilDoUsuario->nome);
        aux2 = aux->listaDeAmigos;
        if(aux2 != NULL){
            while(aux2!=NULL){

                printf("%s\n", aux2->perfilDoUsuario->nome);
                printf("%d\n", aux2->perfilDoUsuario->id);
                aux2 = aux2->proximoUser;
            }
        }
        else{
            printf("Nao tem amigos\n");
        }
        aux = aux->proximoUser;
    }

}


filaReq* removerFila(filaReq **fila){
    filaReq *remover = NULL;

    if(*fila != NULL){
        remover = *fila;
        *fila = remover->proximo;
    }
    else
        printf("lista vazia\n");
    return remover;
}


void aceitarPrimeiraSolicitacaoAmizade(Lista *listaUser, int idPerfil){      // nao sei como usar a struct Lista, e nao sei se crio nova funcao para alocar ou uso a existente
    User *aux_perfil, *aux_perfil_amigo, *novoAmigo;
    filaReq *aux_pendente;

    aux_perfil = buscarPorId(listaUser, idPerfil);
    if(aux_perfil != NULL){ // verifica se existe o perfil na lista
        aux_pendente = removerFila(&aux_perfil->amigosPendentes);
        if(aux_pendente != NULL){
            aux_perfil_amigo = buscarPorId(listaUser, aux_pendente->id);
            free(aux_pendente);
        //  libero a memoria do elemento removido da fila de pendentes
            if(aux_perfil->listaDeAmigos == NULL){
                if((alocaUser(&novoAmigo)) != 0)printf("erro de alocacao");
                novoAmigo->proximoUser = NULL;
                novoAmigo->perfilDoUsuario->id = aux_perfil_amigo->perfilDoUsuario->id;
                strcpy(novoAmigo->perfilDoUsuario->nome, aux_perfil_amigo->perfilDoUsuario->nome);
                aux_perfil->listaDeAmigos = novoAmigo;
            }
            else{
                if((alocaUser(&novoAmigo)) != 0)printf("erro de alocacao");
                novoAmigo->proximoUser = NULL;
                novoAmigo->perfilDoUsuario->id = aux_perfil_amigo->perfilDoUsuario->id;
                strcpy(novoAmigo->perfilDoUsuario->nome, aux_perfil_amigo->perfilDoUsuario->nome);

                User *temp;
                temp = aux_perfil->listaDeAmigos;
                // temp recebe o primeiro elemento da listaAmigos e ao final do while recebe o ultimo endreço da lista
                while(temp->proximoUser != NULL) temp = temp->proximoUser;
                temp->proximoUser = novoAmigo;
            }
        }
    }
    else{
        printf("o id nao existe\n");
    }
}

void aceitarTodasSolicitacaoAmizade(Lista *listaUser, int idPerfil){
    User *aux_perfil;
    aux_perfil = buscarPorId(listaUser, idPerfil);
    if(aux_perfil->amigosPendentes != NULL){
    // verifica se o perfil possui amizades pendentes
        while(aux_perfil->amigosPendentes != NULL){

        aceitarPrimeiraSolicitacaoAmizade(listaUser, idPerfil);
    //  O aux->amigosPendentes é modificado pela funcao acima
    //
        }
    }
    else{
        printf("O perfil nao tem amizades pendentes\n");
    }
}

void rejeitarPrimeiraSolicitacaoAmizade(Lista *listaUser, int idPerfil){
    User *aux_perfil;
    filaReq *aux_pendente;

    aux_perfil = buscarPorId(listaUser, idPerfil);

    if(aux_perfil != NULL){
        aux_pendente = removerFila(&aux_perfil->amigosPendentes);
        free(aux_pendente);
    }
}

void rejeitarTodasSolicitacaoAmizade(Lista *listaUser, int idPerfil){
    User *aux_perfil;

    aux_perfil = buscarPorId(listaUser, idPerfil);
    if(aux_perfil != NULL){
        while(aux_perfil->amigosPendentes != NULL){
            rejeitarPrimeiraSolicitacaoAmizade(listaUser, idPerfil);
        }
    }
}

int numAmigos(Lista *listaUser, int idPerfil){
    User *aux_perfil, *listaAmigo;
    int amigos=0;
    aux_perfil = buscarPorId(listaUser, idPerfil);
    if(aux_perfil != NULL){
        listaAmigo = aux_perfil->listaDeAmigos;
        while(listaAmigo != NULL){
            amigos++;
            listaAmigo = listaAmigo->proximoUser;
        }
        return amigos;
    }
}





User* quemEhOPerfilMaisAmigo(Lista *listaUser){



}






int main(){
    char *test;
    int i=0;
    Lista minhaLista;
    minhaLista.inicio = NULL;


    test = (char*)malloc(sizeof(char)*30);
    if(test == NULL){
        printf("erro de alocação nome 1\n");
    }

    printf("digite um nome para inserir:\n");
    fgets(test, 30, stdin);
    criarNovo(&minhaLista, test);

    printf("digite um nome para inserir:\n");
    fgets(test, 30, stdin);
    criarNovo(&minhaLista, test);

    printf("digite um nome para inserir:\n");
    fgets(test, 30, stdin);
    criarNovo(&minhaLista, test);

    mostra(&minhaLista);

    solicitarAmizade(&minhaLista, 1, 2);

    solicitarAmizade(&minhaLista, 3, 2);

    solicitarAmizade(&minhaLista, 1, 3);


    aceitarPrimeiraSolicitacaoAmizade(&minhaLista, 2);
    rejeitarPrimeiraSolicitacaoAmizade(&minhaLista, 2);
    aceitarPrimeiraSolicitacaoAmizade(&minhaLista, 3);
    rejeitarTodasSolicitacaoAmizade(&minhaLista, 2);

    //mostraFila(&minhaLista);


    mostraAmigos(&minhaLista);


    return 0;
}