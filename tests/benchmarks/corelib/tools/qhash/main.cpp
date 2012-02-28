/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "main.h"

#include <QFile>
#include <QHash>
#include <QString>
#include <QStringList>
#include <QUuid>
#include <QTest>


class tst_QHash : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void qhash_qt4_data() { data(); }
    void qhash_qt4();
    void qhash_faster_data() { data(); }
    void qhash_faster();
    void javaString_data() { data(); }
    void javaString();

private:
    void data();

    QStringList smallFilePaths;
    QStringList uuids;
    QStringList dict;
    QStringList numbers;
};

///////////////////// QHash /////////////////////

#include <QDebug>

void tst_QHash::initTestCase()
{
    // small list of file paths
    QFile smallPathsData("paths_small_data.txt");
    QVERIFY(smallPathsData.open(QIODevice::ReadOnly));
    smallFilePaths = QString::fromLatin1(smallPathsData.readAll()).split(QLatin1Char('\n'));
    QVERIFY(!smallFilePaths.isEmpty());

    // list of UUIDs
    // guaranteed to be completely random, generated by http://xkcd.com/221/
    QUuid ns = QUuid("{f43d2ef3-2fe9-4563-a6f5-5a0100c2d699}");
    uuids.reserve(smallFilePaths.size());

    foreach (const QString &path, smallFilePaths)
        uuids.append(QUuid::createUuidV5(ns, path).toString());


    // lots of strings with alphabetical characters, vaguely reminiscent of
    // a dictionary.
    //
    // this programatically generates a series like:
    //  AAAAAA
    //  AAAAAB
    //  AAAAAC
    //  ...
    //  AAAAAZ
    //  AAAABZ
    //  ...
    //  AAAAZZ
    //  AAABZZ
    QByteArray id("AAAAAAA");

    if (dict.isEmpty()) {
        for (int i = id.length() - 1; i > 0;) {
            dict.append(id);
            char c = id.at(i);
            id[i] = ++c;

            if (c == 'Z') {
                // wrap to next digit
                i--;
                id[i] = 'A';
            }
        }
    }

    // string versions of numbers.
    for (int i = 5000000; i < 5005001; ++i)
        numbers.append(QString::number(i));
}

void tst_QHash::data()
{
    QTest::addColumn<QStringList>("items");
    QTest::newRow("paths-small") << smallFilePaths;
    QTest::newRow("uuids-list") << uuids;
    QTest::newRow("dictionary") << dict;
    QTest::newRow("numbers") << numbers;
}

void tst_QHash::qhash_qt4()
{
    QFETCH(QStringList, items);
    QHash<Qt4String, int> hash;

    QList<Qt4String> realitems;
    foreach (const QString &s, items)
        realitems.append(s);

    QBENCHMARK {
        for (int i = 0, n = realitems.size(); i != n; ++i) {
            hash[realitems.at(i)] = i;
        }
    }
}

void tst_QHash::qhash_faster()
{
    QFETCH(QStringList, items);
    QHash<String, int> hash;

    QList<String> realitems;
    foreach (const QString &s, items)
        realitems.append(s);

    QBENCHMARK {
        for (int i = 0, n = realitems.size(); i != n; ++i) {
            hash[realitems.at(i)] = i;
        }
    }
}

void tst_QHash::javaString()
{
    QFETCH(QStringList, items);
    QHash<JavaString, int> hash;

    QList<JavaString> realitems;
    foreach (const QString &s, items)
        realitems.append(s);

    QBENCHMARK {
        for (int i = 0, n = realitems.size(); i != n; ++i) {
            hash[realitems.at(i)] = i;
        }
    }
}


QTEST_MAIN(tst_QHash)

#include "main.moc"
