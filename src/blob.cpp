#include <lunarutils/blob.hpp>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <format>
#include <lunarutils/types.hpp>

int ByondBlob::read(int amt, void *buffer) {
    if (mode & LU_BLOB_READ == 0) throw new std::runtime_error("attempt to read from write only blob");
    int ramt = this->read_in(this->offset, amt, buffer);
    this->offset += ramt;
    return ramt;
}

int ByondBlob::write(int amt, void * buffer) {
    if (mode & LU_BLOB_READ == 0) throw new std::runtime_error("attempt to write to read only blob");
    int ramt = this->write_out(this->offset, amt, buffer);
    this->offset += ramt;
    return ramt;
}

int ByondBlob::seek(int whence, int offset) {
    if (whence == 0) { // SEEK_CUR
        this->offset += offset;
    } else if (whence == 1) { // SEEK_SET
        this->offset = offset;
    } else if (whence == 2) { // SEEK_END
        this->offset = this->size() + offset;
    }
    return this->offset;
}

void ByondBlob::set(CByondValue &val) {
    CByondValue path;
    ByondValue_SetStr(&path, "/datum/lu_blob");
    bool ok = Byond_New(&path, nullptr, 0, &val);
    if (!ok) Byond_CRASH("error creating blob object");
    CByondValue ptr;
    ByondValue_SetNum(&ptr, 0);
    ptr.data.ref = (unsigned int) this;
    Byond_WriteVar(&val, "ptr", &ptr);
}

VectorBlob::VectorBlob() : ByondBlob() {
    this->mode = LU_BLOB_READ | LU_BLOB_WRITE;
}

int VectorBlob::write_out(int pos, int amt, void * buffer) {
    if (pos+amt < memory.size()) {
        memory.resize(pos+amt);
    }
    memcpy(memory.data()+pos, buffer, amt);
    return amt;
}

int VectorBlob::read_in(int pos, int amt, void * buffer) {
    if (pos+amt < memory.size()) {
        amt = memory.size()-pos;
    }
    memcpy(buffer, memory.data()+pos, amt);
    return amt;
}

int VectorBlob::size() {
    return memory.size();
}

void VectorBlob::close() {
    // do nothing here
}

MemoryBlob::MemoryBlob(void * src, int size, bool free) : ByondBlob() {
    ptr = src;
    should_free = free;
    isize = size;
}

int MemoryBlob::write_out(int pos, int amt, void * buffer) {
    throw new std::runtime_error("write to memory blob is not allowed and should not be possible");
    return 0;
}

int MemoryBlob::read_in(int pos, int amt, void * buffer) {
    if (pos+amt < isize) {
        amt = isize-pos;
    }
    memcpy(buffer, ptr+pos, amt);
    return amt;
}

void MemoryBlob::close() {
    if (should_free) {
        free(ptr);
    }
}

int MemoryBlob::size() {
    return isize;
}