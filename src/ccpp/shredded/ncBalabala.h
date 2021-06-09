#pragma once

char* StrCpy(char* dest, const char* src)
{
    if(dest == nullptr || src == nullptr)
    {
        return nullptr;
    }

    char* destTemp = dest;
    while ((*dest++ = *src++) != '\0')
        ;

    *dest = '\0';
    return destTemp;
}

void* MemCpy(void* dest, const void* src, size_t n)
{
    if(dest == nullptr || src == nullptr)
    {
        return nullptr;
    }

    char* tempDest = (char*)dest;
    char* tempSrc = (char*)src;

    if(dest>src || (char*)dest >= (char*)src+n)
    {
        for(size_t i=n-1; i!=-1; i--)
        {
            tempDest[i] = tempSrc[i];
        }
    }
    else
    {
        for(size_t i=0; i<n; i++)
        {
            tempDest[i] = tempSrc[i];
        }
    }

    return dest;
}