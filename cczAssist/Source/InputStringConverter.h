/*
  ==============================================================================

    The interfaces to convert input String to required format

  ==============================================================================
*/

#ifndef CCZ_INPUTSTRING_CONVERTER_H
#define CCZ_INPUTSTRING_CONVERTER_H

#include "JuceHeader.h"

namespace InputStringConverter {
    unsigned long convetinputtoulong(const String& strOffset)
    {
        unsigned long offset = 0;
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
        return offset;
    }

    std::vector<byte> convertinputbytes(const String& strBytes)
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
            vecbyte.push_back(tmp.trimCharactersAtStart("0").getHexValue32());
            tstr = tstr.substring(2);
        }
#undef VALID_HEX    // only use it in this function
        return vecbyte;
    }

}

#endif // CCZ_INPUTSTRING_CONVERTER_H