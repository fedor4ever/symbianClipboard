/*
 ============================================================================
 Name		: Clipboard.cpp
 Author	  : Fedor Strizhniou
 Copyright   : 
 *  Clipboard tools for Symbian
 Copyright (C) 2019 Fiodar Strizhniou

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
 Description : CClipboard DLL source
 ============================================================================
 */

//  Include Files  

#include <baclipb.h> //Link against: bafl.lib
#include <S32UCMP.H> //Link against: estor.lib
#include <txtetext.h>
#include <utf.h> //Link against: charconv.lib

#include "Clipboard.h"	// CClipboard
#include "Clipboard.pan"	  	// panic codes

const TUint32 KDesMaxLimit = 2<<28;
void InitRFs(RFs &fs);

void InitRFs(RFs &fs)
{
	if(fs.Handle())
		return;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	CleanupStack::Pop(); //fs
}

HBufC8* ReadFromClipboardL(RFs & aFs)
{
	bool hasFs = aFs.Handle();
	InitRFs(aFs);
    HBufC8 *result = NULL;
    CClipboard *cb = CClipboard::NewForReadingLC(aFs);

    TStreamId stid = cb->StreamDictionary().At(KClipboardUidTypePlainText);
    if (KNullStreamId != stid)
    {
        RStoreReadStream stream;
        stream.OpenLC(cb->Store(), stid);

        const TInt32 size = stream.ReadInt32L(); // first 4 byte contains clipboard size
        result = HBufC8::NewL(stream, size);

        stream.Close();
        CleanupStack::PopAndDestroy(); // stream.OpenLC
    }
    CleanupStack::PopAndDestroy(cb);

    if(hasFs)
    	aFs.Close();
    return result;
}

HBufC8* ReadFromClipboardL()
{
	TAutoClose <RFs> fs;
	return ReadFromClipboardL(fs.iObj);
}

HBufC16* ReadFromClipboardWL(RFs & aFs)
{
	bool hasFs = aFs.Handle();
	InitRFs(aFs);
    HBufC16 *result = NULL;
    CClipboard *cb = CClipboard::NewForReadingLC(aFs);

    TStreamId stid = cb->StreamDictionary().At(KClipboardUidTypePlainText);
    if (KNullStreamId != stid)
    {
        RStoreReadStream stream;
        stream.OpenLC(cb->Store(), stid);

        const TInt32 size = stream.ReadInt32L();
        result = HBufC::New(size);
        if(result)
		{
			result->Des().SetLength(size);
			TUnicodeExpander e;
			TMemoryUnicodeSink sink(&result->Des()[0]);
			e.ExpandL(sink, stream, size);
		}

        stream.Close();
        CleanupStack::Pop(); // stream.OpenLC
    }
    CleanupStack::PopAndDestroy(cb);

    if(hasFs)
    	aFs.Close();
    return result;
}

HBufC16* ReadFromClipboardWL()
{
	TAutoClose <RFs> fs;
	return ReadFromClipboardWL(fs.iObj);
}

void WriteToClipboardL(RFs &aFs, const TDesC8 & aBinary)
{
	bool hasFs = aFs.Handle();
	InitRFs(aFs);
    CClipboard *cb = CClipboard::NewForWritingLC(aFs);

    RStoreWriteStream stream;
    TStreamId stid = stream.CreateLC(cb->Store());
    
    stream.WriteInt32L(aBinary.Length());
    stream.WriteL(aBinary);
    stream.WriteInt8L(0); // magic command! :)

    stream.CommitL();
    cb->StreamDictionary().AssignL(KClipboardUidTypePlainText, stid);
    cb->CommitL();

    stream.Close();
    CleanupStack::PopAndDestroy(); // stream.CreateLC
    CleanupStack::PopAndDestroy(cb);

    if(hasFs)
    	aFs.Close();
}

void WriteToClipboardL(RFs &aFs, const TDesC16 & aText)
{
	bool hasFs = aFs.Handle();
	InitRFs(aFs);
    CClipboard *cb = CClipboard::NewForWritingLC(aFs);

    RStoreWriteStream stream;
    TStreamId stid = stream.CreateLC(cb->Store());
    stream.WriteInt32L(aText.Length());

    TUnicodeCompressor c;
    TMemoryUnicodeSource source(aText.Ptr());
    TInt bytes(0);
    TInt words(0);
    c.CompressL(stream, source, KMaxTInt, aText.Length(), &bytes, &words);

    stream.WriteInt8L(0); // magic command! :)

    stream.CommitL();
    cb->StreamDictionary().AssignL(KClipboardUidTypePlainText, stid);
    cb->CommitL();

    stream.Close();
    CleanupStack::PopAndDestroy(); // stream.CreateLC
    CleanupStack::PopAndDestroy(cb);

    if(hasFs)
    	aFs.Close();
}

void WriteToClipboardL(const TDesC16 & dest)
{
	TAutoClose <RFs> fs;
	return WriteToClipboardL(fs.iObj, dest);;
}

void WriteToClipboardL(const TDesC8 & dest)
{
	TAutoClose <RFs> fs;
	return WriteToClipboardL(fs.iObj, dest);
}

char* StrFromClb() //utf-8 string
{
	char *arr = NULL;
	
	TRAPD( err,
	HBufC16* str = ReadFromClipboardWL();
	CleanupStack::PushL(str);
	HBufC8* utf8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L(*str);
	CleanupStack::PopAndDestroy(str);
	CleanupStack::PushL(utf8);
	
	TInt strSize = utf8->Size();
	arr = (char *)User::Alloc(strSize + 1);
	arr[strSize + 1] = '0';
	utf8->Des().Copy((TUint8*)arr, strSize);
			);
	if((err != KErrNone) && arr)
	{
		User::Free(arr);
		arr = NULL;
	}

	return arr;
}

char* BinFromClb()
{
	char *arr = NULL;
	
	TRAPD(err,
	HBufC8 *buf = ReadFromClipboardL();
	TInt strSize = buf->Size();
	
	arr = (char *)User::Alloc(strSize + 1);
	arr[strSize + 1] = '0';
	buf->Des().Copy((TUint8*)arr, strSize);
			);
	if((err != KErrNone) && arr)
	{
		User::Free(arr);
		arr = NULL;
	}

	return arr;
}

TInt StrToClb(const char* utf8str)
{
	TRAPD(err,
	HBufC16* str =
			CnvUtfConverter::ConvertToUnicodeFromUtf8L(TPtrC8( (TUint8 *)utf8str ));
	CleanupStack::PushL(str);
	WriteToClipboardL(*str);
	CleanupStack::PopAndDestroy();
			);
	return err;
}

TInt BinToClb(const char* binary)
{
	TRAPD(err,
	WriteToClipboardL(TPtrC8( (TUint8 *)binary) );
			);
	return err;
}

