#include "savemoney.h"
#include <QStandardItemModel>

savemoney::savemoney(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(addRecord()));
		///
		QStandardItemModel* model = new QStandardItemModel();

	//设置大小

	model->setColumnCount(100);    //列
	model->setRowCount(100);    //行

	//设置标题

	model->setHeaderData(0,Qt::Horizontal,"ID");

	//添加数据

	for(int j=0;j<100;j++)
	{
		//写id
		QStandardItem *itemID = new QStandardItem("hello");//QString::number(j)));
		model->setItem(j,0,itemID);

	}

	//选择这个model

	ui.tableView->setModel(model);

	//隐藏左边那列
	//ui.tableView->verticalHeader()->hide();
	//列宽
	//ui.tableView->setColumnWidth(0,30);
	//整行选择
	//ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	///
}

savemoney::~savemoney()
{

}

void savemoney::addRecord()
{

}

void savemoney::resizeEvent(QResizeEvent * event)
{
	ui.tableView->resize(width()-100, height()-100);
	QWidget::resizeEvent(event);
}