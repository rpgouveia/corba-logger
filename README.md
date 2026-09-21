# LOGGER
 
### SISTEMA DISTRIBUÍDO CLIENTE/SERVIDOR CORBA
 
*Trabalho 1 — Sistemas Distribuídos — Escola Politécnica — PUCPR*
 
O servidor (**Logger**) mantém o registro de eventos que ocorrem em clientes
espalhados na rede. Os clientes enviam eventos por meio da operação assíncrona
`log()` e consultam o endereço do último evento de uma severidade com `locate()`.
 
### Interface (`idl/Logger.idl`)
 
* `oneway void log(severidade, endereco, pid, hora, msg)` — registra um evento
  (impresso na tela pelo Logger)
    * `severidade`: `DEBUG`, `WARNING`, `ERROR` ou `CRITICAL` (enum `Severidade`)
    * `endereco`: string no formato `"ip:porta"`
    * `pid`: identificador do processo (`unsigned short`)
    * `hora`: segundos desde 00:00:00 de 1/1/1970 (`unsigned long long`)
    * `msg`: descrição do evento
* `string locate(severidade)` — retorna o endereço do último evento recebido com
  essa severidade; lança `EventoNaoEncontrado` se ainda não houve nenhum
### Diretórios:
 
* `idl`: arquivo IDL + *stub* + *skeleton*
* `bin`: executáveis (os resultados das compilações são gerados aqui)
* `cliente`: cliente de teste (envia eventos fictícios e testa toda a interface)
* `servidor`: servidor Logger (`LoggerI.cpp` contém a implementação)
### Para gerar stub e skeleton, digite:
 
```bash
$ cd idl
$ tao_idl -Gstl Logger.idl
```
 
(`-Gstl`: para usar biblioteca padrão C++)
 
### Para compilar, digite:
 
```bash
$ cd bin                # compila cliente e servidor
$ make
```
 
ou, separadamente:
 
```bash
$ cd cliente            # (ou: cd servidor)
$ make
```
 
### Para executar:
 
O servidor publica a IOR do Logger no **Servidor de Nomes**, que deve estar
rodando antes. Cada comando em um terminal separado:
 
```bash
# 1. Servidor de Nomes (tao_cosnaming ou Naming_Service, conforme a versão do TAO)
$ tao_cosnaming -ORBEndpoint iiop://localhost:2809
 
# 2. Servidor Logger
$ cd bin
$ ./servidor_logger Logger -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService
 
# 3. Cliente
$ cd bin
$ ./cliente_logger Logger -ORBInitRef NameService=corbaloc:iiop:localhost:2809/NameService
```
 
Para encerrar o servidor, use `Ctrl+C`.
 
### Saída esperada:
 
No terminal do Logger:
 
```
Logger aguardando eventos...
[21/09/2026 00:26:33] DEBUG    192.168.1.1:1500 (pid 1001): conexao iniciada
[21/09/2026 00:26:38] WARNING  192.168.1.2:1600 (pid 2002): uso de memoria acima de 80%
[21/09/2026 00:26:43] ERROR    192.168.1.3:1500 (pid 3003): falha ao abrir arquivo
[21/09/2026 00:26:48] ERROR    192.168.1.2:1600 (pid 2002): timeout na conexao
locate(DEBUG)
locate(WARNING)
locate(ERROR)
locate(CRITICAL)
```
 
No terminal do cliente:
 
```
4 eventos enviados.
 
locate(DEBUG) = 192.168.1.1:1500
locate(WARNING) = 192.168.1.2:1600
locate(ERROR) = 192.168.1.2:1600
locate(CRITICAL): nenhum evento com essa severidade (excecao)
```
 
Repare que `locate(ERROR)` devolve o endereço do **último** evento dessa
severidade, e que `locate(CRITICAL)` lança a exceção, porque nenhum evento
CRITICAL foi enviado.
 
### Para limpar arquivos intermediários:
 
```bash
$ cd cliente            # (ou: cd servidor)
$ make clean            # objetos e executável
$ make cleanall         # também stubs/skeletons gerados
```
