#ifndef PAGEDJOB_HPP
#define PAGEDJOB_HPP
#include <QString>
#include <QList>

namespace Core::Tools
{

template<typename TRequest, typename TItem>
class PagedJob
{
private:

    static constexpr int kMaxPages = 200;

    TRequest m_request{};         // исходный запрос; m_cursor меняется от страницы к странице
    QList<TItem> m_items;         // накопленные результаты всех страниц
    QString m_lastCursor;         // курсор, с которым запрошена последняя страница
    int m_pages = 0;              // сколько страниц уже получено
    bool m_truncated = false;     // true — остановились по kMaxPages, данные могут быть неполными

public:

    void setRequest(const TRequest& req)
    {
        m_request = req;
        m_request.m_cursor.reset();
    }
    TRequest getRequest() { return m_request; }
    QList<TItem> get() { return m_items; }
    bool wasTruncated() { return m_truncated; }

    // Учитывает пришедшую страницу
    // Возвращает true, если нужно запросить следующую (request.m_cursor уже обновлён)
    bool addPage(const QList<TItem>& pageItems, const QString& nextCursor)
    {
        m_items += pageItems;
        ++m_pages;

        if (nextCursor.isEmpty() || pageItems.isEmpty() || nextCursor == m_lastCursor)
            return false;

        if (m_pages >= kMaxPages)
        {
            m_truncated = true;
            return false;
        }

        m_lastCursor = nextCursor;
        m_request.m_cursor = nextCursor;
        return true;
    }


};

}

#endif // PAGEDJOB_HPP
