/*
    Copyright (c) 2006 Volker Krause <vkrause@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <qtest.h>

#include "rfc2047test.h"

#include "../src/kcodecs.h"

using namespace KCodecs;

QTEST_MAIN(RFC2047Test)

void RFC2047Test::testRFC2047decode_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QByteArray>("expectedCharset");
    QTest::addColumn<QByteArray>("defaultCharset");
    QTest::addColumn<bool>("forceCharset");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("empty") << QByteArray()
                           << QByteArray() << QByteArray("utf-8") << false
                           << QString();
    QTest::newRow("identity") << QByteArray("bla")
                              << QByteArray() << QByteArray("utf-8") << false
                              << QString::fromLatin1("bla");

    QTest::newRow("utf-8") << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?= <kloecker@kde.org>")
                           << QByteArray("UTF-8") << QByteArray("utf-8") << false
                           << QString::fromUtf8("Ingo Klöcker <kloecker@kde.org>");
    QTest::newRow("utf-8") << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?= <kloecker@kde.org>")
                           << QByteArray("UTF-8") << QByteArray("iso8859-1") << false
                           << QString::fromUtf8("Ingo Klöcker <kloecker@kde.org>");
    QTest::newRow("utf-8") << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?=")
                           << QByteArray("UTF-8") << QByteArray("utf-8") << false
                           << QString::fromUtf8("Ingo Klöcker");


    QTest::newRow("whitespaces") << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?=       =?utf-8?q?Ingo=20Kl=C3=B6cker?=")
                                 << QByteArray("UTF-8") << QByteArray("utf-8") << false
                                 << QString::fromUtf8("Ingo KlöckerIngo Klöcker");
    QTest::newRow("whitespaces") << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?=  foo  =?utf-8?q?Ingo=20Kl=C3=B6cker?=")
                                 << QByteArray("UTF-8") << QByteArray("utf-8") << false
                                 << QString::fromUtf8("Ingo Klöcker  foo  Ingo Klöcker");

    QTest::newRow("iso-8859-1") << QByteArray("=?ISO-8859-1?Q?Andr=E9s_Ot=F3n?=")
                                << QByteArray("ISO-8859-1") << QByteArray("utf-8") << false
                                << QString::fromUtf8("Andrés Otón");
    QTest::newRow("iso-8859-2") << QByteArray("=?iso-8859-2?q?Rafa=B3_Rzepecki?=")
                                << QByteArray("ISO-8859-2") << QByteArray("utf-8") << false
                                << QString::fromUtf8("Rafał Rzepecki");
    QTest::newRow("iso-8859-9") << QByteArray("=?iso-8859-9?Q?S=2E=C7a=F0lar?= Onur")
                                << QByteArray("ISO-8859-9") << QByteArray("utf-8") << false
                                << QString::fromUtf8("S.Çağlar Onur");
    QTest::newRow("iso-8859-15") << QByteArray("Rafael =?iso-8859-15?q?Rodr=EDguez?=")
                                 << QByteArray("ISO-8859-15") << QByteArray("utf-8") << false
                                 << QString::fromUtf8("Rafael Rodríguez");

    QTest::newRow("wrong charset") << QByteArray("=?iso-8859-1?q?Ingo=20Kl=C3=B6cker?=")
                                   << QByteArray("UTF-8") << QByteArray("utf-8") << true
                                   << QString::fromUtf8("Ingo Klöcker");

    // language parameter according to RFC 2231, section 5
    QTest::newRow("RFC-2331") << QByteArray("From: =?US-ASCII*EN?Q?Keith_Moore?= <moore@cs.utk.edu>")
                              << QByteArray("US-ASCII") << QByteArray("utf-8") << false
                              << QString::fromUtf8("From: Keith Moore <moore@cs.utk.edu>");

    QTest::newRow("broken QP") << QByteArray("Subject: =?iso-8859-1?Q?Belangrijk=3a=20Verhuizing=20FTP=20server?=")
                               << QByteArray("ISO-8859-1") << QByteArray("utf-8") << false
                               << QString::fromUtf8("Subject: Belangrijk: Verhuizing FTP server");

    // mixed charsets, based on bug 125542 but pasted from above instead since I'm unable to enter those asian symbols
    QTest::newRow("mixed charsets") << QByteArray("Subject: =?utf-8?q?Ingo=20Kl=C3=B6cker?= unencoded words =?iso-8859-9?Q?S=2E=C7a=F0lar?=")
                                    << QByteArray("ISO-8859-9") << QByteArray("utf-8") << false
                                    << QString::fromUtf8("Subject: Ingo Klöcker unencoded words S.Çağlar");

    // illegal characters which are already encoded in the given encoding but are not ASCII (bug 206417)
    QTest::newRow("illegal characters") << QByteArray("Subject: =?utf-8?Q?пиѿилл,=20=D0=B4=D0=BE=D0=B1=D1=80=D1=8B=D0=B9=20=D0=B4=D0=B5=D0=BD=D1=8C?=")
                                        << QByteArray("UTF-8") << QByteArray("utf-8") << false
                                        << QString::fromUtf8("Subject: пиѿилл, добрый день");
    QTest::newRow("illegal characters") << QByteArray("Subject: =?iso-8859-1?Q?������?=")
                                        << QByteArray("ISO-8859-1") << QByteArray("utf-8") << false
                                        << QString::fromLatin1("Subject: ������");

    QTest::newRow("small data") << QByteArray("=?iso-8859-1?Q?c?=")
                                << QByteArray("ISO-8859-1") << QByteArray("utf-8") << false
                                << QString::fromUtf8("c");
}

void RFC2047Test::testRFC2047decode()
{
    QFETCH(QByteArray, input);
    QFETCH(QByteArray, expectedCharset);
    QFETCH(QByteArray, defaultCharset);
    QFETCH(bool, forceCharset);
    QFETCH(QString, expectedResult);

    QByteArray detectedCharset;

    const KCodecs::CharsetOption options = forceCharset ? KCodecs::ForceDefaultCharset : KCodecs::NoOption;
    const QString result = KCodecs::decodeRFC2047String(input, &detectedCharset,
                                                        defaultCharset, options);

    QCOMPARE(result, expectedResult);
    QCOMPARE(detectedCharset, expectedCharset);
}

void RFC2047Test::testInvalidDecode_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("") << QByteArray("=") << QString::fromUtf8("=");
    QTest::newRow("") << QByteArray("=?") << QString::fromUtf8("=?");
    QTest::newRow("") << QByteArray("=?a?b?=") << QString::fromUtf8("=?a?b?=");
    QTest::newRow("") << QByteArray("=?a?b?c?") << QString::fromUtf8("=?a?b?c?");
    QTest::newRow("") << QByteArray("=?a??c?=") << QString::fromUtf8("=?a??c?=");
}

void RFC2047Test::testInvalidDecode()
{
    QFETCH(QByteArray, input);
    QFETCH(QString, expectedResult);

    QByteArray encCharset;

    const QString result = KCodecs::decodeRFC2047String(input, &encCharset);
    QCOMPARE(result, expectedResult);
}

void RFC2047Test::testRFC2047encode_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QByteArray>("encoding");
    QTest::addColumn<QByteArray>("expectedResult");

    QTest::newRow("empty") << QString()
                           << QByteArray("utf-8")
                           << QByteArray();
    QTest::newRow("identity") << QString::fromUtf8("bla")
                              << QByteArray("utf-8")
                              << QByteArray("bla");
    QTest::newRow("QP") << QString::fromUtf8("Ingo Klöcker <kloecker@kde.org>")
                        << QByteArray("utf-8")
                        << QByteArray("=?utf-8?q?Ingo=20Kl=C3=B6cker?= <kloecker@kde.org>");

    QTest::newRow("utf-8 fallback") << QString::fromUtf8("æſðđŋħł")
                                    << QByteArray("latin1")
                                    << QByteArray("=?utf-8?B?w6bFv8OwxJHFi8SnxYI=?=");
}


void RFC2047Test::testRFC2047encode()
{
    QFETCH(QString, input);
    QFETCH(QByteArray, encoding);
    QFETCH(QByteArray, expectedResult);

    const QByteArray result = KCodecs::encodeRFC2047String(input, encoding);

    // expected value is probably wrong, libkmime will chose 'B' instead of 'Q' encoding
    QEXPECT_FAIL("QP", "KCodecs will choose 'B' instead of 'Q' encoding", Continue);
    QCOMPARE(result, expectedResult);
}
