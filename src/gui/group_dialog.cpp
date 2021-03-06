﻿#include "group_dialog.h"
#include "ui_group_dialog.h"

#include <QStandardItemModel>


group_dialog::group_dialog (database *db, QWidget *parent) :
  QDialog (parent),
  ui (new Ui::group_dialog), m_db (db)
{
  ui->setupUi (this);

  m_model = std::make_unique<QStandardItemModel> (this);
  fill_the_model ();
  ui->treeView->header()->hide();
  ui->treeView->setModel (m_model.get ());

  connect (ui->add_group_button, SIGNAL (clicked ()), this, SLOT (add_button_clicked ()));
  connect (ui->remove_group_button, SIGNAL (clicked ()), this, SLOT (remove_group()));
  connect (ui->treeView->selectionModel (), SIGNAL (selectionChanged (const QItemSelection, const QItemSelection)),
           this, SLOT (selection_changed ()));
}

group_dialog::~group_dialog()
{
  delete ui;
}

void group_dialog::remove_group ()
{
  auto selected = ui->treeView->currentIndex ();
  if (!selected.isValid ())
    return;
  int idx = m_model->data (selected, Qt::UserRole + 1).toInt ();
  m_db->m_groups->remove (idx);

  fill_the_model ();
}


void group_dialog::fill_the_model()
{
  int ed_years = 0;
  for (const auto &val : m_db->m_groups->get_ids ())
    {
      if (ed_years < m_db->m_groups->get_data (val).get_ed_year ())
        {
          ed_years = m_db->m_groups->get_data (val).get_ed_year ();
        }
    }
  m_model->clear();
  QStandardItem *parent_item = m_model->invisibleRootItem();
  std::vector<QStandardItem *> ed_year_items;
  for (int i = 1; i < ed_years + 1; i++)
    {
      QStandardItem *item = new QStandardItem();
      ed_year_items.push_back (item);
      item->setText (QString ("Education year %1").arg (i));
      item->setFlags (Qt::ItemIsEnabled);
      parent_item->appendRow (item);
    }
  for (const auto &val : m_db->m_groups->get_ids ())
    {
      QStandardItem *item = new QStandardItem();
      auto group = m_db->m_groups->get_data (val);
      item->setText (QString ("Group %1%2").arg (group.get_thread ()).arg (group.get_group_num ()));
      item->setData (QVariant (group.get_group_id ()));
      ed_year_items[group.get_ed_year () - 1]->appendRow (item);
    }
  ui->treeView->expandAll ();
}

void group_dialog::add_button_clicked()
{
  int new_id = m_db->m_groups->create_new ();
  auto &val = m_db->m_groups->get_data (new_id);
  val.set_ed_year (ui->ed_year->value ());
  val.set_num    (ui->number->value ());
  val.set_thread (ui->tread->value ());
  fill_the_model ();
}


void group_dialog::ed_year_changed ()
{
  auto selected = ui->treeView->currentIndex ();
  if (!selected.isValid ())
    return;
  QVariant data = m_model->data (selected, Qt::UserRole + 1);
  int group_id = data.toInt ();
  if (group_id < 0)
    return;

  auto &group = m_db->m_groups->get_data (group_id);
  group.set_ed_year (ui->ed_year->value () > 0 ? ui->ed_year->value () : 0);
  fill_the_model ();
}


void group_dialog::num_changed ()
{
  auto selected = ui->treeView->currentIndex ();
  if (!selected.isValid ())
    return;
  QVariant data = m_model->data (selected, Qt::UserRole + 1);
  int group_id = data.toInt ();
  if (group_id < 0)
    return;

  auto &group = m_db->m_groups->get_data (group_id);
  group.set_num (ui->number->value () > 0 ? ui->number->value () : 0);
  fill_the_model ();
}

void group_dialog::thread_changed ()
{
  auto selected = ui->treeView->currentIndex ();
  if (!selected.isValid ())
    return;
  QVariant data = m_model->data (selected, Qt::UserRole + 1);
  int group_id = data.toInt ();
  if (group_id < 0)
    return;

  auto &group = m_db->m_groups->get_data (group_id);
  group.set_thread (ui->tread->value () > 0 ? ui->tread->value () : 0);
  fill_the_model ();
}



void group_dialog::selection_changed ()
{
  auto selected = ui->treeView->currentIndex ();
  if (!selected.isValid ())
    return;
  QVariant data = m_model->data (selected, Qt::UserRole + 1);
  ui->formLayout->setEnabled (true);
  if (!data.isValid ())
    {
      ui->formLayout->setEnabled (false);
      return;
    }
  int group_id = data.toInt ();
  if (group_id < 0)
    return;

  auto group = m_db->m_groups->get_data (group_id);
  ui->ed_year->setValue (group.get_ed_year ());
  ui->number->setValue (group.get_group_num ());
  ui->tread->setValue (group.get_thread ());
}
