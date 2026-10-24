#pragma once

#include <lunarutils/helpers.hpp>

#define LU_BLOB_READ 1
#define LU_BLOB_WRITE 2

class ByondBlob {
    private:
        char mode = 0;
        int offset = 0;
        virtual int read_in(int pos, int amt, void * buffer) = 0;
        virtual int write_out(int pos, int amt, void * buffer) = 0;
    public:
        virtual int size() = 0;
        int read(int amt, void * buffer);
        int seek(int whence, int offset);
        int write(int amt, void * buffer);
        virtual void close() = 0;

        void set(CByondValue &val);
};

BYOND_API_DEF(lu_blob_openfile);
BYOND_API_DEF(lu_blob_read);
BYOND_API_DEF(lu_blob_read8);
BYOND_API_DEF(lu_blob_read16);
BYOND_API_DEF(lu_blob_readfloat);
BYOND_API_DEF(lu_blob_seek);
BYOND_API_DEF(lu_blob_close);
BYOND_API_DEF(lu_blob_size);
BYOND_API_DEF(lu_blob_write);
BYOND_API_DEF(lu_blob_write8);
BYOND_API_DEF(lu_blob_write16);
BYOND_API_DEF(lu_blob_writefloat);