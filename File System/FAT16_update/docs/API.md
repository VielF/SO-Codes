# APIs Disponíveis no Projeto

## Entrada & Saída

```c
int read_bytes(FILE* fp, unsigned int address, void* buf, unsigned int count)
```

Esta função lê `count` bytes do arquivo `fp` no endereço `address` ao buffer `buf`.
Ela pode ser usada para ler da imagem de disco.

Em sucesso, retorna RB_OK. Em falha, retorna RB_ERROR.

---

```c
int fseek(FILE* fp, long offset, int whence)
```

Esta função é usada selecionar onde em um arquivo se deseja ler/escrever. O `read_bytes()`
a chama internamente.

Ela seta a posição de `fp` para `offset` quando `whence == SEEK_SET`.

Para mas informações, execute em um sistema Linux: `man fseek`.

---

```c
size_t fwrite(void* data, size_t size, size_t nmemb, FILE* fp)
```

Esta função é usada para escrever `data` no arquivo `fp`. Ela escreve `nmemb`
elementos de tamanho `size`, o que significa que se `nmemb > 1`, `data` deve ser um array.

Para mas informações, execute em um sistema Linux: `man fwrite`.

---

## FAT16

```c
uint32_t bpb_faddress(struct fat_bpb* bpb);
```

Esta função lê, do `bpb`, o endereço em disco da tabela FAT.

---

```c
uint32_t bpb_froot_addr(struct fat_bpb* bpb);
```

Esta função lê, do `bpb`, o endereço em disco do diretório raiz.

---

```c
uint32_t bpb_fdata_addr(struct fat_bpb *);
```

Esta função lê, do `bpb`, o endereço em disco da região de dados.

## Auxiliares

```c
bool cstr_to_fat16wnull(char *filename, char output[FAT16STR_SIZE_WNULL]);
```

Esta função converte o nome de arquivo `filename` do formato Cstring para o formato
usado internamente pelo FAT16.

Ela retorna `true` se falhar e `false` se tiver sucesso.

---

```c
struct far_dir_searchres
{
	struct fat_dir fdir; // Diretório encontrado
	bool          found; // Encontrou algo?
	int             idx; // Index relativo ao diretório de busca
};

struct far_dir_searchres find_in_root(struct fat_dir *dirs, char *filename, struct fat_bpb *bpb);
```

Esta função procura um arquivo pelo nome `filename` no diretório `dirs`. Preferencialmente, `dirs` deve
ser um ponteiro para uma array de `struct fat_dir`, enquante `filename` necessita estar no formato
de string do FAT16. Passar o `bpb` também é necessário.

No `far_dir_searchres`, `fdir` é a `struct fat_dir` encontrada, `idx` é o seu index relativo à `dirs`,
e `found` é uma boolean sentinela que diz se foi encontrado algo ou não.

Note que esta função só foi testada com o diretório raiz, e muito provavelmente não funcionará com
subdiretórios.

# Observações

Obviamente, todas as APIs nativas do C estão disponíveis. Algumas funções extras estão documentadas
somente em código; não se limite a usar somente o que foi citado aqui.
