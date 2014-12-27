/*
  ==============================================================================

    The interfaces to convert input String to required format

  ==============================================================================
*/

#ifndef CCZ_INPUTSTRING_CONVERTER_H
#define CCZ_INPUTSTRING_CONVERTER_H

#include "JuceHeader.h"

namespace InputStringConverter {
    unsigned long inline convertinputtoulong(const String& strOffset)
    {
        int64 offset = 0;
        if (strOffset.startsWith("0x") || strOffset.startsWith("0X")
            || strOffset.containsAnyOf("ABCDEFabcdef"))
        {
            offset = strOffset.getHexValue64();
        }
        else
        {
            offset = strOffset.getLargeIntValue();
        }
        String backDecStr((int64)offset);

        bool offsetCovertOK = (backDecStr == strOffset);
        if (!offsetCovertOK)
        {
            String oriStr = strOffset.toUpperCase();
            offsetCovertOK = (String::toHexString((int64)offset).toUpperCase() == 
                (oriStr.startsWith("0X") ? oriStr.substring(2) : oriStr));
        }

        if (!offsetCovertOK)
        {
            Logger::writeToLog("Offset can't be converted!");
            return 0;
        }
        return (unsigned long)offset;
    }

    std::vector<byte> inline convertinputbytes(const String& strBytes)
    {
        // When encountered char that is not valid hex, treat as sepereator
        // every two chars construct a byte, if only one will treat start with 0
        // that's say 123ab will treat as 123a0b, be careful
        std::vector<byte> vecbyte;
        String tstr = strBytes.trim();
#define VALID_HEX(x) (((x) >= '0' && (x) <= '9') \
    || ((x) >= 'A' && (x) <= 'F') \
    || ((x) >= 'a' && (x) <= 'f'))
        while (tstr.isNotEmpty())
        {
            String tmp("0");
            if ( ! VALID_HEX(tstr[0]))
            {
                tstr = tstr.substring(1);
                continue;
            }
            tmp += tstr[0];
            if (tstr.length() > 1 && VALID_HEX(tstr[1]))
            {
                tmp += tstr[1];
            }
            vecbyte.push_back((byte)tmp.getHexValue32());
            tstr = tstr.substring(2);
        }
#undef VALID_HEX    // only use it in this function
        return vecbyte;
    }

    String inline ConvertGBKToUtf8Str(std::vector<char> strGBK) {
        if (strGBK.empty())
        {
            return String::empty;
        }
        int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)&strGBK[0], -1, NULL,0);
        unsigned short * wszUtf8 = new unsigned short[len+1];
        memset(wszUtf8, 0, len * 2 + 2);
        MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)&strGBK[0], -1, (LPWSTR)wszUtf8, len);

        len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
        char *szUtf8=new char[len + 1];
        memset(szUtf8, 0, len + 1);
        WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);

        String rlt = String::fromUTF8(CharPointer_UTF8(szUtf8));
        delete[] szUtf8;
        delete[] wszUtf8;
        return rlt;
    }

    std::vector<char> inline ConvertStringToGBK(const String& strUtf8) {
        std::vector<char> rlt;
        int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.getCharPointer(), -1, NULL,0);
        unsigned short * wszGBK = new unsigned short[len+1];
        memset(wszGBK, 0, len * 2 + 2);
        MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.getCharPointer(), -1, (LPWSTR)wszGBK, len);

        len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
        char *szGBK=new char[len + 1];
        memset(szGBK, 0, len + 1);
        WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);

        for (int i = 0; i < len + 1; ++i)
        {
            rlt.push_back(szGBK[i]);
        }
        delete[] szGBK;
        delete[] wszGBK;
        return rlt;
    }

}

namespace UILayoutConverter{
    inline void Set_Comp_Pos(Component* c, const String& p);
    inline void Set_Comp_Size(Component* c, const String& s);
    inline String Get_UI_Text(const String& t);
}

#endif // CCZ_INPUTSTRING_CONVERTER_H