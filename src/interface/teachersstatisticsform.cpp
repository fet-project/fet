/***************************************************************************
                          teacherstatisticform.cpp  -  description
                             -------------------
    begin                : March 25, 2006
    copyright            : (C) 2006 by Lalescu Liviu
    email                : Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "teachersstatisticsform.h"

#include "timetable_defs.h"
#include "timetable.h"

#include "fet.h"

#include <QString>
#include <QStringList>

#include <QTableWidget>
#include <QHeaderView>

#include <QSet>
#include <QHash>

TeachersStatisticsForm::TeachersStatisticsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);
	
	closeButton->setDefault(true);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	tableWidget->clear();
	tableWidget->setColumnCount(3);
	tableWidget->setRowCount(gt.rules.teachersList.size());
	
	QStringList columns;
	columns<<tr("Teacher");
	columns<<tr("No. of activities");
	columns<<tr("Duration");
	
	tableWidget->setHorizontalHeaderLabels(columns);
	
	QHash<QString, QSet<Activity*> > activitiesForTeacher;
	
	foreach(Activity* act, gt.rules.activitiesList)
		if(act->active)
			foreach(QString teacherName, act->teachersNames){
				QSet<Activity*> acts=activitiesForTeacher.value(teacherName, QSet<Activity*>());
				acts.insert(act);
				activitiesForTeacher.insert(teacherName, acts);
			}
	
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* t=gt.rules.teachersList[i];
		
		QTableWidgetItem* newItem=new QTableWidgetItem(t->name);
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		tableWidget->setItem(i, 0, newItem);

		int	nSubActivities=0;
		int nHours=0;
		
		QSet<Activity*> acts=activitiesForTeacher.value(t->name, QSet<Activity*>());
		
		foreach(Activity* act, acts){
			if(act->active){
				nSubActivities++;
				nHours+=act->duration;
			}
			else{
				assert(0);
			}
		}

		newItem=new QTableWidgetItem(CustomFETString::number(nSubActivities));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		tableWidget->setItem(i, 1, newItem);

		newItem=new QTableWidgetItem(CustomFETString::number(nHours));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		tableWidget->setItem(i, 2, newItem);
	}

	tableWidget->resizeColumnsToContents();
	tableWidget->resizeRowsToContents();
}

TeachersStatisticsForm::~TeachersStatisticsForm()
{
	saveFETDialogGeometry(this);
}
