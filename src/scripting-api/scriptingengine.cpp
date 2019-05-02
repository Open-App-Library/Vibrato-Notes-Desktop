#include "scriptingengine.h"

#include <QDebug>

ScriptingEngine::ScriptingEngine()
{
    CHICKEN_run(CHICKEN_default_toplevel);

    C_word res;
    CHICKEN_eval_string("(print \"Welcome to Chicken\")", &res);

    char buffer[256];
    CHICKEN_get_error_message(buffer, 255);

    qDebug() << "Ran chicken" << buffer;

}
