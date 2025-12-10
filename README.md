# myFS
Trabalho de disciplina de Sistemas Operacionais - File System teórico

## Enunciado
Dado um simulador que permite operar sobre um sistema de arquivos de um S.O. hipotético, assim como a API de tal S.O. para operações com discos, i-nodes e sistemas de arquivos (de forma genérica), implemente seu próprio sistema de arquivos. **O seu sistema de arquivos não precisa permitir a criação de subdiretórios, ou seja, ele possui apenas o diretório raiz**. Todo o código-fonte encontra-se escrito em linguagem C e é composto pelos seguintes arquivos:
```text
util.h – Arquivo de cabeçalho para declaração de funções úteis de conversão de valores
util.c – Arquivo que implementa funções de conversão entre inteiros (uint) e bytes (char)
disk.h – Arquivo de cabeçalho para a declaração da API de acesso e manutenção a discos
disk.c – Arquivo que implementa a API de acesso e manutenção a discos
inode.h – Arquivo de cabeçalho para a declaração da API de manutenção de i-nodes
inode.c – Arquivo com a implementação da API de manutenção de i-nodes
vfs.h – Arquivo de cabeçalho para a declaração da API genérica de sistemas de arquivos
vfs.c – Arquivo que implementa a API genérica de sistemas de arquivos
main.c – Arquivo do programa principal do simulador
myfs.h – Arquivo de cabeçalho para a declaração da função de instalação de seu sistema de arquivos
myfs.c – Arquivo que implementa toda a API de seu sistema de arquivos
```

Todo o seu desenvolvimento deve estar contido apenas no arquivo `myfs.c`, fazendo uso apenas das APIs disponibilizadas nos arquivos de cabeçalho, além é claro, da API padrão C. Toda a descrição das APIs está contida nos respectivos arquivos de cabeçalho. As funções a serem implementadas possuem explicação em `myfs.c`:
```c
int myFSIsIdle (Disk *d);
int myFSFormat (Disk *d, unsigned int blockSize);
int myFSxMount (Disk *d, int x);
int myFSOpen (Disk *d, const char *path);
int myFSRead (int fd, char *buf, unsigned int nbytes);
int myFSWrite (int fd, const char *buf, unsigned int nbytes);
int myFSClose (int fd);
int myFSOpenDir (Disk *d, const char *path);
int myFSReadDir (int fd, char *filename, unsigned int *inumber);
int myFSLink (int fd, const char *filename, unsigned int inumber);
int myFSUnlink (int fd, const char *filename);
int myFSCloseDir (int fd);
int installMyFS (void);
```
Quaisquer funções auxiliares não podem ser externalizadas em novos arquivos de cabeçalho. Identificadores, tipos e assinaturas de função não podem ser modificados. Quaisquer funções necessárias e não implementadas nas APIs de disco, de i-node e de sistema de arquivos devem ser discutidas e solicitadas ao professor por e-mail. O mesmo vale para possíveis erros detectados.
