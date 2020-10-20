/*
 * Copyright (C) 2007 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#include "StelProjector.hpp"
#include "StelPainter.hpp"
#include "StelApp.hpp"
#include "StelCore.hpp"
#include "StelUtils.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"
#include "ExtraCalendar.hpp"
#include "chinese/lunisolar.hpp"

#include <QDebug>
#include <sstream>

static std::string days[] = {
  "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
  "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
  "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};

static std::string months[] = {
  "正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "腊"
};

static std::string zodiacX[] = {
  "庚", "辛", "壬", "癸", "甲", "乙", "丙", "丁", "戊", "己"
};

static std::string zodiacY[] = {
  "子", "丑", "寅", "卯", "辰", "巳", "无", "为", "申", "酉", "戌", "亥"
};

/*************************************************************************
 This method is the one called automatically by the StelModuleMgr just
 after loading the dynamic library
*************************************************************************/
StelModule* ExtraCalendarStelPluginInterface::getStelModule() const
{
	return new ExtraCalendar();
}

StelPluginInfo ExtraCalendarStelPluginInterface::getPluginInfo() const
{
	StelPluginInfo info;
	info.id = "ExtraCalendar";
	info.displayedName = "Extra Calendar";
	info.authors = "Stellarium team";
	info.contact = "www.stellarium.org";
	info.description = "An minimal plugin example.";
	return info;
}

/*************************************************************************
 Constructor
*************************************************************************/
ExtraCalendar::ExtraCalendar()
{
	setObjectName("ExtraCalendar");
  font.setPixelSize(25);
}

/*************************************************************************
 Destructor
*************************************************************************/
ExtraCalendar::~ExtraCalendar()
{
}

/*************************************************************************
 Reimplementation of the getCallOrder method
*************************************************************************/
double ExtraCalendar::getCallOrder(StelModuleActionName actionName) const
{
	if (actionName==StelModule::ActionDraw)
		return StelApp::getInstance().getModuleMgr().getModule("NebulaMgr")->getCallOrder(actionName)+10.;
	return 0;
}


/*************************************************************************
 Init our module
*************************************************************************/
void ExtraCalendar::init()
{
	qDebug() << "init called for ExtraCalendar";
}

/*************************************************************************
 Draw our module. This should print "Hello world!" in the main window
*************************************************************************/
void ExtraCalendar::draw(StelCore* core)
{
	StelPainter painter(core->getProjection2d());
	painter.setColor(1,1,1,1);
	painter.setFont(font);
  Gregorian* g = new Gregorian();
  Lunisolar* l = new Lunisolar();

  QDateTime datetime = StelUtils::jdToQDateTime(core->getJD());
  datetime.setTimeSpec(Qt::UTC);
  QDate date = datetime.toLocalTime().date();

  g->year = date.year();
  g->month = date.month();
  g->day = date.day();

  *l = getLunisolarFromGregorian(*g);

  std::ostringstream oss;
  oss << zodiacX[(l->year - 1900) % 10] << zodiacY[(l->year - 1900) % 12] << "年"
     << (l->isleap ? "闰" : "")
     << months[l->month - 1] << "月"
     << days[l->day - 1]
     << std::endl;
  painter.drawText(300, 300, oss.str().c_str());
}

/*************************************************************************
 Update calendar when time changes
*************************************************************************/
void ExtraCalendar::update(double deltaTime) {
  // qDebug() << deltaTime;
}
