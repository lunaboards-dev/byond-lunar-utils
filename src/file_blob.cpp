#include <lunarutils/blob.hpp>
#include <lunarutils/types.hpp>
#include <cstdio>
#include <stdexcept>
#include <format>

char * modes[] = {"rb", "wb", "r+b"};

class FileBlob : public ByondBlob {
    private:
        FILE * handle;
        int read_in(int offset, int amt, void * buffer) {
            fseek(handle, SEEK_SET, offset);
            return fread(buffer, 1, amt, handle);
        }
        int write_out(int offset, int amt, void * buffer) {
            fseek(handle, SEEK_SET, offset);
            return fwrite(buffer, 1, amt, handle);
        }
    public:
        int size() override {
            return fseek(handle, SEEK_END, 0);
        }

        void close() override {
            fclose(handle);
        }

        FileBlob(const char * path, char mode) : ByondBlob() {
            if (mode < 1 || mode > 2) {
                throw std::invalid_argument("mode must be READ or WRITE");
            }
            this->mode = mode;
            handle = fopen(path, modes[mode-1]);
            if (handle == nullptr) {
                throw std::runtime_error("file not found");
            }
        }
};

BYOND_API_METHOD(lu_blob_openfile) {
    if (argc != 2) {
        Byond_CRASH("lu_openfile requires a path and a mode");
        return {};
    }
    CByondValue val;
    std::string path = ByondValue_ToString(argv);
    CByondValue _mode = argv[1];
    int mode = _mode.data.num;
    try {
        ByondBlob * blob = new FileBlob(path.c_str(), mode);
        blob->set(val);
    } catch(const std::exception &e) {
        Byond_CRASH(std::format("C++ Exception: {}", e.what()).c_str());
    }
    return val;
}