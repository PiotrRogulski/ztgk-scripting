/* ScriptManager Class Implementation */
#include "ScriptManager.h"

/* TODO (2.07) Include Python And PyBind11 Headers */
#include <pybind11/embed.h>

namespace py = pybind11;

namespace duck_app {
    /* TODO (2.08) Implement Constructor And Destructor */
    /*
        * Constructor Initializes Python (Initialize Method)
        * Destructor Finalizes Python (Finalize Method)
    */
    ScriptManager::ScriptManager() {
        InitializePython();
    }

    ScriptManager::~ScriptManager() {
        FinalizePython();
    }

    /* TODO (2.09) Implement InitializePython Method */
    /*
        * Sets Python Home Environment Variable
        * Initializes Python Interpreter
        * Imports All Necessary Modules
    */
    void ScriptManager::InitializePython() {
        wchar_t pythonHome[] = L"../Common/Python";

        Py_SetPythonHome(pythonHome);

        py::initialize_interpreter();

        py::module::import("random");
        py::module::import("API");
    }

    /* TODO (2.10) Implement FinalizePython Method */
    /*
        * Finalize Python Interpreter
    */
    void ScriptManager::FinalizePython() {
        py::finalize_interpreter();
    }


    /* TODO (2.11) Implement ScriptsChanged Method */
    /*
        * Iterate Through All Game Objects With Scripts And Check If Any Has Changed
        * If Any Script Has Been Changed Reinitialize Python Interpreter (Finalize And Initialize)
        * Initialize All Game Object's Scripts One Again
    */
    void ScriptManager::ScriptsChanged() {
        const auto scriptChanged = std::any_of(this->gameObjects.begin(), this->gameObjects.end(),
                                               [](const GameObject* gameObject) {
                                                   return gameObject->GetScript()->FileChanged();
                                               });

        if (scriptChanged) {
            this->FinalizePython();
            this->InitializePython();

            for (const auto gameObject : gameObjects) {
                gameObject->GetScript()->Initialize(gameObject);
            }
        }
    }

    /* TODO (2.12) Implement AddGameObjectsSctipt Method That Pushes Back Game Object Pointer */
    void ScriptManager::AddGameObject(GameObject& gameObject) {
        this->gameObjects.push_back(&gameObject);
    }

    /* TODO (2.13) Implement DispatchOnCreate And DispatchOnUpdate Method */
    /*
        * Checks If Script File Has Changed
        * Calls Appropriate Method On Game Object Instance
    */
    void ScriptManager::DispatchOnCreate() {
        this->ScriptsChanged();

        for (const auto gameObject : gameObjects) {
            gameObject->GetScript()->OnCreate();
        }
    }

    void ScriptManager::DispatchOnUpdate() {
        this->ScriptsChanged();

        for (const auto gameObject : gameObjects) {
            gameObject->GetScript()->OnUpdate();
        }
    }
}
