//
// Created by kolya on 04.12.2022.
//

#ifndef AUTO_GREENHOUSE_FILEMEMMAP_H
#define AUTO_GREENHOUSE_FILEMEMMAP_H


#include <windows.h>
#include <winbase.h>
#include <string>

class FileMemMap {
private:
    HANDLE fileMem;
    void *buffer;

public:
    FileMemMap(const char *name) {
        std::string buf = "FILEMEM_" + std::string(name);
        const char *newName = buf.c_str();
        this->fileMem = OpenFileMappingA(
                FILE_MAP_ALL_ACCESS,
                false,
                (LPCSTR) newName
        );
        if (this->fileMem == nullptr)
            this->fileMem = CreateFileMappingA(
                    INVALID_HANDLE_VALUE,
                    nullptr,
                    PAGE_READWRITE,
                    0, 4096,
                    (LPCSTR) newName
            );

        if (this->fileMem != nullptr)
            this->buffer = MapViewOfFile(
                    this->fileMem,
                    FILE_MAP_ALL_ACCESS,
                    0,
                    0,
                    4096
            );
        else printf("error: FILE_MAP!\n");
    }

    void set(int data) {
        *( (int*) buffer) = data;
    }

    int get() {
        return *( (int*) buffer);
    }
};


#endif //AUTO_GREENHOUSE_FILEMEMMAP_H
