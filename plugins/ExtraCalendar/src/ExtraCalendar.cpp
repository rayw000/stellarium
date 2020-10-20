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

static const char* days[] = {
  "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
  "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
  "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};

static const char* months[] = {
  "正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "十一", "腊"
};

static const char* zodiacX[] = {
  "庚", "辛", "壬", "癸", "甲", "乙", "丙", "丁", "戊", "己"
};

static const char* zodiacY[] = {
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
	info.displayedName = N_("Extra Calendar");
	info.authors = "Ray";
	info.contact = "ray.hackmylife@gmail.com";
	info.description = N_("Display various calendars. Currently support Chinese Lunisolar (1900-2100).");
  info.version = EXTRACALENDAR_PLUGIN_VERSION;
	info.license = EXTRACALENDAR_PLUGIN_LICENSE;
	return info;
}

/*************************************************************************
 Constructor
*************************************************************************/
ExtraCalendar::ExtraCalendar()
{
	setObjectName("ExtraCalendar");
  font.setPixelSize(14);
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
		return StelApp::getInstance().getModuleMgr().getModule("LabelMgr")->getCallOrder(actionName)+10.;
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
  QDateTime datetime = StelUtils::jdToQDateTime(core->getJD());
  datetime.setTimeSpec(Qt::UTC);
  QDate date = datetime.toLocalTime().date();

  // TODO: configurable calendar selection
  QString calendarName("Chinese Lunisolar");

  if (date.year() < 1901 || date.year() > 2099) {
    qDebug() << "ExtraCalendar only supports years between 1900 and 2100.";
    return;
  }

  Gregorian* g = new Gregorian();
  Lunisolar* l = new Lunisolar();

  g->year = date.year();
  g->month = date.month();
  g->day = date.day();

  *l = getLunisolarFromGregorian(*g);

  QString text;
  QTextStream oss(&text);
  oss << "[" << q_(calendarName) << "] "
      << q_(zodiacX[(l->year - 1900) % 10]) << q_(zodiacY[(l->year - 1900) % 12]) << q_("年")
      << q_(l->isleap ? "闰" : "")
      << q_(months[l->month - 1]) << q_("月")
      << q_(days[l->day - 1]);

  StelPainter painter(core->getProjection2d());
	painter.setColor(1,1,1,1);
	painter.setFont(font);
  painter.drawText(200, 200, text);
}
