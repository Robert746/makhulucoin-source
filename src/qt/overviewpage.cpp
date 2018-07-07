#include "overviewpage.h"
#include "ui_overviewpage.h"

#include "bignum.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QAbstractItemDelegate>
#include <QPainter>
#include "main.h"

#define DECORATION_SIZE 64
#define NUM_ITEMS 3

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TxViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC)
    {

    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2*ypad)/2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top()+ypad, mainRect.width() - xspace, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = option.palette.color(QPalette::Text);
        if(value.canConvert<QBrush>())
        {
            QBrush brush = qvariant_cast<QBrush>(value);
            foreground = brush.color();
        }

        painter->setPen(foreground);
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address);

        if(amount < 0)
        {
            foreground = COLOR_NEGATIVE;
        }
        else if(!confirmed)
        {
            foreground = COLOR_UNCONFIRMED;
        }
        else
        {
            foreground = option.palette.color(QPalette::Text);
        }
        painter->setPen(foreground);
        QString amountText = BitcoinUnits::formatWithUnit(unit, amount, true);
        if(!confirmed)
        {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight|Qt::AlignVCenter, amountText);

        painter->setPen(option.palette.color(QPalette::Text));
        painter->drawText(amountRect, Qt::AlignLeft|Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;

};
#include "overviewpage.moc"

OverviewPage::OverviewPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverviewPage),
    clientModel(0),
    walletModel(0),
    currentBalance(-1),
    currentUnconfirmedBalance(-1),
    currentImmatureBalance(-1),
    txdelegate(new TxViewDelegate()),
    filter(0)
{
    ui->setupUi(this);

    // Recent transactions
    ui->listTransactions->setItemDelegate(txdelegate);
    ui->listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    ui->listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    ui->listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(ui->listTransactions, SIGNAL(clicked(QModelIndex)), this, SLOT(handleTransactionClicked(QModelIndex)));

    // init "out of sync" warning labels
    ui->labelWalletStatus->setText("(" + tr("out of sync") + ")");
    ui->labelTransactionsStatus->setText("(" + tr("out of sync") + ")");

    // start with displaying the "out of sync" warnings
    showOutOfSyncWarning(true);
}

void OverviewPage::handleTransactionClicked(const QModelIndex &index)
{
    if(filter)
        emit transactionClicked(filter->mapToSource(index));
}

OverviewPage::~OverviewPage()
{
    delete ui;
}

void OverviewPage::setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance)
{
    int unit = walletModel->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
    ui->labelBalance->setText(BitcoinUnits::formatWithUnit(unit, balance));
    ui->labelUnconfirmed->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
    ui->labelImmature->setText(BitcoinUnits::formatWithUnit(unit, immatureBalance));

    // only show immature (newly mined) balance if it's non-zero, so as not to complicate things
    // for the non-mining users
    bool showImmature = immatureBalance != 0;
    ui->labelImmature->setVisible(showImmature);
    ui->labelImmatureText->setVisible(showImmature);


    //display total coin mined in 24h
    boost::filesystem::path pathDebug = GetDataDir() / "debugblc.log";
    double profit = GetBalance24h(pathDebug.string().c_str());
    profit /= COIN;
    ui->ln24hProfits->setText(QString::number(profit,'f',6));
}

void OverviewPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Show warning if this is a prerelease version
        connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        connect(model, SIGNAL(numBlocksChangedOverView(int)), this, SLOT(setNumBlocksOverview(int)));
        connect(model, SIGNAL(currDiff(double)), this, SLOT(setDiffOverview(double)));
        //connect(model, SIGNAL(currHash(double, const char*)), this, SLOT(setNetwork(double, const char*)));
        updateAlerts(model->getStatusBarWarnings());
    }
}

void OverviewPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if(model && model->getOptionsModel())
    {
        // Set up transaction list
        filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->sort(TransactionTableModel::Status, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

        boost::filesystem::path pathDebug = GetDataDir() / "debugblc.log";
        double profit = GetBalance24h(pathDebug.string().c_str());
        ui->ln24hProfits->setText(QString::number(profit,'f',6));
    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void OverviewPage::updateDisplayUnit()
{
    if(walletModel && walletModel->getOptionsModel())
    {
        if(currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance);

        // Update txdelegate->unit with the current unit
        txdelegate->unit = walletModel->getOptionsModel()->getDisplayUnit();

        ui->listTransactions->update();
    }
}

void OverviewPage::updateAlerts(const QString &warnings)
{
    this->ui->labelAlerts->setVisible(!warnings.isEmpty());
    this->ui->labelAlerts->setText(warnings);
}

void OverviewPage::showOutOfSyncWarning(bool fShow)
{
    ui->labelWalletStatus->setVisible(fShow);
    ui->labelTransactionsStatus->setVisible(fShow);
}


bool OverviewPage::refresh24hProfit()
{
    ui->ln24hProfits->setText(QString::number(0));
    boost::filesystem::path pathDebug = GetDataDir() / "debugblc.log";
    const char* filename = pathDebug.string().c_str();
    FILE *fd; // File pointer;
    char buff[1000];
    if ((fd = fopen(filename, "r")) != NULL) // open file
    {
        fseek(fd, 0, SEEK_SET); // make sure start from 0

        while(!feof(fd))
        {
            fgets(buff,1000,fd); // read file
        }
    }
    fclose(fd);

    remove(filename);

    if ((fd = fopen(filename, "w")) != NULL) // open file
        fprintf(fd, "%s", buff);
    fclose(fd);
    return true;
}

double OverviewPage::calcProfit(double blockReward,double diff,double hash)
{
    double two = 2;
    return blockReward/((diff*pow(two,32)/(hash * 1000)/3600/24));
}

void OverviewPage::on_btnc24hProfits_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm!!!", "Do you want to clear profits data?",
                                QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        refresh24hProfit();
}

void OverviewPage::on_btcCalc_clicked()
{
    double diff = ui->lbCalcDiff->text().toDouble();
    double blockReward = ui->lbCalcBlockRW->text().toDouble();
    double hash = ui->lnCalcSpeed->text().toDouble();
    double profit = calcProfit(blockReward, diff, hash);
    ui->lnCalcProfit->setText(QString::number(profit,'f',6));
}

double OverviewPage::GetBalance24h(const char* filename)
{
    FILE *fd; // File pointer
    char date[1000];
    char balance[1000];
    char buff[1000];
    //char buffold[1000];
    static int crrTime, lastTime;
    static double crrBalance, lastBalance;
    double profit = 0;
    int lineNumb = 0;
    static bool fileExisted = true;

    if ((fd = fopen(filename, "r")) != NULL) // open file
    {
        fseek(fd, 0, SEEK_SET); // make sure start from 0

        while(!feof(fd))
        {
            //strcpy(buffold,buff);
            lineNumb++;
            fgets(buff,1000,fd); // read file *prefer using fscanf
        }
        //printf("buff = %s",buff);
        if(lineNumb == 1 && buff == NULL)
        {
            printfblc(0);
            fileExisted = false;
            fclose(fd);
            return 0;
        }
    }
    else
    {
        return 0;
    }

    char * pch;
    pch = strtok (buff," ");
    int index = 0;
    while (pch != NULL)
    {
        if(index == 0)
            strcpy(date,pch);
        else if(index == 1)
            strcpy(balance,pch);
        pch = strtok (NULL, " ");
        index++;
    }
    crrTime = atoi(date);
    crrBalance = atof(balance);

    if(lineNumb == 1)
    {
        fclose (fd);
//        fd = fopen(filename, "w");
//        fprintf(fd,"%d 0\n%d %f\n", crrTime, crrTime, crrBalance);
//        fclose (fd);
        if(fileExisted == false)
        {
            return crrBalance;
        }
        else
        {
            return 0;
        }
    }

    //read file again from start
    fseek(fd, 0, SEEK_SET); // make sure start from 0
    while(!feof(fd))
    {
        fgets(buff, 1000, fd); // read file
        char * pch1;
        pch1 = strtok (buff," ");
        int index1 = 0;
        while (pch1 != NULL)
        {
            if(index1 == 0)
                strcpy(date,pch1);
            else if(index1 == 1)
                strcpy(balance,pch1);
            pch1 = strtok (NULL, " ");
            index1++;
        }
        lastTime = atoi(date);
        lastBalance = atof(balance);
        if(crrTime - lastTime <= 86400)
        {
            profit = crrBalance - lastBalance;
            break;
        }
    }
    fclose (fd);
    return profit;
}

void OverviewPage::setNumBlocksOverview(int height)
{
    ui->lbCalcBlock->setText(QString::number(height));
    if(fTestNet)
        ui->lbCalcBlockRW->setText(QString::number(COINPERBLOCK_2 * (100 - ( height/ 40 * 10)) / 100));
    else
    {
        if(height <= 15000)
            ui->lbCalcBlockRW->setText(QString::number(COINPERBLOCK * (100 - ( height / HEIGHDECREASE) * 5) / 100));
        else
            ui->lbCalcBlockRW->setText(QString::number(COINPERBLOCK_2 * (100 - ( (height - 15000)/ HEIGHDECREASE_2 * 10)) / 100));
    }
}

void OverviewPage::setDiffOverview(double diff)
{
    ui->lbCalcDiff->setText(QString::number(diff));
}
