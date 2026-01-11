/*
*  myfs.c - Implementacao do sistema de arquivos MyFS
*
*  Autores: Gabhriel Velasco, Lucas Chaves, Marcelo Rother 
*  Projeto: Trabalho Pratico II - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include "myfs.h"
#include "disk.h"
#include "vfs.h"
#include "inode.h"
#include "util.h"

#define MAGIC_NUMBER 0xBEBACAFE    // Assinatura que identifica a gente
#define SB_SECTOR 0    // SuperBloco fica no setor 0
#define INODE_AREA_START 2    // Os inodes comecam no setor 2 
#define RESERVED_INODE_SECTORS 64    // 64 setores reservados para inodes (512 inodes)
#define DATA_AREA_START (INODE_AREA_START + RESERVED_INODE_SECTORS)

typedef struct {
  unsigned int magic;    // Numero magico (assinatura)
  unsigned int total_blocks;    // Total de setores do disco 
  unsigned int free_blocks_start;    // Prox setor livre
} SuperBlock;

typedef struct {
  char filename[MAX_FILENAME_LENGTH];
  unsigned int i_number;    // Numero do inode
  unsigned char is_active;    // 1 se existe, 0 se apagado
} DirEntry;

SuperBlock sb;
static Disk* curr_disk = NULL; 

typedef struct {
  int is_used;  // 1 se estiver em uso
  int inode_number;  // Qual inode esta aberto 
  unsigned int curr_offset_bytes;  // Cursor atual de leitura / escrita 
} FileHandle;

// GDT
FileHandle open_files[MAX_FDS];

void init_file_handles() {
  for (int i = 0; i < MAX_FDS; i++) {
    open_files[i].is_used = 0;
  }
}

//Funcao para verificacao se o sistema de arquivos está ocioso, ou seja,
//se nao ha quisquer descritores de arquivos em uso atualmente. Retorna
//um positivo se ocioso ou, caso contrario, 0.
int myFSIsIdle (Disk *d) {
	// Por enquanto vai ta sempre idle
  // TODO: verificar se ha arquivos abertos
  return 1;
}

//Funcao para formatacao de um disco com o novo sistema de arquivos
//com tamanho de blocos igual a blockSize. Retorna o numero total de
//blocos disponiveis no disco, se formatado com sucesso. Caso contrario,
//retorna -1.
int myFSFormat (Disk *d, unsigned int blockSize) {
  unsigned long total_sectors = diskGetNumSectors(d);

  sb.magic = MAGIC_NUMBER;
  sb.total_blocks = total_sectors;
  sb.free_blocks_start = DATA_AREA_START;

  unsigned char buffer[DISK_SECTORDATASIZE];
  // Zerar o buffer pra nao dar merda
  for (int i = 0; i < DISK_SECTORDATASIZE; i++) {
    buffer[i] = 0;
  }

  // Serializa os dados da struct pro buffer com o metodo q tem em util.h
  ul2char(sb.magic, &buffer[0]);
  ul2char(sb.total_blocks, &buffer[4]);
  ul2char(sb.free_blocks_start, &buffer[8]);

  if (diskWriteSector(d, SB_SECTOR, buffer) < 0) return -1;

  // Cria root dir (inode 1)
  Inode* root = inodeCreate(1, d);
  if (!root) {
    printf("MYFSFORMAT: Nao foi possivel criar o diretorio root \n");
    return -1;
  }

  inodeSetFileType(root, FILETYPE_DIR);
  inodeSetFileSize(root, 0);
  inodeSave(root);
  free(root);

  return total_sectors;
}

//Funcao para montagem/desmontagem do sistema de arquivos, se possível.
//Na montagem (x=1) e' a chance de se fazer inicializacoes, como carregar
//o superbloco na memoria. Na desmontagem (x=0), quaisquer dados pendentes
//de gravacao devem ser persistidos no disco. Retorna um positivo se a
//montagem ou desmontagem foi bem sucedida ou, caso contrario, 0.
int myFSxMount (Disk *d, int x) {
  // Montar disco
  if (x == 1) {
    unsigned char buffer[DISK_SECTORDATASIZE];
    if (diskReadSector(d, SB_SECTOR, buffer) < 0) return 0;

    char2ul(&buffer[0], &sb.magic);
    char2ul(&buffer[4], &sb.total_blocks);
    char2ul(&buffer[8], &sb.free_blocks_start);

    // Assinatura invalida, ou seja, disco nao formatado ou corrompido
    if (sb.magic != MAGIC_NUMBER) return 0;

    curr_disk = d;
    return 1;
  } else { // Unmount
    // Ao desmontar temos que salvar os trem que tavam no buffer no disco
    // TODO salvar
    curr_disk = NULL;
    return 0;
  }
}

//Funcao para abertura de um arquivo, a partir do caminho especificado
//em path, no disco montado especificado em d, no modo Read/Write,
//criando o arquivo se nao existir. Retorna um descritor de arquivo,
//em caso de sucesso. Retorna -1, caso contrario.
int myFSOpen (Disk *d, const char *path) {
	return -1;
}
	
//Funcao para a leitura de um arquivo, a partir de um descritor de arquivo
//existente. Os dados devem ser lidos a partir da posicao atual do cursor
//e copiados para buf. Terao tamanho maximo de nbytes. Ao fim, o cursor
//deve ter posicao atualizada para que a proxima operacao ocorra a partir
//do próximo byte apos o ultimo lido. Retorna o numero de bytes
//efetivamente lidos em caso de sucesso ou -1, caso contrario.
int myFSRead (int fd, char *buf, unsigned int nbytes) {
	return -1;
}

//Funcao para a escrita de um arquivo, a partir de um descritor de arquivo
//existente. Os dados de buf sao copiados para o disco a partir da posição
//atual do cursor e terao tamanho maximo de nbytes. Ao fim, o cursor deve
//ter posicao atualizada para que a proxima operacao ocorra a partir do
//proximo byte apos o ultimo escrito. Retorna o numero de bytes
//efetivamente escritos em caso de sucesso ou -1, caso contrario
int myFSWrite (int fd, const char *buf, unsigned int nbytes) {
	return -1;
}

//Funcao para fechar um arquivo, a partir de um descritor de arquivo
//existente. Retorna 0 caso bem sucedido, ou -1 caso contrario
int myFSClose (int fd) {
	return -1;
}

//Funcao para abertura de um diretorio, a partir do caminho
//especificado em path, no disco indicado por d, no modo Read/Write,
//criando o diretorio se nao existir. Retorna um descritor de arquivo,
//em caso de sucesso. Retorna -1, caso contrario.
int myFSOpenDir (Disk *d, const char *path) {
  // Validar se o path eh raiz. Usando '/' como root
  if (path[0] != '/') return -1;

  // Procura slot livre na tabela de arquivos
  int fd = -1;
  for (int i = 0; i < MAX_FDS; i++) {
    if (!open_files[i].is_used) {
      fd = i;
      break;
    }
  }

  if (fd == -1) return -1; // Tabela cheia

  // Configura descritor
  open_files[fd].is_used = 1;
  open_files[fd].inode_number = 1; // Root sempre eh 1
  open_files[fd].curr_offset_bytes = 0;

  return fd + 1;
}

//Funcao para a leitura de um diretorio, identificado por um descritor
//de arquivo existente. Os dados lidos correspondem a uma entrada de
//diretorio na posicao atual do cursor no diretorio. O nome da entrada
//e' copiado para filename, como uma string terminada em \0 (max 255+1).
//O numero do inode correspondente 'a entrada e' copiado para inumber.
//Retorna 1 se uma entrada foi lida, 0 se fim de diretorio ou -1 caso
//mal sucedido
int myFSReadDir (int fd, char *filename, unsigned int *inumber) {
	return -1;
}

//Funcao para adicionar uma entrada a um diretorio, identificado por um
//descritor de arquivo existente. A nova entrada tera' o nome indicado
//por filename e apontara' para o numero de i-node indicado por inumber.
//Retorna 0 caso bem sucedido, ou -1 caso contrario.
int myFSLink (int fd, const char *filename, unsigned int inumber) {
  // Validacao do FD, tem que ser o rootdir
  int internal_fd = fd - 1;
  if (internal_fd < 0 || internal_fd >= MAX_FDS || !open_files[internal_fd].is_used) return -1;
  if (open_files[internal_fd].inode_number != 1) return -1;

  Inode *root = inodeLoad(1, curr_disk);
  if (!root) return -1;

  unsigned char block[DISK_SECTORDATASIZE];
  unsigned int block_addr;
  unsigned int num_blocks = inodeGetFileSize(root) / DISK_SECTORDATASIZE;
  if (inodeGetFileSize(root) % DISK_SECTORDATASIZE != 0) num_blocks++;

  int saved = 0; // Flag de salvo com sucesso

  // Tenta encontrar um slot livre
  for (int i = 0; i < num_blocks; i++) {
    block_addr = inodeGetBlockAddr(root, i);
    if (block_addr == 0) continue;

    diskReadSector(curr_disk, block_addr, block);
    DirEntry *entries = (DirEntry*) block; 

    int max_entries = DISK_SECTORDATASIZE / sizeof(DirEntry);

    for (int j = 0; j < max_entries; j++) {
      if (entries[j].is_active) {
        strncpy(entries[j].filename, filename, MAX_FILENAME_LENGTH);
        entries[j].i_number = inumber;
        entries[j].is_active = 1;

        diskWriteSector(curr_disk, block_addr, block);
        saved = 1;
        break;
      }
    }
    if (saved) break;
  }

  // Se nao tem vaga, aloca um novo bloco 
  if (!saved) {
    // Numa implementacao real teriamos que usar um mapa de bits, mas aqui so iterar e achar um buraco serve
    unsigned int new_block_addr = sb.free_blocks_start;
    sb.free_blocks_start++;

    for (int i = 0; i < DISK_SECTORDATASIZE; i++) {
      // Zerando o setor pra nn ter lixo
      block[i] = 0;
    }

    DirEntry *entries = (DirEntry*) block;
    strncpy(entries[0].filename, filename, MAX_FILENAME_LENGTH);
    entries[0].i_number = inumber;
    entries[0].is_active = 1;

    if (inodeAddBlock(root, new_block_addr) == 0) {
      diskWriteSector(curr_disk, new_block_addr, block);

      // Atualiza o tamanho do dir
      unsigned new_size = (inodeGetFileSize(root) > 0) ? inodeGetFileSize(root) + DISK_SECTORDATASIZE : DISK_SECTORDATASIZE;
      inodeSetFileSize(root, new_size);
      inodeSave(root);
      saved = 1;
    }
  }
  free(root);
  return saved ? 0 : -1;
}

//Funcao para remover uma entrada existente em um diretorio, 
//identificado por um descritor de arquivo existente. A entrada e'
//identificada pelo nome indicado em filename. Retorna 0 caso bem
//sucedido, ou -1 caso contrario.
int myFSUnlink (int fd, const char *filename) {
  int internal_fd = fd - 1;
  if (internal_fd < 0 || internal_fd >= MAX_FDS || !open_files[internal_fd].is_used) return -1;

  Inode *root = inodeLoad(1, curr_disk);
  if (!root) return -1;

  unsigned char block[DISK_SECTORDATASIZE];
  unsigned int block_addr;
  unsigned int num_blocks = inodeGetFileSize(root) / DISK_SECTORDATASIZE;
  if (inodeGetFileSize(root) % DISK_SECTORDATASIZE != 0) num_blocks++;

  int found = 0;

  for (int i = 0; i < num_blocks; i++) {
    block_addr = inodeGetBlockAddr(root, i);
    if (block_addr == 0) continue;

    diskReadSector(curr_disk, block_addr, block);
    DirEntry *entries = (DirEntry*) block;
    int max_entries = DISK_SECTORDATASIZE / sizeof(DirEntry);

    for (int j = 0; j < max_entries; j++) {
      if (entries[j].is_active && strcmp(entries[0].filename, filename) == 0) {
        entries[j].is_active = 0;
        Inode* file_node = inodeLoad(entries[j].i_number, curr_disk);
        if (file_node) {
          inodeClear(file_node);
          free(file_node);
        } 
        
        diskWriteSector(curr_disk, block_addr, block);
        found = 1;
        break;
      }
      if (found) break;
    }
  }
  free(root);
  return found ? 0 : -1;
}

//Funcao para fechar um diretorio, identificado por um descritor de
//arquivo existente. Retorna 0 caso bem sucedido, ou -1 caso contrario.	
int myFSCloseDir (int fd) {
  int internal_fd = fd - 1;
  if (internal_fd < 0 || internal_fd >= MAX_FDS) return -1;
  if (!open_files[internal_fd].is_used) return -1;

  // Libera slot 
  open_files[internal_fd].is_used = 0;
  return 0;
}

//Funcao para instalar seu sistema de arquivos no S.O., registrando-o junto
//ao virtual FS (vfs). Retorna um identificador unico (slot), caso
//o sistema de arquivos tenha sido registrado com sucesso.
//Caso contrario, retorna -1
int installMyFS (void) {
  init_file_handles();
  FSInfo* fsinfo = malloc(sizeof(FSInfo));
  if (!fsinfo) {
    printf("INSTALLMYFS: Erro ao gerar fsinfo\n");
    return -1;
  }

  fsinfo->fsid = 1; // Identificador unico
  fsinfo->fsname = "FSMuitoFoda"; // Nome legivel do sistema de arquivos
  
  // Mapeando todas as funcoes anteriores pros metadados desse trem
  fsinfo->isidleFn = myFSIsIdle;
  fsinfo->formatFn = myFSFormat;
  fsinfo->xMountFn = myFSxMount;
  fsinfo->openFn = myFSOpen;
  fsinfo->readFn = myFSRead;
  fsinfo->closeFn = myFSClose;
  fsinfo->writeFn = myFSWrite;
  fsinfo->opendirFn = myFSOpenDir;
  fsinfo->readdirFn = myFSReadDir;
  fsinfo->linkFn = myFSLink;
  fsinfo->unlinkFn = myFSUnlink;
  fsinfo->closedirFn = myFSCloseDir;

  return vfsRegisterFS(fsinfo);
}
