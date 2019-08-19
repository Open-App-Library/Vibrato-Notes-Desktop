/*
 * Handy little QIcon-related
 * functions
 */

#ifndef ICONUTILS_H
#define ICONUTILS_H
#include <QIcon>

class IconUtils
{
public:
    static QIcon requestDarkIcon( QString iconName, int size=16  );
};

#endif // ICONUTILS_H
