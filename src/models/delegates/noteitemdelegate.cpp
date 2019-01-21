#include "noteitemdelegate.h"
#include <QPainter>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QAbstractItemModel>
#include "../items/notelistitem.h"

NoteItemDelegate::NoteItemDelegate(QListView *view, QSortFilterProxyModel *proxyModel) :
    m_view(view),
    m_proxyModel(proxyModel)
{

}

QRect NoteItemDelegate::getStarRect(const QStyleOptionViewItem &option) const
{
    return QRect(option.rect.x()+option.rect.width()-35,
                 option.rect.y()+option.rect.height()/2-12,
                 25,25);
}


bool NoteItemDelegate::editorEvent(QEvent *event,
                             QAbstractItemModel *model,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            QRect checkboxRect = getStarRect(option);
            QPoint mousePoint = mouseEvent->pos();
            if ( checkboxRect.contains(mousePoint) ) {
                QModelIndex realIndex = m_proxyModel->mapToSource(index);
                NoteListItem *item = static_cast<NoteListItem*>( realIndex.internalPointer() );
                item->note()->setFavorited( !item->note()->favorited() );
                return true;
            }
            else
                m_view->setCurrentIndex(index);
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void NoteItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 0 ) {
        QModelIndex realIndex = m_proxyModel->mapToSource(index);
        NoteListItem *item = static_cast<NoteListItem*>( realIndex.internalPointer() );

        // Set the background color
        QBrush background =
                (option.state & QStyle::State_Selected) ?
                    option.palette.highlight() :
                    option.palette.base();
        painter->fillRect(option.rect, background);

        if (option.state & QStyle::State_Selected)
            painter->setPen(option.palette.highlightedText().color());
        else
            painter->setPen(option.palette.text().color());

        QRect titleRect = option.rect;
        titleRect.setX(titleRect.x()+5);
        titleRect.setWidth(titleRect.x()-5);

        QFont font=painter->font() ;
        font.setPointSize(10);

        // Title
        font.setWeight(QFont::Bold);
        painter->setFont(font);
        painter->drawText(QPoint(option.rect.x()+10, option.rect.y()+23), item->note()->title());

        // Date
        font.setWeight(QFont::Normal);
        painter->setFont(font);
        painter->drawText(QPoint(option.rect.x()+10, option.rect.y()+43), item->note()->date_created_str());


        // The Excerpt
        QRect excerptRect = option.rect;
        excerptRect.setX( excerptRect.x()+10 );
        excerptRect.setY( excerptRect.y()+50 );
        excerptRect.setWidth( excerptRect.width()-50 );

        QString excerpt = item->note()->text();
        if (excerpt.length() > 50) {
          excerpt = excerpt.mid(0, 50) + "...";
        }
        excerpt.replace("\n", " ");
        painter->drawText(excerptRect, excerpt);

        painter->fillRect(QRect(
                              option.rect.x() + option.rect.width() - 60,
                              option.rect.y(),
                              60,
                              option.rect.height()), background);

        QIcon favoriteIcon;
        if (item->note()->favorited())
            favoriteIcon = QIcon::fromTheme("vibrato-draw-star-solid");
        else
            favoriteIcon = QIcon::fromTheme("vibrato-draw-star");

        favoriteIcon.paint(painter, getStarRect(option));

    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(200,100);
}
