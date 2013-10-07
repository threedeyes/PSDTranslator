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

#ifndef PSD_TRANSLATOR_H
#define PSD_TRANSLATOR_H


#include "BaseTranslator.h"

#include <Translator.h>
#include <TranslatorFormats.h>
#include <TranslationDefs.h>
#include <GraphicsDefs.h>
#include <InterfaceDefs.h>
#include <DataIO.h>
#include <File.h>
#include <ByteOrder.h>

#define DOCUMENT_COUNT "/documentCount"
#define DOCUMENT_INDEX "/documentIndex"

#define PSD_TRANSLATOR_VERSION B_TRANSLATION_MAKE_VERSION(1, 0, 0)
#define PSD_IMAGE_FORMAT	'PSD '

#define PSD_IN_QUALITY		0.5
#define PSD_IN_CAPABILITY	0.5
#define BITS_OUT_QUALITY	1
#define BITS_OUT_CAPABILITY	0.9

class PSDTranslator : public BaseTranslator {
	public:
		PSDTranslator();

		virtual status_t DerivedIdentify(BPositionIO *inSource,
			const translation_format *inFormat, BMessage *ioExtension,
			translator_info *outInfo, uint32 outType);

		virtual status_t DerivedTranslate(BPositionIO *inSource,
			const translator_info *inInfo, BMessage *ioExtension,
			uint32 outType, BPositionIO *outDestination, int32 baseType);

		virtual status_t DerivedCanHandleImageSize(float width,
			float height) const;

		virtual BView *NewConfigView(TranslatorSettings *settings);	

	protected:
		virtual ~PSDTranslator();
};

extern const char *kDocumentCount;
extern const char *kDocumentIndex;

#endif	/* PSD_TRANSLATOR_H */
