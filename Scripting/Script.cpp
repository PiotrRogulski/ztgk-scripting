/* Script Class Implementation */
#include "Script.h"

#include "GameObject.h"

namespace duck_app {
    /* TODO (1.12) Implement Constructor That Initializes Script File Instance */
    Script::Script(const std::string& filePath) : m_scriptFile(filePath) { }

    Script::~Script() = default;

    /* TODO (1.13) Implement Initialize Method */
    /*
        * Sets Path Variable (Environment) To Contain Current Script Location (Catalog)
        * Imports Script Module
        * Gets Class From The Script Implementation
        * Constructs New Object Of The Given Script Class
        * Gets All Implemented Methods From Script Implementation
        * Sets Given Game Object As Class Member
    */
    void Script::Initialize(GameObject* gameObject) {
        PySys_SetPath(std::wstring(this->m_scriptFile.scriptName.begin(), this->m_scriptFile.scriptName.end()).c_str());

        auto pyModule = py::module::import(this->m_scriptFile.scriptName.c_str());
        if (pyModule) {
            auto pyClass = pyModule.attr(this->m_scriptFile.scriptName.c_str());

            this->pyClassObject = pyClass();

            if (this->pyClassObject) {
                this->pyClassObject.attr("gameObject") = gameObject;

                this->onCreateMethod = this->pyClassObject.attr("OnCreate");
                this->onUpdateMethod = this->pyClassObject.attr("OnUpdate");
            }
        }
    }

    /* TODO (1.14) Implement FileChanged Method That Checks If File Has Changed */
    bool Script::FileChanged() {
        return this->m_scriptFile.FileChanged();
    }

    /* TODO (1.15) Implement OnUpdate Method That Calls Python OnUpdate Sctipt Mathod */
    void Script::OnUpdate() const {
        if (this->onUpdateMethod) {
            this->onUpdateMethod();
        }
    }

    /* TODO (1.16) Implement OnCreate Method That Calls Python OnCreate Sctipt Mathod */
    void Script::OnCreate() const {
        if (this->onCreateMethod) {
            this->onCreateMethod();
        }
    }
}
