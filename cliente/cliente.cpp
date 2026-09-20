//
// CLIENTE DO LOGGER
// Programação Distribuída - Escola Politécnica - PUCPR
//

#include <iostream>
#include <string>
#include <ctime>
#include <LoggerC.h>
#include <orbsvcs/CosNamingC.h>

using namespace std;
using namespace CORBA;
using namespace CosNaming;

static const char* nomeSeveridade(Severidade s)
{
    switch (s) {
        case DEBUG:    return "DEBUG";
        case WARNING:  return "WARNING";
        case ERROR:    return "ERROR";
        case CRITICAL: return "CRITICAL";
        default:       return "???";
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "USO: " << argv[0] << " <nome_do_servidor>\n";
        return 1;
    }

    try {
        // 1. Inicializa ORB
        ORB_var orb = ORB_init(argc, argv, "ORB");

        // 2. Obtém referência do Logger no servidor de nomes
        Object_var tmp = orb->resolve_initial_references("NameService");
        NamingContext_var ns = NamingContext::_narrow(tmp.in());

        Name nome(1);
        nome.length(1);
        nome[0].id = string_dup(argv[1]);

        tmp = ns->resolve(nome);
        Logger_var logger = Logger::_narrow(tmp.in());

        // 3. Envia eventos fictícios com log()
        ULongLong agora = static_cast<ULongLong>(time(nullptr));

        logger->log(DEBUG,   "192.168.1.1:1500", 1001, agora,      "conexao iniciada");
        logger->log(WARNING, "192.168.1.2:1600", 2002, agora + 5,  "uso de memoria acima de 80%");
        logger->log(ERROR,   "192.168.1.3:1500", 3003, agora + 10, "falha ao abrir arquivo");
        logger->log(ERROR,   "192.168.1.2:1600", 2002, agora + 15, "timeout na conexao");
        // nenhum evento CRITICAL: locate(CRITICAL) deve lançar exceção
        cout << "4 eventos enviados.\n\n";

        // 4. Testa locate() para todas as severidades
        Severidade todas[] = { DEBUG, WARNING, ERROR, CRITICAL };
        for (Severidade s : todas) {
            try {
                string end = logger->locate(s);
                cout << "locate(" << nomeSeveridade(s) << ") = " << end << endl;
            } catch (const EventoNaoEncontrado&) {
                cout << "locate(" << nomeSeveridade(s)
                     << "): nenhum evento com essa severidade (excecao)\n";
            }
        }

        // 5. Finalizações
        orb->destroy();
    } catch (const CORBA::Exception& e) {
        cerr << "Erro CORBA: " << e << endl;
    }
    return 0;
}
