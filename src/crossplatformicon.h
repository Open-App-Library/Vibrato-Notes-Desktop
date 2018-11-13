#ifndef CROSSPLATFORMICON_H
#define CROSSPLATFORMICON_H

#include <QObject>
#include <QIcon>

class CrossPlatformIcon : public QObject
{
	Q_OBJECT
public:
	static CrossPlatformIcon *IconManager();
	static void destroyIconManager();

	static QString themeName();
	static QString fallbackThemeName();
	static QIcon get(QString name, int size=-1);

public slots:
	static void setThemeName( QString themeName );
	static void setFallbackThemeName( QString fallbackThemeName );

signals:
	void themeNameChanged(QString themeName);
	void fallbackThemeNameChanged(QString fallbackThemeName);

private:
	static QString m_fallbackThemeName;
	static QString m_themeName;
	static CrossPlatformIcon *m_IconManager;
};

#endif // CROSSPLATFORMICON_H
