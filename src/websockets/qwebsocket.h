/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWEBSOCKET_H
#define QWEBSOCKET_H

#include <QtCore/QUrl>
#ifndef QT_NO_NETWORKPROXY
#include <QtNetwork/QNetworkProxy>
#endif
#ifndef QT_NO_SSL
#include <QtNetwork/QSslError>
#include <QtNetwork/QSslConfiguration>
#endif
#include "qwebsockets_global.h"
#include "qwebsocketprotocol.h"
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 2)
#else
#include "qwebsocket4.h"
#endif

QT_BEGIN_NAMESPACE

class QTcpSocket;
class QWebSocketPrivate;

class Q_WEBSOCKETS_EXPORT QWebSocket : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QWebSocket)
    Q_DECLARE_PRIVATE(QWebSocket)

public:
    explicit QWebSocket(const QString &origin = QString(),
                        QWebSocketProtocol::Version version = QWebSocketProtocol::V_LATEST,
                        QObject *parent = Q_NULLPTR);
    virtual ~QWebSocket();

    void abort();
    QAbstractSocket::SocketError error() const;
    QString errorString() const;
    bool flush();
    bool isValid() const;
    QHostAddress localAddress() const;
    quint16 localPort() const;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 2)
    QAbstractSocket::PauseModes pauseMode() const;
#endif
    QHostAddress peerAddress() const;
    QString peerName() const;
    quint16 peerPort() const;
#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy proxy() const;
    void setProxy(const QNetworkProxy &networkProxy);
#endif
    qint64 readBufferSize() const;
    void setReadBufferSize(qint64 size);

    void resume();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 2)
    void setPauseMode(QAbstractSocket::PauseModes pauseMode);
#endif

    void setSocketOption(QAbstractSocket::SocketOption option, const QVariant &value);
    QVariant socketOption(QAbstractSocket::SocketOption option);
    QAbstractSocket::SocketState state() const;

    QWebSocketProtocol::Version version() const;
    QString resourceName() const;
    QUrl requestUrl() const;
    QString origin() const;
    QString protocol() const;
    QString extension() const;
    QWebSocketProtocol::CloseCode closeCode() const;
    QString closeReason() const;

    const QList<QPair<QString, QString> > &additionalQueryString() const;
    const QList<QPair<QString, QString> > &additionalHeaders() const;

    void setAdditonalQueryString(QList<QPair<QString, QString> >);
    void setAddtionalHeaders(QList<QPair<QString, QString> > );

    qint64 write(const char *message);//Q_REQUIRED_RESULT;
    qint64 write(const char *message, qint64 maxSize);//Q_REQUIRED_RESULT;
    qint64 write(const QString &message);//Q_REQUIRED_RESULT;
    qint64 write(const QByteArray &data);//Q_REQUIRED_RESULT;

#ifndef QT_NO_SSL
    void ignoreSslErrors(const QList<QSslError> &errors);
    void setSslConfiguration(const QSslConfiguration &sslConfiguration);
    QSslConfiguration sslConfiguration() const;
#endif

public Q_SLOTS:
    void close(QWebSocketProtocol::CloseCode closeCode = QWebSocketProtocol::CC_NORMAL,
               const QString &reason = QString());
    void open(const QUrl &url, bool mask = true);
    void ping(const QByteArray &payload = QByteArray());
#ifndef QT_NO_SSL
    void ignoreSslErrors();
#endif

Q_SIGNALS:
    void aboutToClose();
    void hostFound();
    void connected();
    void disconnected();
    void stateChanged(QAbstractSocket::SocketState state);
#ifndef QT_NO_NETWORKPROXY
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *pAuthenticator);
#endif
    void readChannelFinished();
    void textFrameReceived(QString frame, bool isLastFrame);
    void binaryFrameReceived(QByteArray frame, bool isLastFrame);
    void textMessageReceived(QString message);
    void binaryMessageReceived(QByteArray message);
    void error(QAbstractSocket::SocketError error);
    void pong(quint64 elapsedTime, QByteArray payload);
    void bytesWritten(qint64 bytes);
    void debugMessageAvailable(QString message);

#ifndef QT_NO_SSL
    void sslErrors(const QList<QSslError> &errors);
#endif

private:
    QWebSocket(QTcpSocket *pTcpSocket, QWebSocketProtocol::Version version,
               QObject *parent = Q_NULLPTR);
    QWebSocketPrivate * const d_ptr;
};

QT_END_NAMESPACE

#endif // QWEBSOCKET_H
