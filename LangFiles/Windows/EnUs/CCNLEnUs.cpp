// CCNLEnUs.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

/// the language code of this language file.
static const char LANGUAGE_CODE[] = "enUS";

/// the name of this language.
static const char LANGUAGE_NAME[] = "English (US)";

/// the language identifier
static const LANGID LANGUAGE_ID = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

/**
 *	@brief	Returns the version of spec on which this language file is based.
 *	@return	version number
 */
DWORD APIENTRY CCNLGetVersion()
{
	return 6;
}

/**
 *	@brief		Retrieves the language code.
 *	@param[out]	szLangCode	If this parameter is not NULL, the language code string is returned.
 *	@return		The length of language code, not include terminating null character.
 *	@note		If szLangCode paramter is NULL, this function simply returns the length of the language code.
 */
DWORD APIENTRY CCNLGetLanguageCode(char* szLangCode)
{
	if (NULL != szLangCode)
	{
		strcpy(szLangCode, LANGUAGE_CODE);
	}
	return strlen(LANGUAGE_CODE);
}

/**
 *	@brief		Retrieves the name of this language.
 *
 *	The name is written in English.
 *
 *	@param[out]	szLangName	If this parameter is not NULL, the language name is returned.
 *	@return		The length of language name, not include terminating null character.
 *	@note		If szLangName paramter is NULL, this function simply returns the length of the language name.
 */
DWORD APIENTRY CCNLGetLanguageName(char* szLangName)
{
	if (NULL != szLangName)
	{
		strcpy(szLangName, LANGUAGE_NAME);
	}
	return strlen(LANGUAGE_NAME);
}

/**
 *	@brief		Retrieves the language identifier of this language.
 *	@return		The language identifier
 */
UINT APIENTRY CCNLGetLangID()
{
	return LANGUAGE_ID;
}
