#ifndef SCRIPT_HEADER
#define SCRIPT_HEADER

/* System Includes */
#include <filesystem>
#include <string>

/* TODO (1.05) Include Python And PyBind11 Headers */
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace duck_app {
    class GameObject; /* Forward Declaration Of GameObject */

    class Script {
        struct ScriptFile {
            std::string scriptPathFull;

            std::string scriptName; /* Module Name */
            std::string scriptPath; /* Module Path */


            /* TODO (1.01) Script File Modification Time (C++17) */
            std::filesystem::file_time_type modificationTime;

            /* TODO (1.02) Create Constructor That Takes String Argument Representing Path To The File */
            /*
                * Initializes Filesystem Path With Given String
                * Takes Filename And Extension From Filesystem Path
                * Takes Script Name And Catalg Path
                * Takes Modification Time
            */
            explicit ScriptFile(const std::string& filePath) {
                this->scriptPathFull = filePath;

                const std::filesystem::path filesystemPath{filePath};

                this->scriptName = filesystemPath.stem().string();
                this->scriptPath = filesystemPath.parent_path().string();

                this->modificationTime = std::filesystem::last_write_time(this->scriptPathFull);
            }


            /* TODO (1.03) Create FileChanged Method That Checks If File Has Changed */
            /*
                * Takes Current Modification Time
                * Compares It With Stored Modificaiton Time
            */
            bool FileChanged() {
                if (!std::filesystem::exists(this->scriptPathFull)) {
                    return false;
                }

                const auto currentModificationFile = std::filesystem::last_write_time(this->scriptPathFull);

                if (this->modificationTime != currentModificationFile) {
                    this->modificationTime = currentModificationFile;

                    return true;
                }

                return false;
            }
        };

        /* TODO (1.04) Create Script File Instance */
        ScriptFile m_scriptFile;

        /* TODO (1.06) Create Instance Of Script Class */
        py::object pyClassObject;

        /* TODO (1.07) Create Instances Of Script Methods */
        py::object onCreateMethod;
        py::object onUpdateMethod;

    public:
        /* TODO (1.08) Create Constructor For Class */
        explicit Script(const std::string& filePath);

        ~Script();

        /* TODO (1.09) Create Initialize Method That Takes GameObject Pointer Argument */
        void Initialize(GameObject* gameObject);

        /* TODO (1.10) Create FileChanged Method That Checks If Script Source Has Changed */
        bool FileChanged();

        /* TODO (1.11) Create OnCreate And OnUpdate Methods */
        void OnCreate() const;

        void OnUpdate() const;
    };
}

#endif /* SCRIPT_HEADER */
