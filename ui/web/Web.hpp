//
// Created by Raffaele Montella on 14/01/22.
//

#ifndef FAIRWINDSK_WEB_HPP
#define FAIRWINDSK_WEB_HPP

#include <QWidget>

#include <FairWindSK.hpp>
#include <QtWidgets/QProgressBar>
#include "ui_Web.h"

#include "WebView.hpp"
#include "DownloadManagerWidget.hpp"

namespace Ui { class Web; }

namespace fairwindsk::ui::web {

    class AppItem;

    class Web : public QWidget {
    Q_OBJECT

    public:
        explicit Web(QWidget *parent = nullptr);

        void goHome();

        ~Web() override;

        void setApp(fairwindsk::AppItem *appItem);

        void showButtons(bool show);
        void toggleButtons();

        DownloadManagerWidget &downloadManagerWidget() { return m_downloadManagerWidget; }

    public slots:
        void toolButton_home_clicked();

    private slots:
        void handleWebViewLoadProgress(int);

    private:
        Ui::Web *ui;

        WebView *m_webView = nullptr;
        WebPage *m_webPage = nullptr;
        fairwindsk::AppItem *m_appItem = nullptr;

        QWebEngineProfile *m_profile = nullptr;
        DownloadManagerWidget m_downloadManagerWidget;


        QProgressBar *m_progressBar = nullptr;

    };
} // fairwindsk::ui::web

#endif //FAIRWIND_MAINPAGE_HPP
