/************************************************************************
**
** Authors:   Nils Schimmelmann <nschimme@gmail.com> (Jahara)
**
** This file is part of the MMapper project.
** Maintained by Nils Schimmelmann <nschimme@gmail.com>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the:
** Free Software Foundation, Inc.
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
************************************************************************/

#ifndef _REMOTEEDITSESSION_H_
#define _REMOTEEDITSESSION_H_

#include <QObject>
#include <QPointer>

class RemoteEdit;
class RemoteEditProcess;
class RemoteEditWidget;

#define REMOTE_EDIT_VIEW_KEY -1

class RemoteEditSession : public QObject
{
    Q_OBJECT

    friend class RemoteEditExternalSession;
    friend class RemoteEditInternalSession;

public:
    RemoteEditSession(uint sessionId, int key, RemoteEdit *remoteEdit);

    int getId() const { return m_sessionId; }

    int getKey() const { return m_key; }

    const bool isEditSession() const { return m_key != REMOTE_EDIT_VIEW_KEY; }

    const QString &getContent() const { return m_content; }

    void setContent(QString content) { m_content = std::move(content); }

    void cancel();
    void save();

protected slots:
    void onCancel() { cancel(); }
    void onSave(const QString &content)
    {
        setContent(content);
        save();
    }

private:
    const uint m_sessionId;
    const int m_key;
    RemoteEdit *m_manager;

    QString m_content{};
};

class RemoteEditInternalSession : public RemoteEditSession
{
    Q_OBJECT
public:
    RemoteEditInternalSession(
        uint sessionId, int key, const QString &title, const QString &body, RemoteEdit *remoteEdit);
    ~RemoteEditInternalSession();

private:
    QPointer<RemoteEditWidget> m_widget;
};

class RemoteEditExternalSession : public RemoteEditSession
{
    Q_OBJECT
public:
    RemoteEditExternalSession(
        uint sessionId, int key, const QString &title, const QString &body, RemoteEdit *remoteEdit);
    ~RemoteEditExternalSession();

private:
    QPointer<RemoteEditProcess> m_process;
};

#endif /* _REMOTEEDITSESSION_H_ */