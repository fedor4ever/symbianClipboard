/*
 ============================================================================
 Name		: Clipboard.h
 Author	  : Fedor Strizhniou
 Copyright   : 
 *  Clipboard tools for Symbian
 Copyright (C) 2018 Fiodar Strizhniou

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/
 Description : Clipboard.h - CClipboard class header
 ============================================================================
 */

// This file defines the API for Clipboard.dll

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

//  Include Files

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <f32file.h> // RFs

//  Function Definitions

#ifdef __cplusplus
// These function allocate memory, release memeory after use
EXPORT_C HBufC8* ReadFromClipboardL(RFs& fs);
EXPORT_C HBufC8* ReadFromClipboardL();

EXPORT_C HBufC16* ReadFromClipboardWL(RFs& fs);
EXPORT_C HBufC16* ReadFromClipboardWL();


EXPORT_C void WriteToClipboardL(RFs& fs, const TDesC8 & dest);
EXPORT_C void WriteToClipboardL(RFs& fs, const TDesC16& dest);

EXPORT_C void WriteToClipboardL(const TDesC8 & dest);
EXPORT_C void WriteToClipboardL(const TDesC16& dest);

#endif // __cplusplus

extern "C" {
EXPORT_C char* StrFromCbL(); //utf-8 string
EXPORT_C char* BinFromCbL();

EXPORT_C void StrToCbL(const char* utf8str);
EXPORT_C void BinToCbL(const char* binary);
}


#endif  // __CLIPBOARD_H__

