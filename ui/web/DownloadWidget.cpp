//
// Created by Raffaele Montella on 03/03/24.
//

#include "DownloadWidget.hpp"

#include <QFileInfo>
#include <QUrl>
#include <QWebEngineDownloadRequest>

using namespace Qt::StringLiterals;

namespace fairwindsk::ui::web {
    DownloadWidget::DownloadWidget(QWebEngineDownloadRequest *download, QWidget *parent)
            : QFrame(parent)
            , m_download(download)
            , m_timeAdded()
    {
        m_timeAdded.start();
        setupUi(this);
        m_dstName->setText(m_download->downloadFileName());
        m_srcUrl->setText(m_download->url().toDisplayString());

        connect(m_cancelButton, &QPushButton::clicked,
                [this](bool) {
                    if (m_download->state() == QWebEngineDownloadRequest::DownloadInProgress)
                        m_download->cancel();
                    else
                            emit removeClicked(this);
                });

        connect(m_download, &QWebEngineDownloadRequest::totalBytesChanged, this, &DownloadWidget::updateWidget);
        connect(m_download, &QWebEngineDownloadRequest::receivedBytesChanged, this, &DownloadWidget::updateWidget);

        connect(m_download, &QWebEngineDownloadRequest::stateChanged,
                this, &DownloadWidget::updateWidget);

        updateWidget();
    }

    inline QString DownloadWidget::withUnit(qreal bytes)
    {
        if (bytes < (1 << 10))
            return tr("%L1 B").arg(bytes);
        if (bytes < (1 << 20))
            return tr("%L1 KiB").arg(bytes / (1 << 10), 0, 'f', 2);
        if (bytes < (1 << 30))
            return tr("%L1 MiB").arg(bytes / (1 << 20), 0, 'f', 2);
        return tr("%L1 GiB").arg(bytes / (1 << 30), 0, 'f', 2);
    }

    void DownloadWidget::updateWidget()
    {
        qreal totalBytes = m_download->totalBytes();
        qreal receivedBytes = m_download->receivedBytes();
        qreal bytesPerSecond = receivedBytes / m_timeAdded.elapsed() * 1000;

        auto state = m_download->state();
        switch (state) {
            case QWebEngineDownloadRequest::DownloadRequested:
                Q_UNREACHABLE();
                break;
            case QWebEngineDownloadRequest::DownloadInProgress:
                if (totalBytes >= 0) {
                    m_progressBar->setValue(qRound(100 * receivedBytes / totalBytes));
                    m_progressBar->setDisabled(false);
                    m_progressBar->setFormat(
                            tr("%p% - %1 of %2 downloaded - %3/s")
                                    .arg(withUnit(receivedBytes), withUnit(totalBytes),
                                         withUnit(bytesPerSecond)));
                } else {
                    m_progressBar->setValue(0);
                    m_progressBar->setDisabled(false);
                    m_progressBar->setFormat(
                            tr("unknown size - %1 downloaded - %2/s")
                                    .arg(withUnit(receivedBytes), withUnit(bytesPerSecond)));
                }
                break;
            case QWebEngineDownloadRequest::DownloadCompleted:
                m_progressBar->setValue(100);
                m_progressBar->setDisabled(true);
                m_progressBar->setFormat(
                        tr("completed - %1 downloaded - %2/s")
                                .arg(withUnit(receivedBytes), withUnit(bytesPerSecond)));
                break;
            case QWebEngineDownloadRequest::DownloadCancelled:
                m_progressBar->setValue(0);
                m_progressBar->setDisabled(true);
                m_progressBar->setFormat(
                        tr("cancelled - %1 downloaded - %2/s")
                                .arg(withUnit(receivedBytes), withUnit(bytesPerSecond)));
                break;
            case QWebEngineDownloadRequest::DownloadInterrupted:
                m_progressBar->setValue(0);
                m_progressBar->setDisabled(true);
                m_progressBar->setFormat(
                        tr("interrupted: %1")
                                .arg(m_download->interruptReasonString()));
                break;
        }

        if (state == QWebEngineDownloadRequest::DownloadInProgress) {
            static QIcon cancelIcon(u":process-stop.png"_s);
            m_cancelButton->setIcon(cancelIcon);
            m_cancelButton->setToolTip(tr("Stop downloading"));
        } else {
            static QIcon removeIcon(u":edit-clear.png"_s);
            m_cancelButton->setIcon(removeIcon);
            m_cancelButton->setToolTip(tr("Remove from list"));
        }
    }
}