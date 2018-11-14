#include "crossplatformicon.h"
#include <QPixmap>
#include <QDebug>

QString CrossPlatformIcon::m_fallbackThemeName = "breeze-qownnotes";
QString CrossPlatformIcon::m_themeName = "Adwaita";
CrossPlatformIcon *CrossPlatformIcon::m_IconManager = new CrossPlatformIcon;

CrossPlatformIcon *CrossPlatformIcon::IconManager()
{
	return m_IconManager;
}

void CrossPlatformIcon::destroyIconManager()
{
	delete m_IconManager;
}

QString CrossPlatformIcon::themeName()
{
	return m_themeName;
}

QString CrossPlatformIcon::fallbackThemeName()
{
	return m_fallbackThemeName;
}

void CrossPlatformIcon::setThemeName( QString themeName )
{
	m_themeName = themeName;
	emit m_IconManager->themeNameChanged( themeName );
}

void CrossPlatformIcon::setFallbackThemeName( QString fallbackThemeName )
{
	m_fallbackThemeName = fallbackThemeName;
	emit m_IconManager->fallbackThemeNameChanged( fallbackThemeName );
}

/*
 * Function get:
 * This function first checks if an icon with a theme pack exists
 * in the current theme. If it does, it returns the QIcon.
 * Otherwise, it will switch to a fallback icon theme temporarily
 * locate that
 */
QIcon CrossPlatformIcon::get(QString name, int size)
{
	// If theme icon exists, return it.
	if ( QIcon::hasThemeIcon(name) )
		return QIcon::fromTheme( name );

	// Grab icon from Vibrato Notes icon set and return it.
	QIcon::setThemeName( m_fallbackThemeName );

	// If fallback theme does not have icon either,
	// switch back to the system icon theme and
	// return a blank QIcon
	if ( !QIcon::hasThemeIcon(name) ) {
		QIcon::setThemeName( m_themeName );
		return QIcon();
	}

	// Grab the theme icon
	QIcon iconRaw = QIcon::fromTheme( name );
	QList<QSize> iconSizes = iconRaw.availableSizes();

	// Convert the icon to a pixmap.
	QIcon iconPixmap;
	if ( size > 0 )
		iconPixmap = iconRaw.pixmap( size );
	else
 		iconPixmap = iconRaw.pixmap( iconSizes[ iconSizes.length()-1 ].width() );
	QIcon icon = QIcon( iconPixmap );

	// Set the icon theme back to the system theme
	QIcon::setThemeName( m_themeName );

	return icon;
}
