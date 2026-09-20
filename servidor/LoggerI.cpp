//
// Implementação do Logger (servant)
// Programação Distribuída - Escola Politécnica - PUCPR
//

#include "LoggerI.h"
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

// Converte a severidade para texto (para impressão)
static const char* nomeSeveridade (Severidade s)
{
    switch (s) {
        case DEBUG:    return "DEBUG";
        case WARNING:  return "WARNING";
        case ERROR:    return "ERROR";
        case CRITICAL: return "CRITICAL";
        default:       return "???";
    }
}

Logger_i::Logger_i ()
{
}

Logger_i::~Logger_i ()
{
}

void Logger_i::log (
    ::Severidade severidade,
    const std::string endereco,
    ::CORBA::UShort pid,
    ::CORBA::ULongLong hora,
    const std::string msg)
{
    // formata a hora (segundos desde 1/1/1970) em data legível
    time_t t = static_cast<time_t>(hora);
    char data[32];
    strftime(data, sizeof(data), "%d/%m/%Y %H:%M:%S", localtime(&t));

    cout << "[" << data << "] "
         << left << setw(8) << nomeSeveridade(severidade) << " "
         << endereco << " (pid " << pid << "): "
         << msg << endl;

    // guarda o endereço do último evento dessa severidade
    ultimoEndereco[severidade] = endereco;
}

std::string Logger_i::locate (::Severidade s)
{
    cout << "locate(" << nomeSeveridade(s) << ")\n";

    auto it = ultimoEndereco.find(s);
    if (it == ultimoEndereco.end())
        throw EventoNaoEncontrado();

    return it->second;
}
