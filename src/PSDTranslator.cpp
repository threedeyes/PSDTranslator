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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ConfigView.h"
#include "PSDTranslator.h"
#include "PSDLoader.h"

const char *kDocumentCount = "/documentCount";
const char *kDocumentIndex = "/documentIndex";

#define kPSDMimeType "image/vnd.adobe.photoshop"
#define kPSDName "Photoshop image"

static const translation_format sInputFormats[] = {
	{
		PSD_IMAGE_FORMAT,
		B_TRANSLATOR_BITMAP,
		PSD_IN_QUALITY,
		PSD_IN_CAPABILITY,
		kPSDMimeType,
		kPSDName
	},
};

static const translation_format sOutputFormats[] = {
	{
		B_TRANSLATOR_BITMAP,
		B_TRANSLATOR_BITMAP,
		BITS_OUT_QUALITY,
		BITS_OUT_CAPABILITY,
		"image/x-be-bitmap",
		"Be Bitmap Format (PSDTranslator)"
	},
};


static const TranSetting sDefaultSettings[] = {
	{B_TRANSLATOR_EXT_HEADER_ONLY, TRAN_SETTING_BOOL, false},
	{B_TRANSLATOR_EXT_DATA_ONLY, TRAN_SETTING_BOOL, false},
	{PSD_SETTING_LAYERS_DECODE, TRAN_SETTING_BOOL, true}
};

const uint32 kNumInputFormats = 1;
const uint32 kNumOutputFormats = 1;
const uint32 kNumDefaultSettings = sizeof(sDefaultSettings)
	/ sizeof(TranSetting);


PSDTranslator::PSDTranslator()
	: BaseTranslator("Photoshop image",
		"Photoshop image translator",
		PSD_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats,
		sOutputFormats, kNumOutputFormats,
		"PSDTranslator",
		sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_BITMAP, PSD_IMAGE_FORMAT)
{
}


PSDTranslator::~PSDTranslator()
{
}


status_t
PSDTranslator::DerivedIdentify(BPositionIO *stream,
	const translation_format *format, BMessage *ioExtension,
	translator_info *info, uint32 outType)
{
	if (!outType)
		outType = B_TRANSLATOR_BITMAP;
	if (outType != B_TRANSLATOR_BITMAP && outType != PSD_IMAGE_FORMAT)
		return B_NO_TRANSLATOR;
		
	uint32 signatureData;
	ssize_t signatureSize = 4;
	if (stream->Read(&signatureData, signatureSize) != signatureSize)
		return B_IO_ERROR;
			
	const uint32 kPSDMagic = B_HOST_TO_BENDIAN_INT32('8BPS');
	
	if (signatureData != kPSDMagic)
		return B_ILLEGAL_DATA;
	
	int32 layersDecodeFlag = 
		(fSettings->SetGetBool(PSD_SETTING_LAYERS_DECODE)) ? 1 : 0;
	
	PSDLoader psdFile(stream, layersDecodeFlag?PSD_DECODE_ALL:PSD_DECODE_MERGED);
	if(!psdFile.IsLoaded() || psdFile.LayersCount() <= 0)
		return B_ILLEGAL_DATA;

	int32 documentCount = psdFile.LayersCount();
	int32 documentIndex = 1;
	
	if (ioExtension) {
		if (ioExtension->FindInt32(DOCUMENT_INDEX, &documentIndex) != B_OK)
			documentIndex = 1;
		if (documentIndex < 1 || documentIndex > documentCount)
			return B_NO_TRANSLATOR;
	}	
	
	if (ioExtension) {
		ioExtension->RemoveName(DOCUMENT_COUNT);
		ioExtension->AddInt32(DOCUMENT_COUNT, documentCount);
	}	
		
	info->type = PSD_IMAGE_FORMAT;
	info->group = B_TRANSLATOR_BITMAP;
	info->quality = PSD_IN_QUALITY;
	info->capability = PSD_IN_CAPABILITY;
	BString iconName("PSD Image");
	if(layersDecodeFlag)
		iconName << " (merged)";
	if (documentIndex > 1) {
		iconName = "PSD Layer";
		iconName << " #" << (documentIndex-1);
	}
	snprintf(info->name, sizeof(info->name), iconName.String());
	strcpy(info->MIME, kPSDMimeType);
	
	return B_OK;
}


status_t
PSDTranslator::DerivedTranslate(BPositionIO *source,
	const translator_info *info, BMessage *ioExtension,
	uint32 outType, BPositionIO *target, int32 baseType)
{
	if (outType != B_TRANSLATOR_BITMAP)
		return B_NO_TRANSLATOR;

	switch (baseType) {
		case 0:
		{								
			if (outType != B_TRANSLATOR_BITMAP)
				return B_NO_TRANSLATOR;

			int32 layersDecodeFlag = 
				(fSettings->SetGetBool(PSD_SETTING_LAYERS_DECODE)) ? 1 : 0;


			PSDLoader psdFile(source, layersDecodeFlag?PSD_DECODE_ALL:PSD_DECODE_MERGED);
			if (!psdFile.IsLoaded() || psdFile.LayersCount() <= 0)
				return B_NO_TRANSLATOR;

			int32 documentIndex = 1;

			if (ioExtension) {
				if (ioExtension->FindInt32(DOCUMENT_INDEX,
					&documentIndex) != B_OK)
					documentIndex = 1;
			}
			
			return psdFile.GetImage(target, documentIndex);			
		}

		default:
			return B_NO_TRANSLATOR;
	}
}


status_t
PSDTranslator::DerivedCanHandleImageSize(float width, float height) const
{
	return B_OK;
}


BView *
PSDTranslator::NewConfigView(TranslatorSettings *settings)
{
	return new ConfigView(settings);
}


BTranslator *
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (n != 0)
		return NULL;

	return new PSDTranslator();
}

