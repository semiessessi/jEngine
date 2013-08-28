// for handling i/o to text files with unicode support
// this just converts everything to the UCS2 representation of ISO 10646 characters
// this should operate nicely across the ms and gnu compilers with their 16 and 32-bit wchar_t types respectively
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifndef MAX_PATH
#define MAX_PATH 255
#endif

enum jTextFormat
{
	JTEXT_UTF8,
	JTEXT_UTF16_LE,
	JTEXT_UTF16_BE,
	JTEXT_UTF32_LE,
	JTEXT_UTF32_BE
};

class jTextFile
{
private:
	wchar_t			path[MAX_PATH];
	wchar_t*		data;
	unsigned int 	length;
	jTextFormat		format;

    unsigned int    readPos;
public:
	jTextFile()
	{
		for(unsigned int i = 0; i < MAX_PATH; ++i) path[i] = 0;
		data = 0;
		length = 0;
		format = JTEXT_UTF8;
        readPos = 0;
	}
	
	~jTextFile()
	{
		if(data) free(data);
	}

    void SetFormat(const jTextFormat& _format) { format = _format; }

    jTextFormat GetEncoding() const { return format; }
    wchar_t* GetPath() { return path; }
    wchar_t* GetString() const { return data; }
    unsigned int GetStringLength() const { return length; }

	bool Load(const wchar_t* _path)
	{
		FILE* f = _wfopen(_path, L"rb");
		if(!f) return false;
        
        memset(path, 0, 255*sizeof(wchar_t));
		wcscpy(path, _path);
		
		// read all data out to temporary buffer
		fseek(f, 0, SEEK_END);
		unsigned int dataLength = ftell(f);
		fseek(f, 0, SEEK_SET);
	
		unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char)*dataLength);
		if(!buf) return false;
		
		memset(buf, 0, sizeof(char)*dataLength);
		
		fread(buf, sizeof(char), dataLength, f);
		
		fclose(f);
		// done with file
		
		// check BOM to identify type, but assume UTF-8 as default
		unsigned int pos = 0;
		format = JTEXT_UTF8;
		
		if(buf[0] == 0xEF)	// could be the UTF-8 BOM
		{
			if((buf[1] == 0xBB) && (buf[2] == 0xBF))
			{
				// make sure we don't overwrite format... even though should never happen
				format = JTEXT_UTF8;
				pos = 3;
				goto BOM_identified;
			}
		}
		
		if(buf[0] == 0xFF) // could be the little endian UTF-16 or UTF-32 BOM
		{
			if(buf[1] == 0xFE) 
			{
				if((buf[2] == 0x00) && (buf[3] == 0x00))
				{
					format = JTEXT_UTF32_LE;
					pos = 4;
					goto BOM_identified;
				}
				else
				{
					format = JTEXT_UTF16_LE;
					pos = 2;
					goto BOM_identified;
				}
			}
		}
		
		if((buf[0] == 0xFE) && (buf[1] == 0xFF)) // this is the big endian UTF-16 BOM
		{
			format = JTEXT_UTF16_BE;
			pos = 2;
			goto BOM_identified;
		}
		
		if((buf[0] == 0x00) && (buf[1] == 0x00) && (buf[2] == 0xFE) && (buf[3] == 0xFF)) // this is the big endian UTF-32 BOM
		{
			format = JTEXT_UTF32_BE;
			pos = 4;
			goto BOM_identified;
		}
		
BOM_identified:
		
		// now convert all into UCS2 for internal use
		// allocate data so that it is at least as many wchar_t's as there were char's so that UTF8 will always fit
		// we get some extra leeway from the BOM as well... so we get at least 1 extra wchar_t
		// track the length as the text is decoded then realloc the wchar_t array at the end...
		// using sizeof(wchar_t) is important... it varies between MS C++ and GNU C++ compilers
		
		data = (wchar_t*)realloc(data, sizeof(wchar_t) * dataLength);
		memset(data, 0, sizeof(wchar_t) * dataLength);
		
		unsigned int dataPos = 0;
		
		switch(format)
		{
			case JTEXT_UTF8:
			// go through byte by byte and add to data if less than 0x80, otherwise decode
			// note that this decodes the character properly, then throws away any chars that can't be represented by jEngine
			// should meet requirements mentioned in RFC3629 - http://tools.ietf.org/html/rfc3629
			for(unsigned int i = pos; i < dataLength; ++i)
			{
				if(buf[i] < 0x80)
				{
					data[dataPos] = (wchar_t)(buf[i]);
				}
				else
				{
					// check how many bytes in sequence encoded and break out into their own array
					unsigned int numBytes = 0;
					unsigned int tempChar = 0;

					if(buf[i] & 0x80) ++numBytes;
					if(buf[i] & 0x40) ++numBytes;
					if(buf[i] & 0x20) ++numBytes;
					if(buf[i] & 0x10) ++numBytes; // allows up to 4 bytes of UTF-8

					unsigned char	bytes[4];
					for(unsigned int j = 0; j < numBytes; ++j) bytes[j] = buf[i+j];

					// we need to take the lowest 6-bits from each byte, from lowest to highest and concatenate them into the UCS16 2-bytes
					// anything outside of the UCS2 range should be replaced with a "?" for now.
					i += (numBytes - 1);
					unsigned int k = numBytes-1;
					// shift each of the trailing bytes and add as appropriate
					for(unsigned int l = 0; k > 0; --k, l += 6)
					{
						tempChar |= ((unsigned int)(bytes[k] & 0x3F) << l);
					}
					
                    const unsigned char mask[] = { 0xFF, 0x7F, 0x3F, 0x1F, 0x0F };
					// now take the leading byte and add the bits from it
					tempChar |= ((unsigned int)(bytes[k] & mask[numBytes]) << ((numBytes - 1)*6));
					
					// check if the codepoint is valid and supported by jEngine and add the appropriate character to the data
					if(tempChar > 0xFFFF)
                    {
                        data[dataPos] = L'?';
                    }
					else data[dataPos] = (wchar_t)tempChar;
				}
				++dataPos;
			}			
			break;
			
			case JTEXT_UTF16_LE:
			// go through 2 bytes at a time and decode
			// as with UTF-8 decoder above this will fully decode the character before converting to UCS2
			// this is horrible... UCS2 != UTF-16 despite the common opinion. so the "obvious shortcut" is in fact a deathtrap
			// should meet requirements mentioned in RFC2781 - http://tools.ietf.org/html/rfc2781
			for(unsigned int i = pos; i < dataLength; i += 2)
			{
				unsigned short c = ((unsigned short)(buf[i+1]) << 8) | (unsigned short)(buf[i]);
				if((c < 0xDC00) || (c > 0xDFFF))
				{
					// single word character - just dump straight into the data
					data[dataPos] = (wchar_t)c;
				}
				else if (c > 0xDBFF) // pick out some invalid characters and complain with a "?" for now...
				{
					data[dataPos] = L'?';
				}
				else
				{
					// two word character (higher number of words are essentially forbidden)
					// we have the low-order word in c...
					i += 2;
					unsigned int tempChar = ((unsigned int)(((unsigned short)(buf[i+3]) << 8) | (unsigned short)(buf[i+2])) & 0xFFFF) << 10;
					tempChar = tempChar | (((unsigned int)c) & 0xFFFF);
					
					// Unicode allows for more code points... but jEngine does not... at least under Windows :(
					if(tempChar > 0xFFFF)
					{
							data[dataPos] = L'?';
					}
					else
					{
						data[dataPos] = (wchar_t)tempChar;
					}
					
				}
				++dataPos;
			}
			break;
			
			case JTEXT_UTF16_BE:
			// this is just the above with the byte order swapped. should work... but needs testing
			for(unsigned int i = pos; i < dataLength; i += 2)
			{
				unsigned short c = ((unsigned short)(buf[i]) << 8) | (unsigned short)(buf[i+1]);
				if((c < 0xDC00) || (c > 0xDFFF))
				{
					// single word character - just dump straight into the data
					data[dataPos] = (wchar_t)c;
				}
				else if (c > 0xDBFF) // pick out some invalid characters and complain with a "?" for now...
				{
					data[dataPos] = L'?';
				}
				else
				{
					// two word character (higher number of words are essentially forbidden)
					// we have the low-order word in c...
					i += 2;
					unsigned int tempChar = ((unsigned int)(((unsigned short)(buf[i+2]) << 8) | (unsigned short)(buf[i+3])) & 0xFFFF) << 10;
					tempChar = tempChar | (((unsigned int)c) & 0xFFFF);
					
					// Unicode allows for more code points... but jEngine does not... at least under Windows :(
					if(tempChar > 0xFFFF)
					{
							data[dataPos] = L'?';
					}
					else
					{
						data[dataPos] = (wchar_t)tempChar;
					}
					
				}
				++dataPos;
			}		
			break;
			
			case JTEXT_UTF32_LE:
			// this roughly complies with the Unicode Standard 4.1 section 3.10, D43-45
			// however to allow all jEngine builds to handle the results it throws away all code points > 0xFFFF and replaces them with ? (for now)
			for(unsigned int i = pos; i < dataLength; i += 4)
			{
				// construct the character
				unsigned int tempChar = (unsigned int)(buf[i])
											| (unsigned int)(buf[i+1]) << 8
											| (unsigned int)(buf[i+2]) << 16
											| (unsigned int)(buf[i+3]) << 24;
											
				if(tempChar > 0xFFFF)
				{
						data[dataPos] = L'?';
				}
				else
				{
					data[dataPos] = (wchar_t)tempChar;
				}
			}
			break;
			
			case JTEXT_UTF32_BE:
			// just swapped byte order from above
			for(unsigned int i = pos; i < dataLength; i += 4)
			{
				// construct the character
				unsigned int tempChar = (unsigned int)(buf[i+3])
											| (unsigned int)(buf[i+2]) << 8
											| (unsigned int)(buf[i+1]) << 16
											| (unsigned int)(buf[i]) << 24;
											
				if(tempChar > 0xFFFF)
				{
						data[dataPos] = L'?';
				}
				else
				{
					data[dataPos] = (wchar_t)tempChar;
				}
			}
			break;
			
		}
		
		// add trailing null and reallocate data store
		++dataPos;
		data = (wchar_t*)realloc(data, sizeof(wchar_t)*(dataPos));
		data[dataPos - 1] = 0;
        length = dataPos;
		
		// if data is not null then consider it a success
		if(data) return true;
		
		return false;
	}
	
	bool New(const wchar_t* _path, jTextFormat _format = JTEXT_UTF8)
	{
		FILE* f = _wfopen(_path, L"wb");
		if(!f) return false;
		
		// insert the BOM unless its UTF-8 as the Unicode Standard 4.1 advises against using a BOM with UTF-8
		unsigned char	bom[4] = { 0xFF, 0xFE, 0x00, 0x00 };
		switch(_format)
		{
			default:
                return false; // bad format
			case JTEXT_UTF8:
			break;
			
			case JTEXT_UTF16_LE:
			fwrite(&(bom[0]), sizeof(char), 1, f);
			fwrite(&(bom[1]), sizeof(char), 1, f);
			break;
			
			case JTEXT_UTF16_BE:
			fwrite(&(bom[1]), sizeof(char), 1, f);
			fwrite(&(bom[0]), sizeof(char), 1, f);
			break;
			
			case JTEXT_UTF32_LE:
			fwrite(&(bom[0]), sizeof(char), 1, f);
			fwrite(&(bom[1]), sizeof(char), 1, f);
			fwrite(&(bom[2]), sizeof(char), 1, f);
			fwrite(&(bom[3]), sizeof(char), 1, f);
			break;
			
			case JTEXT_UTF32_BE:
			fwrite(&(bom[3]), sizeof(char), 1, f);
			fwrite(&(bom[2]), sizeof(char), 1, f);
			fwrite(&(bom[1]), sizeof(char), 1, f);
			fwrite(&(bom[0]), sizeof(char), 1, f);
			break;
		}
		
		fclose(f);
		
        memset(path, 0, 255*sizeof(wchar_t));
		wcscpy(path, _path);
		format = _format;
		
		// reset data
		if(data) free(data);
		data = 0;
		length = 0;
		
		return true;
	}

	bool Append(const wchar_t* _string, bool _save = true)
	{
		// measure string
		unsigned int t = wcslen(_string);
		
		if(!t) return false;
		
		// reallocate
		data = (wchar_t*)realloc(data, sizeof(wchar_t)*(length + t));
		
		if(!data) return false;
		
		// copy
		for(unsigned int i = 0; i < t; ++i) data[length + i] = _string[i];
		
		// adjust the stored length
		length += t;
		
		return _save ? Save() : true;
	}
	
	bool AppendLine(const wchar_t* _string, bool _save = true)
	{
		// measure string
		unsigned int t = wcslen(_string) + 2;
		
		if(!t) return false;
		
		// reallocate
		data = (wchar_t*)realloc(data, sizeof(wchar_t)*(length + t));
		
		if(!data) return false;
		
		// copy
		for(unsigned int i = 0; i < (t - 2); ++i) data[length + i] = _string[i];
		
		// adjust the stored length
		length += t;
		
		// windows compatible line ending
		data[length-2] = '\r';
		data[length-1] = '\n';
				
        return _save ? Save() : true;
	}
	
	bool Save() const
	{
		FILE* f = _wfopen(path, L"wb");
		if(!f) return false;

        // insert the BOM unless its UTF-8 as the Unicode Standard 4.1 advises against using a BOM with UTF-8
		unsigned char	bom[4] = { 0xFF, 0xFE, 0x00, 0x00 };
		switch(format)
		{
			default:
                return false; // bad format
			case JTEXT_UTF8:
			    break;
			
			case JTEXT_UTF16_LE:
			    fwrite(&(bom[0]), sizeof(char), 1, f);
			    fwrite(&(bom[1]), sizeof(char), 1, f);
			    break;
			
			case JTEXT_UTF16_BE:
			    fwrite(&(bom[1]), sizeof(char), 1, f);
			    fwrite(&(bom[0]), sizeof(char), 1, f);
			    break;
			
			case JTEXT_UTF32_LE:
			    fwrite(&(bom[0]), sizeof(char), 1, f);
			    fwrite(&(bom[1]), sizeof(char), 1, f);
			    fwrite(&(bom[2]), sizeof(char), 1, f);
			    fwrite(&(bom[3]), sizeof(char), 1, f);
			    break;
			
			case JTEXT_UTF32_BE:
			    fwrite(&(bom[3]), sizeof(char), 1, f);
			    fwrite(&(bom[2]), sizeof(char), 1, f);
			    fwrite(&(bom[1]), sizeof(char), 1, f);
			    fwrite(&(bom[0]), sizeof(char), 1, f);
			    break;
		}

        unsigned int* data32 = 0;
		switch(format)
		{
			default:
                return false; // format not supported for output
			case JTEXT_UTF8:
                for(unsigned int i = 0; i < length; ++i)
                {
                    if((unsigned short)data[i] < 0x80)
                    {
                        fwrite(&(data[i]), sizeof(char), 1, f);
                    }
                    else if((unsigned short)data[i] < 0x800)
                    {
                        // we have two bytes
                        char c[2] = { 0xC0 | (char)(data[i] >> 7), 0x80 | (char)(data[i] & 0x7F) };
                        fwrite(c, sizeof(char), 2, f);
                    }
                    else // if((unsigned short)data[i] < 0x10000)
                    {
                        // we have three bytes
                        char c[3] = { 0xE0 | (char)(data[i] >> 14), 0x80 | (char)((data[i] >> 7)  & 0x7F), 0x80 | (char)(data[i] & 0x7F) };
                        fwrite(c, sizeof(char), 3, f);
                    }
                    /*
                    else if((unsigned short)data[i] < 0x100000)
                    {
                        // we have four bytes
                        // ...
                    } 
                    */
                }
			    break;
            case JTEXT_UTF16_LE:
                // there should never be any surrogate pairs in here... so why not just dump it all
                fwrite(data, sizeof(wchar_t), length, f);
                break;
            case JTEXT_UTF16_BE:
                // switch every other byte then do the same as with the little endian version
                for(unsigned int i = 0; i < length; ++i) data[i] = (data[i] << 8) | (data[i] >> 8);
                fwrite(data, sizeof(wchar_t), length, f);
                for(unsigned int i = 0; i < length; ++i) data[i] = (data[i] << 8) | (data[i] >> 8);
                break;
            case JTEXT_UTF32_LE:
                data32 = new unsigned int[length];
                for(unsigned int i = 0; i < length; ++i) data32[i] = (unsigned int)data[i];
                fwrite(data32, sizeof(unsigned int), length, f);
                delete[] data32;
                break;
            case JTEXT_UTF32_BE:
                data32 = new unsigned int[length];
                for(unsigned int i = 0; i < length; ++i) data32[i] = ((unsigned int)((data[i] << 8) | (data[i] >> 8))) << 16;
                fwrite(data32, sizeof(unsigned int), length, f);
                delete[] data32;
                break;
		}
		
		fclose(f);
		return true;
	}

	bool SaveAs(const wchar_t* _path)
    {
        memset(path, 0, 255*sizeof(wchar_t));
        wcscpy(path, _path);

        return Save();
    }

    wchar_t* ReadLine()
    {
        if(readPos)
        {
            data[readPos] = L'\r';
            readPos += 2;
            if(readPos > length)
            {
                ReadDone();
                return 0;
            }
        }
        wchar_t* ret = &(data[readPos]);
        while((data[readPos] != L'\r') && (data[readPos] != 0))
        {
            if(readPos > length)
            {
                ReadDone();
                return 0;
            }
            ++readPos;
        }
        data[readPos] = 0;
        return ret;
    }

    void ReadDone()
    {
        // if(readPos) data[readPos] = L'\r';
        readPos = 0;
    }

    bool ReadFormat(wchar_t* _formatString, void* _match1 = 0, void* _match2 = 0, void* _match3 = 0, void* _match4 = 0)
    {
        // this is a bit lame and uses TEMPORARY FILES 
        // since i can't find how to get the length of the string scanned by scanf...
        wchar_t* str = &(data[readPos]);
        FILE* f = tmpfile();
        rewind(f);
        fputws(str, f);
        rewind(f);
        unsigned int pos = ftell(f);
        int ret = 0;
        if(_match4) ret = fwscanf(f, _formatString, _match1, _match2, _match3, _match4);
        else if(_match3) ret = fwscanf(f, _formatString, _match1, _match2, _match3);
        else if(_match2) ret = fwscanf(f, _formatString, _match1, _match2);
        else if(_match1) ret = fwscanf(f, _formatString, _match1);
        else ret = fwscanf(f, _formatString);

        if(ret == EOF)
        {
            readPos = 0;
            fclose(f);
            return false;
        }

        readPos += ((ftell(f)>>1) - pos);
        fclose(f);

        return true;
    }
};

class jKeyFile
{
private:
    wchar_t**       keys;
    wchar_t**       data;
    unsigned int    count;

    wchar_t         path[MAX_PATH];
public:
    jKeyFile()
    {
        keys = data = 0;
        count = 0;
    }

    ~jKeyFile()
    {
        for(unsigned int i = 0; i < count; ++i) delete keys[i];
        for(unsigned int i = 0; i < count; ++i) delete data[i];
        if(keys) delete keys;
        if(data) delete data;
    }

    bool AddKey(const wchar_t* _keyname, const wchar_t* _value)
    {
        unsigned int l = wcslen(_keyname) + 1;
        
        ++count;
        keys = (wchar_t**)realloc(keys, sizeof(wchar_t)*count);
        
        keys[count-1] = new wchar_t[l];
        wcscpy(keys[count-1], _keyname);

        l = wcslen(_value) + 1;
        data = (wchar_t**)realloc(data, sizeof(wchar_t)*count);
        data[count-1] = new wchar_t[l];
        wcscpy(data[count-1], _value);

        return true;
    }

    bool Load(const wchar_t* _path)
    {
        memset(path, 0, 255*sizeof(wchar_t));
		wcscpy(path, _path);

        jTextFile f;
        if(!f.Load(path)) return false;
        wchar_t* p = f.GetString();
        wchar_t keyname[255];
        wchar_t keydata[255];
        // s is the start position of the string we want to extract, e is the end
        unsigned int s = 0, e = 0, i = 0, l = 0;
        for(unsigned int i = 0; i < f.GetStringLength(); ++i)
        {
            // key=data\r\n
            if(p[i] == L'=')
            {
                if(p[i + 1] == L'=') ++i;
                else
                {
                    e = i;
                    // read in the key, treating two consecutive = as a single =
                    l = e - s;
                    if(l > 255) return false;
                    for(i = 0; i < l; ++i)
                    {
                        if(p[s + i] == L'=')
                        {
                            ++s;
                            --l;
                        }
                        keyname[i] = p[s + i];
                    }
                    keyname[++i] = 0;
                    s = e + 1;
                }
            }
            else if(p[i] == L'\r')
            {
                e = i;
                // read in the data, treating two consecutive = as a single =
                l = e - s;
                if(l > 255) return false;
                for(i = 0; i < l; ++i)
                {
                    if(p[s + i] == L'=')
                    {
                        ++s;
                        --l;
                    }
                    keydata[i] = p[s + i];
                }
                keydata[++i] = 0;
                s = e + 2;
                ++i;

                AddKey(keyname, keydata);
            }
        }

        return true;
    }

    bool Save() const
    {
        jTextFile f;
        if(!f.New(path)) return false;

        for(unsigned int i = 0; i < count; ++i)
        {
            f.Append(keys[i], false);
            f.Append(L"=", false);
            f.AppendLine(data[i], false);
        }

        f.Save();

        return true;
    }

    bool SaveAs(const wchar_t* _path)
    {
        memset(path, 0, 255*sizeof(wchar_t));
		wcscpy(path, _path);

        return Save();
    }

    wchar_t* GetValue(const wchar_t* _key) const
    {
        for(unsigned int i = 0; i < count; ++i)
        {
            if(!_wcsicmp(_key, keys[i]))
            {
                return data[i];
            }
        }
    }
};