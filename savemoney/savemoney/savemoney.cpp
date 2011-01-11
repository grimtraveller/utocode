#include "savemoney.h"
#include <QStandardItemModel>

savemoney::savemoney(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(addRecord()));
		///
		QStandardItemModel* model = new QStandardItemModel();

	//���ô�С

	model->setColumnCount(100);    //��
	model->setRowCount(100);    //��

	//���ñ���

	model->setHeaderData(0,Qt::Horizontal,"ID");

	//�������

	for(int j=0;j<100;j++)
	{
		//дid
		QStandardItem *itemID = new QStandardItem("hello");//QString::number(j)));
		model->setItem(j,0,itemID);

	}

	//ѡ�����model

	ui.tableView->setModel(model);

	//�����������
	//ui.tableView->verticalHeader()->hide();
	//�п�
	//ui.tableView->setColumnWidth(0,30);
	//����ѡ��
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