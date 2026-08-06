#include <Includes.hpp>

static char inputText[256] = "";
static char inputMulti[1024] = "";

static int selectedAssembly = 0;
static std::vector<std::string> assemblyNames;

static void DrawAssemblies() {
    if (!BNM::IsLoaded()) {
        ImGui::Text("BNM not loaded");
        return;
    }
    if (assemblyNames.empty()) {
        auto imgs = BNM::Image::GetImages();
        for (auto &img : imgs) {
            const char *n = img.GetInfo() ? img.GetInfo()->name : nullptr;
            assemblyNames.emplace_back(n ? n : "(unnamed)");
        }
    }
    if (ImGui::BeginCombo("Assembly", (size_t) selectedAssembly < assemblyNames.size() ? assemblyNames[selectedAssembly].c_str() : "Select", ImGuiComboFlags_HeightLargest))
    {
        for (size_t i = 0; i < assemblyNames.size(); ++i)
        {
            bool sel = (size_t) selectedAssembly == i;
            if (ImGui::Selectable(assemblyNames[i].c_str(), sel)) selectedAssembly = (int) i;
            if (sel) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Text("Assembly count: %zu", assemblyNames.size());
}

void DrawMenu() {

    ImGui::Begin("Demo");
    if (ImGui::BeginTabBar("MainTab")) {

        if (ImGui::BeginTabItem("Main")) {

            ImGui::InputText("Input", inputText, sizeof(inputText));
            ImGui::InputTextMultiline("Multi", inputMulti, sizeof(inputMulti), ImVec2(-1, 100));

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Assemblies")) {

            DrawAssemblies();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Paths")) {

            static std::string documents = GetDocumentsPath();
            static std::string download = GetDownloadPath();
            static std::string pictures = GetPicturePath();
            static std::string dcim = GetDCIMPath();

            ImGui::Text("Documents: %s", documents.c_str());
            ImGui::Text("Download: %s", download.c_str());
            ImGui::Text("Pictures: %s", pictures.c_str());
            ImGui::Text("DCIM: %s", dcim.c_str());

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

EGLBoolean(*orig_eglSwapBuffers)(EGLDisplay, EGLSurface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!setup) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
        io.IniFilename = nullptr;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.Fonts->AddFontFromFileTTF("/system/fonts/NotoSansCJK-Regular.ttc", 40.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(2.5f);
        ImGui_ImplOpenGL3_Init("#version 300 es");
        setup = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    static bool prevWantTextInput = false;
    bool wantTextInput = ImGui::GetIO().WantTextInput;
    if (wantTextInput != prevWantTextInput) {
        prevWantTextInput = wantTextInput;
        ShowKeyboard(wantTextInput);
    }
    DrainInputQueue();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2((float)glWidth / 2, (float)glHeight / 2), ImGuiCond_Once);
    DrawMenu();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
}

void *input_thread(void *) {
    void* egl = DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
    void* inp = DobbySymbolResolver("libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");

    if (egl) DobbyHook(egl, (void*)_eglSwapBuffers, (void**)&orig_eglSwapBuffers);
    if (inp) DobbyHook(inp, (void*)myInput, (void**)&origInput);

    return nullptr;
}

void *MainThread(void *) {
    bool load = false;
    for (int i = 0; i < 10; i++) {
        JNIEnv *env = nullptr;
        if (jvm && jvm->AttachCurrentThread(&env, nullptr) == JNI_OK && env) {
            load = BNM::Loading::TryLoadByJNI(env, nullptr);
            jvm->DetachCurrentThread();
            if (load) break;
        }
        sleep(1);
    }
    if (!load) LOGI("BNM load failed");
    return nullptr;
}

__attribute__((constructor))
void libmain() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, nullptr, input_thread, nullptr);
    pthread_create(&thread2, nullptr, MainThread, nullptr);
    pthread_detach(thread1);
    pthread_detach(thread2);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jvm = vm;
    // Load Dex From Memory
    LoadDex(imgui_dex, imgui_dex_len);
   // Toast("Testing Toast");
    return JNI_VERSION_1_6;
}
