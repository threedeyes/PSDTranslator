/*
 * Copyright 2012, Gerasim Troeglazov, 3dEyes@gmail.com. All rights reserved.
 *
 * libpsd is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ConfigView.h"
#include "PSDTranslator.h"

#include <StringView.h>
#include <SpaceLayoutItem.h>
#include <ControlLook.h>

#include <stdio.h>


ConfigView::ConfigView(TranslatorSettings *settings)
	: BGroupView("PSDTranslator Settings", B_VERTICAL, 0)
{
	fSettings = settings;

	BAlignment leftAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_UNSET);

	BStringView *stringView = new BStringView("title", "Photoshop image translator");
	stringView->SetFont(be_bold_font);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	float spacing = be_control_look->DefaultItemSpacing();
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

	char version[256];
	sprintf(version, "Version %d.%d.%d, %s",
		int(B_TRANSLATION_MAJOR_VERSION(PSD_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(PSD_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(PSD_TRANSLATOR_VERSION)),
		__DATE__);
	stringView = new BStringView("version", version);
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("copyright",
		B_UTF8_COPYRIGHT "2005-2012 Haiku Inc.");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	stringView = new BStringView("my_copyright",
		B_UTF8_COPYRIGHT "2012 Gerasim Troeglazov <3dEyes@gmail.com>.");
	stringView->SetExplicitAlignment(leftAlignment);
	AddChild(stringView);

	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));

 	fLayersDecode = new BCheckBox("Decode all layers",
		new BMessage(MSG_LAYERS_DECODE_CHANGED));
 	int32 val = (fSettings->SetGetBool(PSD_SETTING_LAYERS_DECODE)) ? 1 : 0;
 	fLayersDecode->SetValue(val);
 	fLayersDecode->SetViewColor(ViewColor());
	AddChild(fLayersDecode);
	
	AddChild(BSpaceLayoutItem::CreateVerticalStrut(spacing));
	
	BString copyrightText;
	copyrightText << "Based on libpsd 0.9.0\n"
		<< "Photoshop file formats decode library\n"
		<< B_UTF8_COPYRIGHT "Copyright (C) 2004-2007 Graphest Software";
	
	fCopyrightView = new BTextView("CopyrightLibs");
	fCopyrightView->SetExplicitAlignment(leftAlignment);
	fCopyrightView->MakeEditable(false);
	fCopyrightView->MakeResizable(true);
	fCopyrightView->SetWordWrap(true);
	fCopyrightView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fCopyrightView->SetText(copyrightText.String());
	fCopyrightView->SetExplicitMinSize(BSize(300,200));

	BFont font;
	font.SetSize(font.Size() * 0.9);
	fCopyrightView->SetFontAndColor(&font, B_FONT_SIZE, NULL);
	AddChild(fCopyrightView);

	fCopyrightView->SetExplicitAlignment(leftAlignment);
	
	AddChild(BSpaceLayoutItem::CreateGlue());
	GroupLayout()->SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, 
		B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING);

	SetExplicitPreferredSize(GroupLayout()->MinSize());		
}


ConfigView::~ConfigView()
{
	fSettings->Release();
}


void
ConfigView::AllAttached()
{
	fLayersDecode->SetTarget(this);
}


void
ConfigView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_LAYERS_DECODE_CHANGED: {
			bool val = fLayersDecode->Value() == B_CONTROL_ON;
			fSettings->SetGetBool(PSD_SETTING_LAYERS_DECODE, &val);
			fSettings->SaveSettings();			
			break;
		}
		default:
			BView::MessageReceived(message);
	}
}
