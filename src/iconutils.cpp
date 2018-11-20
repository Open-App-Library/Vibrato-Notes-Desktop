/*
 * Handy little QIcon-related
 * functions
 */

#include "iconutils.h"
#include <QDebug>

QIcon IconUtils::requestDarkIcon( QString iconName, int size )
{
    QString curTheme = QIcon::themeName();
    QIcon::setThemeName( curTheme + "-dark" );

    QIcon rawIcon = QIcon::fromTheme( iconName );
    QPixmap map = rawIcon.pixmap(QSize( size,size ));
    QIcon newIconFromPixmap(map);

    QIcon::setThemeName( curTheme );

    return newIconFromPixmap;
}
