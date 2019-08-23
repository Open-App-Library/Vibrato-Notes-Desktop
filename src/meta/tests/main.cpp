#include <QDebug>
#include <QMap>
#include "../tag.h"
#include "../notebook.h"
#include "../note.h"

void loopFields(VibratoObject *obj) {
    QVector<QString> fieldKeys = obj->fieldKeys();
    VibratoObjectMap fields = obj->fields();


    for (int i=0; i<fieldKeys.length(); i++) {
        qDebug() << fieldKeys[i] << fields[fieldKeys[i]];
    }
}

int main() {

    /*
     * Tag Test
     */
    QMap<QString, QVariant> map;
    map["title"] = "EPIC TAG";
    Tag *t = new Tag(map);
    qDebug("Tag Example:");
    loopFields(t);
    delete t;

    qDebug("=============================================");

    /*
     * Notebook Test
     */
    Notebook *nb = new Notebook();
    qDebug("Notebook Example:");
    loopFields(nb);
    delete nb;

    qDebug("=============================================");
    Note *n = new Note(nullptr);
    qDebug("Note Example:");
    loopFields(n);
    delete n;

}
