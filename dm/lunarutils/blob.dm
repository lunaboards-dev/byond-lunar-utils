#define IS_CLOSED (ptr == null)
#define LOAD_EXT(call) call = load_ext(world.system_type == "windows" ? "./lunarutils.dll" : "./liblunarutils.so", "byond:"+#call)

/datum/lu_blob
    var/ptr

    var/global/lu_blob_read
    var/global/lu_blob_read8
    var/global/lu_blob_read16
    var/global/lu_blob_readfloat
    var/global/lu_blob_seek
    var/global/lu_blob_size
    var/global/lu_blob_close
    var/global/lu_blob_write
    var/global/lu_blob_write8
    var/global/lu_blob_write16
    var/global/lu_blob_writefloat

/datum/lu_blob/proc/open_lib()
    if (lu_blob_read != null)
        return
    LOAD_EXT(lu_blob_read)
    LOAD_EXT(lu_blob_read8)
    LOAD_EXT(lu_blob_read16)
    LOAD_EXT(lu_blob_readfloat)
    LOAD_EXT(lu_blob_seek)
    LOAD_EXT(lu_blob_size)
    LOAD_EXT(lu_blob_close)
    LOAD_EXT(lu_blob_write)
    LOAD_EXT(lu_blob_write8)
    LOAD_EXT(lu_blob_write16)
    LOAD_EXT(lu_blob_writefloat)

/datum/lu_blob/proc/closed()
    open_lib()
    return IS_CLOSED

/datum/lu_blob/proc/read(count)
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_read)(src, count)

/datum/lu_blob/proc/read8(signed = FALSE)
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_read8)(src, signed)

/datum/lu_blob/proc/read16(signed = FALSE, be = FALSE)
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_read16)(src, signed, be)

/datum/lu_blob/proc/readfloat(be = FALSE)
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_readfloat)(src, be)

/datum/lu_blob/proc/seek(whence = SEEK_CUR, offset = 0)
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_seek)(src, whence, offset)

/datum/lu_blob/proc/size()
    open_lib()
    if IS_CLOSED
        throw "cannot read from closed handle"
    return call_ext(lu_blob_size)(src)

/datum/lu_blob/proc/close()
    open_lib()
    if (!IS_CLOSED)
        return call_ext(lu_blob_close)(src)

/datum/lu_blob/proc/write(str)
    open_lib()
    if (!IS_CLOSED)
        return call_ext(lu_blob_write)(src, str)

/datum/lu_blob/proc/write8(data)
    open_lib()
    if (!IS_CLOSED)
        return call_ext(lu_blob_write8)(src, data)

/datum/lu_blob/proc/write16(data, be = FALSE)
    open_lib()
    if (!IS_CLOSED)
        return call_ext(lu_blob_write16)(src, data, be)

/datum/lu_blob/proc/writefloat(data, be = FALSE)
    open_lib()
    if (!IS_CLOSED)
        return call_ext(lu_blob_writefloat)(src, data, be)

/datum/lu_blob/proc/writestr(data)
    write16(length(data))
    write(data)

/datum/lu_blob/proc/writestr8(data)
    write16(length(data))
    write(data)

/datum/lu_blob/proc/readstr(data)
    var/len = read16()
    return read(len)

/datum/lu_blob/proc/readstr8(data)
    var/len = read8()
    return read(len)

/proc/lu_openfile(path, mode = OPEN_READ)
    return call_ext(world.system_type == "windows" ? "./lunarutils.dll" : "./liblunarutils.so", "byond:lu_blob_openfile")(path, mode)

#undef IS_CLOSED
#undef LOAD_EXT