//
// SERVIDOR LOGGER
// Programação Distribuída - Escola Politécnica - PUCPR
//
// USANDO SERVIDOR DE NOMES

#include <iostream>
#include <string>
#include "LoggerI.h"
#include <orbsvcs/CosNamingC.h>

using namespace std;
using namespace CORBA;
using namespace PortableServer;
using namespace CosNaming;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "USO: " << argv[0] << " <nome_do_servidor>\n";
        return 1;
    }

    try {
        // 1. Inicia ORB
        ORB_var orb = ORB_init(argc, argv, "ORB");

        // 2. Ativa RootPOA
        Object_var tmp = orb->resolve_initial_references("RootPOA");
        POA_var poa = POA::_narrow(tmp.in());
        POAManager_var ger = poa->the_POAManager();
        ger->activate();

        // 3. Instancia "servant"
        Logger_i li;

        // 4. Registra servant no POA, criando o objeto distribuído
        Logger_var logger = li._this();

        // 5. Publica IOR no servidor de nomes
        tmp = orb->resolve_initial_references("NameService");
        NamingContext_var ns = NamingContext::_narrow(tmp.in());

        Name nome(1);
        nome.length(1);
        nome[0].id = string_dup(argv[1]);

        ns->rebind(nome, logger.in());

        // 6. Aguarda requisições
        cout << "Logger aguardando eventos...\n";
        orb->run();

        // 7. Finalizações
        poa->destroy(true, true);
        orb->destroy();
    } catch (const CORBA::Exception& e) {
        cerr << "Erro CORBA: " << e << endl;
    }
    return 0;
}
