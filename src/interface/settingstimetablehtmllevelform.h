//
//
// Description: This file is part of FET
//
//
// Author: Lalescu Liviu <Please see http://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>
// Copyright (C) 2003 Liviu Lalescu <http://lalescu.ro/liviu/>
//
/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#ifndef SETTINGSTIMETABLEHTMLLEVELFORM_H
#define SETTINGSTIMETABLEHTMLLEVELFORM_H

#include "ui_settingstimetablehtmllevelform_template.h"

class SettingsTimetableHtmlLevelForm:public QDialog, Ui::SettingsTimetableHtmlLevelForm_template
{
	Q_OBJECT

public:
	SettingsTimetableHtmlLevelForm(QWidget* parent);
	~SettingsTimetableHtmlLevelForm();

public slots:
	void ok();
	void cancel();
};

#endif
