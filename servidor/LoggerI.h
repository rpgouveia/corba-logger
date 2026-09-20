#ifndef LOGGERI_H_
#define LOGGERI_H_

#include "LoggerS.h"
#include <map>
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Logger_i : public virtual POA_Logger {
private:
    // último endereço recebido para cada severidade
    std::map<Severidade, std::string> ultimoEndereco;
public:
    Logger_i ();
    virtual ~Logger_i ();

    virtual void log (
        ::Severidade severidade,
        const std::string endereco,
        ::CORBA::UShort pid,
        ::CORBA::ULongLong hora,
        const std::string msg);

    virtual std::string locate (::Severidade s);
};

#endif /* LOGGERI_H_ */
