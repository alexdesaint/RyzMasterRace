#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include <QtWidgets>

#include <MSRRegister.hpp>
#include <CPUIDRegister.hpp>
#include <QRegister.hpp>

class PerCoreMonitor : public QVBoxLayout {
private:
    int cpu;

    QLabel *powerLabels;
    uint32_t powerOld;

    QProgressBar *freqBar;

    QLabel *maxFreqLabels, *actuFreqLabels, *tscLabel;
    uint64_t tscOld, mperfOld, aperfOld, mPerfReadOnlyOld, aPerfReadOnlyOld, irPerfCountold;

public:
    PerCoreMonitor(int cpu) : cpu(cpu), powerLabels(new QLabel), freqBar(new QProgressBar), maxFreqLabels(new QLabel), actuFreqLabels(new QLabel), tscLabel(new QLabel) {
        freqBar->setMaximum(4500);
        freqBar->setOrientation(Qt::Orientation::Vertical);
        freqBar->setValue(3000);
        freqBar->setTextVisible(false);
        freqBar->setInvertedAppearance(true);

        maxFreqLabels->setText("__Hz");
        actuFreqLabels->setText("__Hz");
        tscLabel->setText("__Hz");
        powerLabels->setText("__W");

        powerLabels->setMinimumWidth(20);

        addWidget(freqBar);
        addWidget(maxFreqLabels);
        addWidget(actuFreqLabels);
        addWidget(tscLabel);
        addWidget(powerLabels);

    }

    void refresh(uint32_t timeScale) {
        char str[200];

        {
            auto tsc = MSRRegister::Registers[MSRRegister::MSR::TSC];
            //auto mperf = MSRRegister::Registers[MSRRegister::MSR::MPERF];
            //auto aperf = MSRRegister::Registers[MSRRegister::MSR::APERF];
            auto mperfro = MSRRegister::Registers[MSRRegister::MSR::MPerfReadOnly];
            auto aperfro = MSRRegister::Registers[MSRRegister::MSR::APerfReadOnly];
            //auto irPerfCount = MSRRegister::Registers[MSRRegister::MSR::IRPerfCount];
            uint64_t t = tsc.data[0][cpu];
            //uint64_t m = mperf.data[0][cpu];
            //uint64_t a = aperf.data[0][cpu];
            uint64_t mro = mperfro.data[0][cpu];
            uint64_t aro = aperfro.data[0][cpu];
            //uint64_t i = irPerfCount.data[0][cpu];

            sprintf(str, "%lu mHz", (t - tscOld)/1000000);
            tscLabel->setText(str);

            auto maxf= ((t - tscOld) * timeScale * (aro - aPerfReadOnlyOld) * timeScale)/(1000000);
            sprintf(str, "%lu MHz", maxf);
            maxFreqLabels->setText(str);

            maxf = ((t - tscOld) * (aro - aPerfReadOnlyOld) * timeScale)/((mro - mPerfReadOnlyOld) * 1000000);
            sprintf(str, "%lu MHz", maxf);
            actuFreqLabels->setText(str);
            freqBar->setValue(maxf);

            //printf("%lu %lu %lu %lu\n", t - tscOld, aro - aPerfReadOnlyOld, mro - mPerfReadOnlyOld, i - irPerfCountold);

            tscOld = t;
            //mperfOld = m;
            //aperfOld = a;
            mPerfReadOnlyOld = mro;
            aPerfReadOnlyOld = aro;
            //irPerfCountold = i;
        }
        {
            auto &r = MSRRegister::Registers[MSRRegister::MSR::CORE_ENERGY_STAT];
            uint32_t p = r.data[0][cpu] & r.bitDescriptors[1].mask;
            sprintf(str, "%.2fW", (p - powerOld) * MSRRegister::joulesScale / timeScale);
            powerOld = p;
            powerLabels->setText(str);
        }
    }
};

class MainWindow : public QMainWindow {
private:
    QRegister<MSRRegister::Register> *msrRegisters;
    QRegister<CPUIDRegister::Register> *cpuidRegisters;

    QWidget *createMonitor();
    void refresh() {
        MSRRegister::read();
        msrRegisters->refresh();
        refreshMonitor();
    }
    void refreshMonitor();
    void about();

    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *refreshAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QTabWidget *tab;
    QTimer *timer;

    std::vector<PerCoreMonitor *> perCoreMonitor;

public:
    MainWindow();
};

MainWindow::MainWindow() : timer(new QTimer), tab(new QTabWidget) {
    setCentralWidget(tab);

    MSRRegister::open();
    MSRRegister::read();
    msrRegisters = new QRegister<MSRRegister::Register>(MSRRegister::Registers, MSRRegister::numOfCPU);
    cpuidRegisters = new QRegister<CPUIDRegister::Register>(CPUIDRegister::Registers, 1);
    cpuidRegisters->refresh();
    //table->setHeaderHidden(true);

    connect(timer, &QTimer::timeout, this, &MainWindow::refresh);
    timer->start(500);

    tab->addTab(createMonitor(), "Monitor data");
    tab->addTab(msrRegisters, "Raw MSR");
    tab->addTab(cpuidRegisters, "Raw CPUID");

    refresh();

    createActions();
    createMenus();

    statusBar()->showMessage("Ready");

    setWindowTitle(tr("RyzMasterRace"));
    setMinimumSize(160, 160);
    resize(480, 320);
}

QWidget *MainWindow::createMonitor() {
    auto widget = new QWidget;
    auto *layout = new QHBoxLayout;

    perCoreMonitor.resize(MSRRegister::numOfCPU/2);

    for(int i = 0; i < MSRRegister::numOfCPU/2; i++) {
        perCoreMonitor[i] = new PerCoreMonitor(i);
        layout->addLayout(perCoreMonitor[i]);
    }

    widget->setLayout(layout);
    return widget;
}

void MainWindow::refreshMonitor() {
    char str[200];
    uint32_t timeScale = 1000 / timer->interval();

    for (auto cm : perCoreMonitor) {
        cm->refresh(timeScale);
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About RyzMasterRace"),
                       tr("<b>RyzMasterRace</b> is under GNU General Public License v3.0"));
}

void MainWindow::createActions() {

    refreshAct = new QAction(tr("&Refresh"), this);
    refreshAct->setShortcuts(QKeySequence::Refresh);
    refreshAct->setStatusTip(tr("Create a new file"));
    connect(refreshAct, &QAction::triggered, this, &MainWindow::refresh);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus() {

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(refreshAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
