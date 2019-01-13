/*
 ============================================================================
 Name		: ClipboardDll.cpp 
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
 Description : ClipboardDll.cpp - main DLL source
 ============================================================================
 */

//  Include Files  

#include <e32std.h>		 // GLDEF_C
#include "Clipboard.pan"		// panic codes

//  Global Functions

GLDEF_C void Panic(TClipboardPanic aPanic)
// Panics the thread with given panic code
	{
	User::Panic(_L("Clipboard"), aPanic);
	}

//  Exported Functions

#ifndef EKA2 // for EKA1 only
EXPORT_C TInt E32Dll(TDllReason /*aReason*/)
// Called when the DLL is loaded and unloaded. Note: have to define
// epoccalldllentrypoints in MMP file to get this called in THUMB.
	{
	return KErrNone;
	}
#endif

