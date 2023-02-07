/*
  O codigo é uma representação simplificada de uma rede social, a estrututa principal do código é uma lista enca-
  -deada, onde cada nó representa um usuário da rede. Cada usuário possui estruturas que irão guardar dados sobre
  ele e existem diversas funções para fazer a interação desses usuários.
  Código feito por : 
  AntonioAugustoPenteadoSoaresJúnior - 2210100447
  Vinicios Oliveira Souza -             
*/

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct filaRequisicoes {                                  
  int id;                                                  
  struct filaRequisicoes *proximo;
};
typedef struct filaRequisicoes filaReq;

struct perfil {
  int id;
  char *nome;
};
typedef struct perfil Perfil;

struct user {
  struct perfil *perfilDoUsuario;
  struct user *listaDeAmigos; 
  struct filaRequisicoes *amigosPendentes;
  struct user *proximoUser;
};
typedef struct user User;

// foi incrementado uma estrutura do tipo 'Lista' que guarda o início de uma lista para auxiliar na implementação do código 
struct listaInicio {
  User *inicio;
};
typedef struct listaInicio Lista;

int alocaUser(User **novo) {
  if (((*novo) = (User *)malloc(sizeof(User))) == NULL)
    return -1;
  if (((*novo)->perfilDoUsuario = (Perfil *)malloc(sizeof(Perfil))) == NULL)
    return -1;
  if (((*novo)->perfilDoUsuario->nome = (char *)malloc(sizeof(char) * 30)) == NULL)
    return -1;
  (*novo)->listaDeAmigos = NULL;
  (*novo)->amigosPendentes = NULL;
  return 0;
}

void criarNovo(Lista *lista, char *name) {
  User *novo, *aux;

  if ((alocaUser(&novo)) != 0)
    printf("Erro de alocacao de usuario");
  else {
  // se não possuir usuário na lista ainda  
    if (lista->inicio == NULL) {
      novo->perfilDoUsuario->id = 1;
      strcpy(novo->perfilDoUsuario->nome, name);
      novo->proximoUser = NULL;
      lista->inicio = novo;
    } 
  // se já possuir usuários na lista 
    else {
      aux = lista->inicio;
      while (aux->proximoUser != NULL) {
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

User* buscarPorId(Lista *lista, int id) {
  User *aux;
  aux = lista->inicio;
  // aux recebe o endereço do primeiro elemento da lista
  while (aux != NULL) {
    if (aux->perfilDoUsuario->id == id) {
      return aux;
    }
    aux = aux->proximoUser;
  }
  printf("Nao foi possivel encontrar o id informado\n");
  return NULL;
}

User* buscarPorNome(Lista *lista, char *nome) {
  User *aux;
  int i = 1;
  aux = lista->inicio;
  while (aux != NULL) {
    i = strcmp(aux->perfilDoUsuario->nome, nome);
// quando duas strings sao iguais a função strcmp retorna 0
    if (i == 0) {
      return aux;
    }
    aux = aux->proximoUser;
  }
  printf("Nao foi possivel encontrar o nome informado\n");
  return NULL;
}

void desalocaUser(User *userRemov) {
  free(userRemov->perfilDoUsuario->nome);
  free(userRemov->perfilDoUsuario);
  free(userRemov->listaDeAmigos);
  free(userRemov->amigosPendentes);
  free(userRemov);
}

int removerPorId(Lista *lista, int id) {
  User *user_remov, *anterior;

  anterior = NULL;
  user_remov = lista->inicio;
  while (user_remov != NULL) {
    if (user_remov->perfilDoUsuario->id == id) {
      if (anterior == NULL) {

        lista->inicio = user_remov->proximoUser;

        desalocaUser(user_remov);

        return 1;
      } else if (user_remov->proximoUser == NULL) {
        anterior->proximoUser = NULL;

        desalocaUser(user_remov);

        return 1;
      } else {
        anterior->proximoUser = user_remov->proximoUser;

        desalocaUser(user_remov);

        return 1;
      }
    } else {
      anterior = user_remov;
      user_remov = user_remov->proximoUser;
    }
  }

  return 0;
}

int removerPorNome(Lista *lista, char *nome) {
  User *user_remov, *anterior;
  int i = 1;
  anterior = NULL;
  user_remov = lista->inicio;
  while (user_remov != NULL) {

    i = strcmp(user_remov->perfilDoUsuario->nome, nome);

    if (i == 0) {
      if (anterior == NULL) {

        lista->inicio = user_remov->proximoUser;

        desalocaUser(user_remov);

        return 1;
      } else if (user_remov->proximoUser == NULL) {
        anterior->proximoUser = NULL;

        desalocaUser(user_remov);

        return 1;
      } else {
        anterior->proximoUser = user_remov->proximoUser;

        desalocaUser(user_remov);

        return 1;
      }
    } 
    else {
      anterior = user_remov;
      user_remov = user_remov->proximoUser;
    }
  }

  return 0;
}

void inserirfila(filaReq **fila, int num) {
  filaReq *aux, *novo = malloc(sizeof(filaReq));
  if (novo) {
    novo->id = num;
    novo->proximo = NULL;
    if (*fila == NULL) {
      *fila = novo;
    } 
    else{
      aux = *fila;
      while (aux->proximo) {
        aux = aux->proximo;
      }
      aux->proximo = novo;
    }
  } 
  else
    printf("erro de alocacao");
}

void solicitarAmizade(Lista *listaUser, int idPerfilSolicitante,
                      int idPerfilAmigo) {
  User *aux;
  filaReq *aux_fila;
  aux = listaUser->inicio;

  if ((buscarPorId(listaUser, idPerfilAmigo)) &&
      (buscarPorId(listaUser, idPerfilSolicitante)) != NULL) {

    if (idPerfilSolicitante != idPerfilAmigo) {

      while (aux != NULL) {

        if (idPerfilAmigo == aux->perfilDoUsuario->id) {
          aux_fila = aux->amigosPendentes;

          // esse while vai verificar se ja existe a mesma solicitaçao na fila

          while (aux_fila != NULL) {
            if (idPerfilSolicitante == aux_fila->id)
              return;
            aux_fila = aux_fila->proximo;
          }
          inserirfila(&aux->amigosPendentes, idPerfilSolicitante);
          break;
        }
        aux = aux->proximoUser;
      }
    } else
      printf("nao pode pedir amizade para voce mesmo");
  } else
    printf("ids nao existem na lista\n");
}

void mostraFila(Lista *lista) {
  filaReq *aux;
  User *aux2;
  aux2 = lista->inicio;
  while (aux2 != NULL) {
    printf("Lista de amigos pendentes de %s", aux2->perfilDoUsuario->nome);
    if (aux2->amigosPendentes == NULL) {
      printf("O usuario nao possui amigos pendentes\n");
    } else {
      aux = aux2->amigosPendentes;
      while (aux != NULL) {
        printf("%d\n", aux->id);
        aux = aux->proximo;
      }
    }
    aux2 = aux2->proximoUser;
  }
}

void mostra(Lista *lista) {
  User *aux;
  aux = lista->inicio;
  while (aux != NULL) {
    printf("O ID do usuario que entrei eh: %d\n", aux->perfilDoUsuario->id);
    printf("Nome: %s\n", aux->perfilDoUsuario->nome);
    aux = aux->proximoUser;
  }
}

void mostraAmigos(Lista *lista) {
  User *aux, *aux2;
  aux = lista->inicio;
  while (aux != NULL) {
    printf("Lista de amigos de %s", aux->perfilDoUsuario->nome);
    aux2 = aux->listaDeAmigos;
    if (aux2 != NULL) {
      while (aux2 != NULL) {

        printf("%s\n", aux2->perfilDoUsuario->nome);
        printf("%d\n", aux2->perfilDoUsuario->id);
        aux2 = aux2->proximoUser;
      }
    } else {
      printf("Nao tem amigos\n");
    }
    aux = aux->proximoUser;
  }
}

filaReq *removerFila(filaReq **fila) {
  filaReq *remover = NULL;

  if (*fila != NULL) {
    remover = *fila;
    *fila = remover->proximo;
  } else
    printf("lista vazia\n");
  return remover;
}

void aceitarPrimeiraSolicitacaoAmizade(Lista *listaUser, int idPerfil) {
  User *aux_perfil, *aux_perfil_amigo, *novoAmigo;
  filaReq *aux_pendente;

  aux_perfil = buscarPorId(listaUser, idPerfil);
  if (aux_perfil != NULL) { // verifica se existe o perfil na lista
    aux_pendente = removerFila(&aux_perfil->amigosPendentes);
    if (aux_pendente != NULL) {
      aux_perfil_amigo = buscarPorId(listaUser, aux_pendente->id);
      free(aux_pendente);
      //  libero a memoria do elemento removido da fila de pendentes
      if (aux_perfil->listaDeAmigos == NULL) {
        if ((alocaUser(&novoAmigo)) != 0)
          printf("erro de alocacao");
        novoAmigo->proximoUser = NULL;
        novoAmigo->perfilDoUsuario->id = aux_perfil_amigo->perfilDoUsuario->id;
        strcpy(novoAmigo->perfilDoUsuario->nome,
               aux_perfil_amigo->perfilDoUsuario->nome);
        aux_perfil->listaDeAmigos = novoAmigo;
      } else {
        if ((alocaUser(&novoAmigo)) != 0)
          printf("erro de alocacao");
        novoAmigo->proximoUser = NULL;
        novoAmigo->perfilDoUsuario->id = aux_perfil_amigo->perfilDoUsuario->id;
        strcpy(novoAmigo->perfilDoUsuario->nome,
               aux_perfil_amigo->perfilDoUsuario->nome);

        User *temp;
        temp = aux_perfil->listaDeAmigos;
        // temp recebe o primeiro elemento da listaAmigos e ao final do while
        // recebe o ultimo endreço da lista
        while (temp->proximoUser != NULL)
          temp = temp->proximoUser;
        temp->proximoUser = novoAmigo;
      }
    }
  } else {
    printf("o id nao existe\n");
  }
}

void aceitarTodasSolicitacaoAmizade(Lista *listaUser, int idPerfil) {
  User *aux_perfil;
  aux_perfil = buscarPorId(listaUser, idPerfil);
  if (aux_perfil->amigosPendentes != NULL) {
    // verifica se o perfil possui amizades pendentes
    while (aux_perfil->amigosPendentes != NULL) {

      aceitarPrimeiraSolicitacaoAmizade(listaUser, idPerfil);
      //  O aux->amigosPendentes é modificado pela funcao acima
    }
  } else {
    printf("O perfil nao tem amizades pendentes\n");
  }
}

void rejeitarPrimeiraSolicitacaoAmizade(Lista *listaUser, int idPerfil) {
  User *aux_perfil;
  filaReq *aux_pendente;

  aux_perfil = buscarPorId(listaUser, idPerfil);

  if (aux_perfil != NULL) {
    aux_pendente = removerFila(&aux_perfil->amigosPendentes);
    free(aux_pendente);
    //  libera a memoria alocada do elemento removida da fila
  }
}

void rejeitarTodasSolicitacaoAmizade(Lista *listaUser, int idPerfil) {
  User *aux_perfil;

  aux_perfil = buscarPorId(listaUser, idPerfil);
  if (aux_perfil != NULL) {
    while (aux_perfil->amigosPendentes != NULL) {
      rejeitarPrimeiraSolicitacaoAmizade(listaUser, idPerfil);
    }
  }
}

int numAmigos(Lista *listaUser, int idPerfil) {
  User *aux_perfil, *listaAmigos;
  int amigos = 0;
  aux_perfil = buscarPorId(listaUser, idPerfil);
  if (aux_perfil != NULL) {
    listaAmigos = aux_perfil->listaDeAmigos;
    while (listaAmigos != NULL) {
      amigos++;
      listaAmigos = listaAmigos->proximoUser;
    }
    return amigos;
  }
  return amigos;
}

int numUsers(Lista *listaUser) {
  User *aux;
  int numUsuarios = 0;
  aux = listaUser->inicio;
  while (aux != NULL) {
    numUsuarios++;
    aux = aux->proximoUser;
  }
  return numUsuarios;
}

int numSolicitacoesAmigos(Lista *listaUser, int idPerfil) {
  User *aux_perfil;
  filaReq *aux_pendente;
  int numAmigosPendentes = 0;
  aux_perfil = buscarPorId(listaUser, idPerfil);
  if (aux_perfil != NULL) {
    aux_pendente = aux_perfil->amigosPendentes;
    while (aux_pendente != NULL) {
      numAmigosPendentes++;
      aux_pendente = aux_pendente->proximo;
    }
    return numAmigosPendentes;
  }
  return numAmigosPendentes;
}

// o perfil mais amigo é aquele que aparece mais vezes na lista de amigos de todos usuarios
User *quemEhOPerfilMaisAmigo(Lista *listaUser) {
  User *aux_perfil, *listaAmigosPerfil;
  int numUsuarios = numUsers(listaUser);
  int *vet;
// vet é um vetor dinamico de tamanho igual a da lista de usuarios
// o indice 0 de vet e igual ao usuario 1 da lista 'vet[0]==1'
// se existir na lista de amigos de um usuario o perfil com id == 1 entao o conteudo em vet[0] vai incrementar 
  aux_perfil = listaUser->inicio;
  vet = (int *)calloc(numUsuarios, sizeof(int));
  while (aux_perfil != NULL) {
    listaAmigosPerfil = aux_perfil->listaDeAmigos;  //recebe o endereço do primeiro elemento da lista de amigos
    while (listaAmigosPerfil != NULL) {
//  o for vai comparar o id contido em listaAmigosPerfil com todas as possibilidades de usuarios na lista de amigos 
//  quando encontrar o conteudo na posicao *(vet+i-1) vai incrementar
      for (int i = 1; i <= numUsuarios; i++) {  
        if (listaAmigosPerfil->perfilDoUsuario->id == i) {
          *(vet + i - 1) = *(vet + i - 1) + 1;
          break;
        }
      }
      listaAmigosPerfil = listaAmigosPerfil->proximoUser;
    }
    aux_perfil = aux_perfil->proximoUser;
  }
// agora iremos comparar e decobrir em que indice esta o maior elemento  
// maior recebe o conteudo do primeiro indice do vetor e iremos comparar este com o resto
// indice recebe 0 porque para ele sempre vai ser incrementado para fazer referencia ao id : 'vet[0] == id[1]' 'vet[1] == id[2]'
  int maior = *(vet);
  int indice = 0;
  User *usuarioMaisAmigo;
  for (int i = 1; i < numUsuarios; i++) {
    if (maior < *(vet + i)) {
      maior = *(vet + i);
      indice = i;
    }
  }
  indice++;
  free(vet);
// a partir daqui sabemos o id do usuario mais amigos e iremos encontrar seu endereço
  usuarioMaisAmigo = buscarPorId(listaUser, indice);
  return usuarioMaisAmigo;
}

void menu() {
  printf("****************** FACEBRICK ALEGRETE - A REDE SOCIAL DO MOMENTO "
         "*************************************************\n");
  //printf("\033[1;32m"); // Muda a cor para verde
  //printf("\033[1m");    // Torna o texto negrito
  //printf("\033[0m");    // Reseta a formatação

  printf("1 - Adicionar/criar um novo usuario na rede social \n");
  printf("2 - Buscar um perfil na rede social pelo id \n");
  printf("3 - Remover um usuario da rede social pelo nome\n");
  printf("4 - Solicitar a amizade de um perfil \n");
  printf("5 - Aceitar primeira soliciatacao de amizade \n");
  printf("6 - Quantidade de numero de amigos de um perfil e da rede social \n"); // o case no menu nao funciona mas as funçoes funcionam 
  printf("7 - Quantidade de solicitacoes de amizades de um perfil \n");
  printf("8 - Perfil mais amigo de todos \n");
  printf("9 - buscar um perfil pelo nome \n");
  printf("10 - Mostrar usuario \n");
  printf("11 - remover usuario pelo id\n");
  printf("12 - aceitar todas as solicitacoes de amizade\n");
  printf("13 - rejeitar primeira solicitacao\n");
  printf("14 - rejeitar todas as solicitacoes\n");
  printf("0 - Sair \n");
}

int main() {
  char *nome;
  User *aux;
  int i = 0;
  int opcao;
  int idPerfilSolicitante = 0;
  int idParaRemover = 0;
  int idParaBuscar = 0;
  int idPerfilAmigo = 0;
  int idPerfil;
  

  Lista minhaLista;
  minhaLista.inicio = NULL;

  nome = (char *)malloc(sizeof(char) * 30);
  if (nome == NULL) {
    printf("erro de alocação nome 1\n");
  }
  
  do {
    
    menu();

    printf("Digite sua opcao: ");
    scanf("%d", &opcao);
    fflush(stdin);

    switch (opcao) {
    case 1:
      // 1 - Adicionar/criar um novo usuário na rede social
      printf("Inserir o usuário na rede social:\n");
      scanf("%30[^\n]", nome);
      fflush(stdin);

      // Cria um novo
      criarNovo(&minhaLista, nome);

      printf("Seja bem-vindo %s \n", nome);

      system("cls");
      mostra(&minhaLista);
      break;

    case 2:
      // Buscar um perfil na rede social pelo id
      printf("Insira o ID que gostaria de buscar: \n");
      scanf("%d", &idParaBuscar);
      fflush(stdin);
      User *user;
      user = buscarPorId(&minhaLista, idParaBuscar);
      system("cls");
      printf("O usuario eh: %s \n", user->perfilDoUsuario->nome);

      break;

    case 3:
      // Remover um usuário da rede social pelo nome.
      printf("Insira o nome para remover o usuário: \n");
      scanf("%30[^\n]", nome);
      fflush(stdin);

      i = removerPorNome(&minhaLista, nome);
      if(i == 1){
        printf("remocao efetuada com exito\n");
      }
      else{
        printf("nao foi possivel remover usuario\n");
      }
      mostra(&minhaLista);

      break;

    case 4:
      // 4 - Solicitar a amizade de um perfil
      printf("Insira o perfil solicitante: \n");

      // Inserindo o IDPERFILSOLICITANTE
      scanf("%d", &idPerfilSolicitante);
      fflush(stdin);
      // Insira o perfil do seu amigo para solicitaão
      printf("Insira o perfil amigo: \n");
      scanf("%d", &idPerfilAmigo);
      fflush(stdin);
      // Solicita a amizade de acordo com o perfil de solicitação
      solicitarAmizade(&minhaLista, idPerfilSolicitante, idPerfilAmigo);
      break;

    case 5:
      // 5 - Aceitar primeira solociatacao amizade

      printf("Insira o ID do seu perfil:  \n");                 
      scanf("%d", &idPerfil);
      fflush(stdin);
      aceitarPrimeiraSolicitacaoAmizade(&minhaLista, idPerfil);

      break;

    case 6:
      // 6 - Quantidade de número de amigos de um perfil e da rede social
      printf("Para consultar o número de amigos da rede social do perfil, "
             "insira o ID do seu perfil:  \n");
      scanf("%d", &idPerfil);
      fflush(stdin);

      i = numAmigos(&minhaLista, idPerfil);
      printf("%d\n", i);
      i = numUsers(&minhaLista);
      printf("%d\n", i);

      //mostraAmigos(&minhaLista);

      break;

    case 7:
      // 7 - Quantidade de solicitações de amizades de um perfil
      printf("Insira o ID do perfil para consultar a quantidade de "
             "solicitações de amizades de um perfil: ");
      scanf("%d", &idPerfil);
      fflush(stdin);
      i = numSolicitacoesAmigos(&minhaLista, idPerfil);
      printf("%d\n", i);
      break;
    
    case 8:
      // 8 - Perfil mais amigo de todos
      printf("O id do perfil mais amigo : ");
      aux = quemEhOPerfilMaisAmigo(&minhaLista);
      printf("%d\n", aux->perfilDoUsuario->id);
      break;

    case 9:
      
      printf("Insira o nome para buscar o usuário: \n");
      scanf("%30[^\n]", nome);
      fflush(stdin);
      
      aux = buscarPorNome(&minhaLista, nome);
      system("cls");
      printf("o nome do perfil requisitado: %s\n", aux->perfilDoUsuario->nome);
      break;

    case 10:
      mostra(&minhaLista);
      break;
    
    case 11:
      // remover usuario pelo id
      printf("Insira o ID para remover o usuário: \n");
      scanf("%d", &idParaRemover);
      fflush(stdin);
      i = removerPorId(&minhaLista, idParaRemover);
      if(i == 0){
        printf("usuario removido com exito\n");
      }
      else{
        printf("nao foi possivel remover usuario\n");
      }

      mostra(&minhaLista);

      break;
    case 12:
    // aceitar todas solicitacoes  
      printf("Insira o ID do seu perfil:  \n");                 
      scanf("%d", &idPerfil);
      fflush(stdin);
      aceitarTodasSolicitacaoAmizade(&minhaLista, idPerfil);
      break;
   
    case 13:
    // rejeitar primeira solicitacao
      printf("Insira o ID do seu perfil:  \n");                 
      scanf("%d", &idPerfil);
      fflush(stdin);
      rejeitarPrimeiraSolicitacaoAmizade(&minhaLista, idPerfil);
      break;
    
    case 14:
    // rejeitar todas solicitacoes
      printf("Insira o ID do seu perfil:  \n");                 
      scanf("%d", &idPerfil);
      fflush(stdin);
      rejeitarTodasSolicitacaoAmizade(&minhaLista, idPerfil);
      break;
   
    default:
      printf("SAINDO............");
      break;
    }

  } while (opcao != 0);
  
  free(nome);
  
  return 0;
}