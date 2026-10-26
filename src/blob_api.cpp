#include <lunarutils/blob.hpp>
#include <lunarutils/types.hpp>
#include <cstring>
#include <cstdint>
#include <format>

ByondBlob * from_ptr(CByondValue &val) {
    CByondValue pval;
    Byond_ReadVar(&val, "ptr", &pval);
    return (ByondBlob *) pval.data.ref;
}

BYOND_API_METHOD(lu_blob_close) {
    if (argc != 1) {
        Byond_CRASH("close requires src");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    blob->close();
    delete blob;
    return {};
}

BYOND_API_METHOD(lu_blob_read) {
    if (argc != 2) {
        Byond_CRASH("read requires src and amt");
        return {};
    }
    CByondValue src = argv[0];
    CByondValue _amt = argv[1];
    ByondBlob * blob = from_ptr(src);
    int amt = _amt.data.num;
    void * buffer = malloc(amt+1);
    memset(buffer, 0, amt+1);
    int ramt = blob->read(amt, buffer);
    char * str = (char *) buffer;
    CByondValue rval;
    ByondValue_SetStr(&rval, str);
    return rval;
}

BYOND_API_METHOD(lu_blob_read8) {
    if (argc != 2) {
        Byond_CRASH("read requires src and signed");
        return {};
    }
    CByondValue src = argv[0];
    CByondValue _is_sign = argv[1];
    ByondBlob * blob = from_ptr(src);
    bool is_sign = _is_sign.data.num != 0;
    uint8_t buffer = 0;
    int ramt = blob->read(1, &buffer);
    CByondValue rval;
    if (is_sign)
        ByondValue_SetNum(&rval, (int8_t) buffer);
    else
        ByondValue_SetNum(&rval, buffer);
    return rval;
}

BYOND_API_METHOD(lu_blob_read16) {
    if (argc != 3) {
        Byond_CRASH("read requires src, signed, and be");
        return {};
    }
    CByondValue src = argv[0];
    CByondValue _is_sign = argv[1];
    CByondValue _is_be = argv[2];
    ByondBlob * blob = from_ptr(src);
    bool is_sign = _is_sign.data.num != 0;
    bool is_be = _is_be.data.num != 0;
    uint16_t buffer = 0;
    int ramt = blob->read(2, &buffer);
    if (is_be)
        buffer = (buffer >> 8) | (buffer << 8);
    CByondValue rval;
    if (is_sign)
        ByondValue_SetNum(&rval, (int16_t) buffer);
    else
        ByondValue_SetNum(&rval, buffer);
    return rval;
}

BYOND_API_METHOD(lu_blob_readfloat) {
    if (argc != 2) {
        Byond_CRASH("read requires src and be");
        return {};
    }
    CByondValue src = argv[0];
    CByondValue _is_be = argv[1];
    ByondBlob * blob = from_ptr(src);
    bool is_be = _is_be.data.num != 0;
    uint32_t buffer = 0;
    int ramt = blob->read(4, &buffer);
    if (is_be) {
        buffer = ((buffer & 0xFF) << 24) |
                 ((buffer & 0xFF00) << 8) |
                 ((buffer & 0xFF0000) >> 8) |
                 (buffer >> 24);
    }
    CByondValue rval;
    ByondValue_SetNum(&rval, 0);
    rval.data.ref = buffer; // lol
    return rval;
}

BYOND_API_METHOD(lu_blob_seek) {
    if (argc != 3) {
        Byond_CRASH("seek requires src, whence, and offset");
        return {};
    }
    CByondValue src = argv[0];
    CByondValue _whence = argv[1];
    CByondValue _offset = argv[2];
    ByondBlob * blob = from_ptr(src);
    int whence = _whence.data.num;
    int offset = _offset.data.num;
    if (whence < 0 || whence > 2) {
        Byond_CRASH("invalid position to seek from");
        return {};
    }
    int pos = blob->seek(whence, offset);
    CByondValue rval;
    ByondValue_SetNum(&rval, pos);
    return rval;
}

BYOND_API_METHOD(lu_blob_size) {
    if (argc != 1) {
        Byond_CRASH("size requires src");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    int size = blob->size();
    CByondValue rval;
    ByondValue_SetNum(&rval, size);
    return rval;
}

BYOND_API_METHOD(lu_blob_write) {
    if (argc != 2) {
        Byond_CRASH("write requires src and data");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    CByondValue _data = argv[1];
    std::string str = ByondValue_ToString(&_data);
    return (blob->write(str.length()-1, str.data()) == str.length()-1) ? ByondTrue : ByondFalse;
}

BYOND_API_METHOD(lu_blob_write8) {
    if (argc != 2) {
        Byond_CRASH("write requires src and data");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    CByondValue _data = argv[1];
    // CByondValue _signed = argv[2];
    
    // bool issigned = _signed.data.ref;
    char val = _data.data.num;

    int written = 0;

    /* if (issigned) {
        signed char vwrite = val;
        written = blob->write(1, &vwrite);
    } else {
        char vwrite = val;
        written = blob->write(1, &vwrite);
    } */
    written = blob->write(1, &val);
    return (written == 1) ? ByondTrue : ByondFalse;
}

BYOND_API_METHOD(lu_blob_write16) {
    if (argc != 3) {
        Byond_CRASH("write requires src, data, and be");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    CByondValue _data = argv[1];
    CByondValue _be = argv[2];

    short wval = _data.data.num;

    if (_be.data.ref != 0) {
        wval = (wval >> 8) | (wval << 8);
    }

    return (blob->write(2, &wval) == 2) ? ByondTrue : ByondFalse;
}

BYOND_API_METHOD(lu_blob_writefloat) {
    if (argc != 3) {
        Byond_CRASH("write requires src, data, and be");
        return {};
    }
    CByondValue src = argv[0];
    ByondBlob * blob = from_ptr(src);
    CByondValue _data = argv[1];
    CByondValue _be = argv[2];

    float _wval = _data.data.num;
    int buffer = *((int*) &_wval);

    if (_be.data.ref != 0) {
        buffer = ((buffer & 0xFF) << 24) |
                 ((buffer & 0xFF00) << 8) |
                 ((buffer & 0xFF0000) >> 8) |
                 (buffer >> 24);
    }

    return (blob->write(4, &buffer) == 4) ? ByondTrue : ByondFalse;
}

BYOND_API_METHOD(lu_blob_new) {
    if (argc != 0) {
        Byond_CRASH("lu_blob_new takes no args");
        return {};
    }

    CByondValue val;
    std::string path = ByondValue_ToString(argv);
    CByondValue _mode = argv[1];
    int mode = _mode.data.num;
    try {
        ByondBlob * blob = new VectorBlob();
        blob->set(val);
    } catch(const std::exception &e) {
        Byond_CRASH(std::format("C++ Exception: {}", e.what()).c_str());
    }
    return val;
}