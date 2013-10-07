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


#ifndef PSD_LOADER_H
#define PSD_LOADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Translator.h>
#include <TranslatorFormats.h>
#include <TranslationDefs.h>
#include <GraphicsDefs.h>
#include <InterfaceDefs.h>
#include <DataIO.h>
#include <File.h>
#include <ByteOrder.h>
#include <List.h>


#include "libpsd.h"
#include "psd_color.h"

#define PSD_DECODE_MERGED	0x01
#define PSD_DECODE_LAYERS	0x02
#define PSD_DECODE_ALL		0x03

class PSDLoader {
public:
					PSDLoader(BPositionIO *stream, uint32 flags = PSD_DECODE_ALL);
					~PSDLoader();
		
	int				LayersCount(void);
	int 			GetImage(BPositionIO *target, int index);
	bool			IsLoaded(void);
private:	
	psd_context*	fPSDContext;
	uint32			fFlags;
	int				fLayersCount;
	size_t			fStreamSize;
	BList			fLayersList;
	bool			fLoaded;
};


#endif	/* PSD_LOADER_H */
