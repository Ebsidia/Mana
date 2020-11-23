#include "mapch.h"
#include "Mana/Utilities/PlatformUtilities.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Mana/Core/Application.h"

namespace Mana {

    std::string FileDialogs::openFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR sizeFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile = sizeFile;
        ofn.nMaxFile = sizeof(sizeFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }

        return std::string();
    }

    std::string FileDialogs::saveFile(const char* filter)
    {
        OPENFILENAMEA ofn;
        CHAR sizeFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile = sizeFile;
        ofn.nMaxFile = sizeof(sizeFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }

        return std::string();
    }
}