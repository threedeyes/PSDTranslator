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


#include "PSDLoader.h"
#include "BaseTranslator.h"


PSDLoader::PSDLoader(BPositionIO *stream, uint32 flags)
{
	fFlags = flags;
	fLoaded = false;
	fLayersCount = 0;

	stream->Seek(0, SEEK_END);
	fStreamSize = stream->Position();
	stream->Seek(0, SEEK_SET);

	if (fStreamSize <= 0)
		return;

	if (fFlags&PSD_DECODE_ALL) {
		psd_status status = psd_image_load(&fPSDContext, stream);
		if (status != psd_status_done)
			return;
		fLayersCount++;
	} else if (fFlags&PSD_DECODE_MERGED) {
		psd_status status = psd_image_load_merged(&fPSDContext, stream);
		if (status != psd_status_done)
			return;
		fLayersCount++;
	} else if (fFlags&PSD_DECODE_LAYERS) {
		psd_status status = psd_image_load_layer(&fPSDContext, stream);
		if (status != psd_status_done)
			return;
	}
	
	if (fFlags&PSD_DECODE_LAYERS) {
		for (int i=0; i < fPSDContext->layer_count; i++) {
			psd_layer_record* newRecord = &fPSDContext->layer_records[i];
			if (newRecord->layer_type == psd_layer_type_normal) {
				fLayersList.AddItem(newRecord);
				fLayersCount++;
			}
		}
	}

	fLoaded = true;	
}


PSDLoader::~PSDLoader()
{
	if (fPSDContext != NULL)
		psd_image_free(fPSDContext);
}


bool
PSDLoader::IsLoaded(void)
{
	return fLoaded;
}
	
	
int
PSDLoader::LayersCount(void)
{
	return fLayersCount;
}


int
PSDLoader::GetImage(BPositionIO *target, int index)
{
	if (index < 1 || index > fLayersCount + 1 || !fLoaded)
		return B_NO_TRANSLATOR;

	int width = fPSDContext->width;
	int height = fPSDContext->height;
	
	psd_layer_record* layer = NULL;
	psd_argb_color* imageData = fPSDContext->merged_image_data;
	
	if (!fFlags&PSD_DECODE_MERGED)
		index++;

	if (index > 1) {
		layer = (psd_layer_record*)fLayersList.ItemAt(index - 2);
		width = layer->width;
		height = layer->height;
		imageData = layer->image_data;
	}
	
	TranslatorBitmap bitsHeader;
	bitsHeader.magic = B_TRANSLATOR_BITMAP;
	bitsHeader.bounds.left = 0;
	bitsHeader.bounds.top = 0;
	bitsHeader.bounds.right = width - 1;
	bitsHeader.bounds.bottom = height - 1;
	bitsHeader.rowBytes = sizeof(uint32) * width;
	bitsHeader.colors = B_RGBA32;
	bitsHeader.dataSize = bitsHeader.rowBytes * height;
	if (swap_data(B_UINT32_TYPE, &bitsHeader,
		sizeof(TranslatorBitmap), B_SWAP_HOST_TO_BENDIAN) != B_OK) {		
		return B_NO_TRANSLATOR;
	}
	target->Write(&bitsHeader, sizeof(TranslatorBitmap));	
	
	uint8 *rowBuff = new uint8[width * sizeof(uint32)];
	for (int i = 0; i < height; i++) {
		psd_argb_color *rowData = imageData + (i * width);
		uint8 *rowBuffPtr = rowBuff;
		for (int j=0; j < width; j++, rowData++) {			
			rowBuffPtr[2] = PSD_GET_RED_COMPONENT(*rowData);
			rowBuffPtr[1] = PSD_GET_GREEN_COMPONENT(*rowData);
			rowBuffPtr[0] = PSD_GET_BLUE_COMPONENT(*rowData);
			rowBuffPtr[3] = PSD_GET_ALPHA_COMPONENT(*rowData);
			rowBuffPtr += sizeof(uint32);			
		}
		target->Write(rowBuff, width * sizeof(uint32));
	}				
	delete rowBuff;		

	return B_OK;
}
