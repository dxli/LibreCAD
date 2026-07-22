/******************************************************************************
 *
 * Project:  Shapelib
 * Purpose:  Default implementation of file io based on stdio.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2007, Frank Warmerdam
 * Copyright (c) 2016-2024, Even Rouault <even dot rouault at spatialys.com>
 *
 * SPDX-License-Identifier: MIT OR LGPL-2.0-or-later
 ******************************************************************************
 *
 */

#include "shapefil_private.h"

#include <assert.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SHPAPI_UTF8_HOOKS
#ifdef SHPAPI_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#pragma comment(lib, "kernel32.lib")
#endif
#endif

/* LibreCAD delta (see libraries/shapelib/README.librecad):
 * Old-style C casts in this file replaced with reinterpret_cast/static_cast
 * so a C++17 compile with -Wold-style-cast stays clean.  Cast targets are
 * unchanged; semantics identical to upstream.
 */
static SAFile SADFOpen(const char *pszFilename, const char *pszAccess,
                       void *pvUserData)
{
    (void)pvUserData;
    return reinterpret_cast<SAFile>(fopen(pszFilename, pszAccess));
}

static SAOffset SADFRead(void *p, SAOffset size, SAOffset nmemb, SAFile file)
{
    return static_cast<SAOffset>(fread(p, static_cast<size_t>(size),
                                       static_cast<size_t>(nmemb),
                                       reinterpret_cast<FILE *>(file)));
}

static SAOffset SADFWrite(const void *p, SAOffset size, SAOffset nmemb,
                          SAFile file)
{
    return static_cast<SAOffset>(fwrite(p, static_cast<size_t>(size),
                                        static_cast<size_t>(nmemb),
                                        reinterpret_cast<FILE *>(file)));
}

static SAOffset SADFSeek(SAFile file, SAOffset offset, int whence)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    return static_cast<SAOffset>(
        _fseeki64(reinterpret_cast<FILE *>(file),
                  static_cast<__int64>(offset), whence));
#else
    return static_cast<SAOffset>(
        fseek(reinterpret_cast<FILE *>(file),
              static_cast<long>(offset), whence));
#endif
}

static SAOffset SADFTell(SAFile file)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400
    return static_cast<SAOffset>(_ftelli64(reinterpret_cast<FILE *>(file)));
#else
    return static_cast<SAOffset>(ftell(reinterpret_cast<FILE *>(file)));
#endif
}

static int SADFFlush(SAFile file)
{
    return fflush(reinterpret_cast<FILE *>(file));
}

static int SADFClose(SAFile file)
{
    return fclose(reinterpret_cast<FILE *>(file));
}

static int SADRemove(const char *filename, void *pvUserData)
{
    (void)pvUserData;
    return remove(filename);
}

static void SADError(const char *message)
{
    fprintf(stderr, "%s\n", message);
}

void SASetupDefaultHooks(SAHooks *psHooks)
{
    psHooks->FOpen = SADFOpen;
    psHooks->FRead = SADFRead;
    psHooks->FWrite = SADFWrite;
    psHooks->FSeek = SADFSeek;
    psHooks->FTell = SADFTell;
    psHooks->FFlush = SADFFlush;
    psHooks->FClose = SADFClose;
    psHooks->Remove = SADRemove;

    psHooks->Error = SADError;
    psHooks->Atof = atof;
    psHooks->pvUserData = SHPLIB_NULLPTR;
}

#ifdef SHPAPI_WINDOWS

static wchar_t *Utf8ToWideChar(const char *pszFilename)
{
    const int nMulti = static_cast<int>(strlen(pszFilename)) + 1;
    const int nWide =
        MultiByteToWideChar(CP_UTF8, 0, pszFilename, nMulti, 0, 0);
    if (nWide == 0)
    {
        return NULL;
    }
    wchar_t *pwszFileName =
        static_cast<wchar_t *>(malloc(nWide * sizeof(wchar_t)));
    if (pwszFileName == NULL)
    {
        return NULL;
    }
    if (MultiByteToWideChar(CP_UTF8, 0, pszFilename, nMulti, pwszFileName,
                            nWide) == 0)
    {
        free(pwszFileName);
        return NULL;
    }
    return pwszFileName;
}

/************************************************************************/
/*                           SAUtf8WFOpen                               */
/************************************************************************/

static SAFile SAUtf8WFOpen(const char *pszFilename, const char *pszAccess,
                           void *pvUserData)
{
    (void)pvUserData;
    SAFile file = NULL;
    wchar_t *pwszFileName = Utf8ToWideChar(pszFilename);
    wchar_t *pwszAccess = Utf8ToWideChar(pszAccess);
    if (pwszFileName != NULL && pwszAccess != NULL)
    {
        file = reinterpret_cast<SAFile>(_wfopen(pwszFileName, pwszAccess));
    }
    free(pwszFileName);
    free(pwszAccess);
    return file;
}

static int SAUtf8WRemove(const char *pszFilename, void *pvUserData)
{
    (void)pvUserData;
    wchar_t *pwszFileName = Utf8ToWideChar(pszFilename);
    int rc = -1;
    if (pwszFileName != NULL)
    {
        rc = _wremove(pwszFileName);
    }
    free(pwszFileName);
    return rc;
}

#endif

#ifdef SHPAPI_UTF8_HOOKS
#ifndef SHPAPI_WINDOWS
#error "no implementations of UTF-8 hooks available for this platform"
#endif

void SASetupUtf8Hooks(SAHooks *psHooks)
{
    psHooks->FOpen = SAUtf8WFOpen;
    psHooks->Remove = SAUtf8WRemove;
    psHooks->FRead = SADFRead;
    psHooks->FWrite = SADFWrite;
    psHooks->FSeek = SADFSeek;
    psHooks->FTell = SADFTell;
    psHooks->FFlush = SADFFlush;
    psHooks->FClose = SADFClose;

    psHooks->Error = SADError;
    psHooks->Atof = atof;
    psHooks->pvUserData = SHPLIB_NULLPTR;
}
#endif
