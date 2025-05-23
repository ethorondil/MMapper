#pragma once
// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (C) 2019 The MMapper Authors
// Author: Ulf Hermann <ulfonk_mennhar@gmx.de> (Alve)
// Author: Marek Krejza <krejza@gmail.com> (Caligor)

#include "../map/parseevent.h"
#include "pathmachine.h"

#include <QString>
#include <QtCore>

class Configuration;
class MapFrontend;
class ParseEvent;
class QElapsedTimer;
class QObject;

/**
@author alve,,,
*/
class NODISCARD_QOBJECT Mmapper2PathMachine final : public PathMachine
{
    Q_OBJECT

private:
    QElapsedTimer m_time;

public:
    explicit Mmapper2PathMachine(MapFrontend &map, QObject *parent);

signals:
    void sig_log(const QString &, const QString &);

public slots:
    void slot_handleParseEvent(const SigParseEvent &);
};
