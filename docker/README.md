# Ambiente ACE+TAO em Docker

Imagem com ACE 8.0.7 / TAO 4.0.7 compilados do fonte — mesma linha de versão
usada no laboratório da universidade.

Há dois Dockerfiles:

| Arquivo | Em disco | Conteúdo | Observação |
|---|---|---|---|
| `Dockerfile.slim` | 556 MB | 131 MB | **recomendado** |
| `Dockerfile` | 3,32 GB | 644 MB | build simples, em um estágio só |

O `Dockerfile.slim` usa build multi-stage: um estágio compila e o outro recebe
apenas as bibliotecas, os headers, o `tao_idl` e o `tao_cosnaming`. Além disso
compila com `debug=0` (o padrão do ACE em Linux é `debug=1`) e, no orbsvcs, usa
o alvo `CosNaming_Serv`, que constrói só a cadeia do Servidor de Nomes em vez de
todos os serviços CORBA.
 
## 1. Construir a imagem

Da raiz do projeto — repare que o `-f` é relativo ao diretório atual:

```bash
docker build -f docker/Dockerfile.slim -t tao:8.0.7-slim .
```

## 2. Subir o container com o projeto montado

```bash
docker run -it --rm --name tao -v "$PWD":/projeto -u "$(id -u):$(id -g)" tao:8.0.7-slim
```

* `--name tao`: sem isso o Docker sorteia um nome e o `docker exec` do passo 4
  não encontra o container. Para descobrir o nome de um container já em
  execução, use `docker ps`.
* `-u`: faz os arquivos gerados (`.o`, stubs, executáveis) nascerem com o seu
  usuário no host, e não com root.

## 3. Compilar

Dentro do container:

```bash
cd bin && make
```

Se a pasta já tiver artefatos de outra imagem, limpe antes, senão pode dar erro
de link:

```bash
cd /projeto/cliente  && make cleanall
cd /projeto/servidor && make cleanall
```

O `make` só funciona dentro do container: no host não existe `ACE_ROOT`, e o
`include` da primeira linha dos Makefiles falha.

## 4. Abrir os terminais extras

A execução usa três processos, cada um em um terminal. Nos terminais 2 e 3,
entre no mesmo container:

```bash
docker exec -it tao bash
```

O `docker exec` cai em `/projeto`, então faça `cd bin` antes de chamar os
executáveis.

## 5. Terminal 1 — Servidor de Nomes

Precisa subir antes do Logger:

```bash
tao_cosnaming -ORBEndpoint iiop://localhost:2809
```

## 6. Terminal 2 — Logger (servidor)

```bash
./servidor_logger Logger
```

## 7. Terminal 3 — Cliente

```bash
./cliente_logger Logger
```

A imagem já define `NameServiceIOR=corbaloc:iiop:localhost:2809/NameService`,
a mesma variável configurada no laboratório da universidade. O TAO a lê na
inicialização como referência padrão do `NameService`, o que dispensa o
`-ORBInitRef` na linha de comando. Se preferir ser explícito, ou apontar para
outro Servidor de Nomes, o argumento continua valendo e tem precedência:

```bash
./servidor_logger Logger \
    -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService
```

---

**Observações**

* As portas ficam dentro do container, então nada precisa ser publicado no host.
* O `--rm` do passo 2 apaga o container ao sair dele, e os terminais abertos com
  `docker exec` morrem junto. Os binários e stubs sobrevivem, porque estão no
  volume montado. Se o terminal 2 for fechado, o Logger cai e o cliente passa a
  dar erro de CORBA — basta subi-lo de novo.
* O que está em `$ACE_ROOT/lib` são links simbólicos relativos para o diretório
  onde cada biblioteca foi compilada. Se você editar a lista de diretórios
  copiados no `Dockerfile.slim`, algum link pode ficar pendente; a checagem com
  `find -xtype l` e o `tao_idl -V` no fim do build existem para pegar isso.